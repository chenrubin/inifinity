/****************************************
*	Author : ChenR						*
*	Reviewer: Tsisana					*
*	Counting letters in dictionary		*
*	24/1/2020 							*
****************************************/

#include <sys/mman.h> /* mmap, munmap */
#include <stdio.h>	/* printing errno */
#include <unistd.h> /* lseek */
#include <fcntl.h> /* O_RDONLY */
#include <string.h> /* strtok */
#include <stdlib.h> /* malloc */
#include <ctype.h> /* toupper */
#include <pthread.h> /* pthread_create */
#include <assert.h> /* assert */
#include <errno.h> /* errno */

#define MULTIPLYER (8)
#define NUM_OF_LETTERS_IN_ENGLISH (26)
#define ASCII_ZERO_POINT ('A')
#define WORDS_TO_DISREGARD 0

typedef struct wrap
{
	char **str;
	size_t size;
}thread_param_t;

/* copy dictionary from file to buffer using mmap */
static char *CopyDictToStringIMP();

/* get file size for mmap */
static size_t GetFileSizeIMP(int file_descriptor);

/* count words in dictionary */
static size_t CountWordsIMP(char *str);

/* insert strings from buffer to array */
static void InsertWordsToArrayIMP(char **arr, char *word_list, size_t size);

/* thread routine that counts letters in a given chunk in an array of strings */
static void *CountingLettersInEachThreadIMP(void *param);

/* count the letters in each string */
static void CountingLettersInEachWordIMP(char *str, size_t *histogram);

/* sum all threads' histograms to one */
static size_t *SumAllThreadsResultsIMP(size_t *res_histo, 
									   size_t *thread_histogram);

/* This is the main function for counting letters from dictionary */
size_t *CountingLetters(size_t num_of_threads);

/* update thread params */
static void UpdateThreadParamsIMP(thread_param_t *param, 
								  char **arr_of_strings, 
								  size_t amount_per_thread, 
								  size_t num_of_words,
								  size_t num_of_current_thread,
								  size_t num_of_threads);
								  
static char *CopyDictToStringIMP(size_t *file_size)
{
	char *word_list = NULL;
	int fd = open("/usr/share/dict/american-english", O_RDONLY);
	
	*file_size = GetFileSizeIMP(fd);
    word_list = mmap(NULL, 
    				 *file_size, 
    				 PROT_READ | PROT_WRITE, 
    				 MAP_PRIVATE, 
    				 fd, 
    				 0);

	return word_list;
}

static size_t GetFileSizeIMP(int file_descriptor)
{
	size_t size = 0;
	
	assert(file_descriptor);
	
	size = lseek(file_descriptor, 0, SEEK_END);
	lseek(file_descriptor, 0, SEEK_SET);
	
	return size;
}

static size_t CountWordsIMP(char *str)
{
	size_t num_of_words = 0;
	char *token = NULL;
	char *tmp_str = NULL;
	
	assert(str);
	
	tmp_str = str;
	
	token = strtok(tmp_str, ",. !\n");
	while (NULL != token)
	{
		token = strtok(NULL, ",. !\n");
		++(num_of_words);
	}
	
	return num_of_words;
}

static void InsertWordsToArrayIMP(char **arr, char *word_list, size_t size)
{
	int i = 0;
	char *token = NULL;
	
	assert(word_list);
	
	token = strtok(word_list, ",. !\n");
	 
	while (NULL != token)
	{
		arr[i] = token;
		++i;
		token = strtok(NULL, ",. !\n");
	}
	
	for (i = 1; i < MULTIPLYER; ++i)
	{
		memcpy(arr + (i * size), arr, size * sizeof(void *));
	}
}

static void *CountingLettersInEachThreadIMP(void *param)
{
	size_t i = 0;
	char **str = NULL;
	size_t size = 0;
	size_t *quantity_histogram = NULL;
	
	assert(param);
	
	str = ((thread_param_t *)param) -> str;
	size = ((thread_param_t *)param) -> size;
	quantity_histogram = (size_t *)calloc(NUM_OF_LETTERS_IN_ENGLISH, 
												  sizeof(size_t));
	
	if (NULL == quantity_histogram)
	{
		return NULL;
	}

	for (i = 0; i < size; ++i)
	{
		CountingLettersInEachWordIMP(*(str + i), quantity_histogram);
	}

	return quantity_histogram;
}

static void CountingLettersInEachWordIMP(char *str, size_t *histogram)
{
	assert(str);
	assert(histogram);
	
	while ('\0' != *str)
	{
		if (0 != isalpha(*str))
		{
			histogram[toupper(*str) - ASCII_ZERO_POINT] += 1;
		}
		++str;
	}
}

static size_t *SumAllThreadsResultsIMP(size_t *res_histo, 
									   size_t *thread_histogram)
{
	size_t i = 0;
	
	assert(res_histo);
	assert(thread_histogram);
	
	for (i = 0; i < NUM_OF_LETTERS_IN_ENGLISH; ++i)
	{
		res_histo[i] += thread_histogram[i];
	}
	
	return res_histo;
}

static void UpdateThreadParamsIMP(thread_param_t *param, 
								  char **arr_of_strings, 
								  size_t amount_per_thread, 
								  size_t num_of_words,
								  size_t num_of_current_thread,
								  size_t num_of_threads)
{	
	assert(param);
	assert(arr_of_strings);
	
	param -> str = arr_of_strings + num_of_current_thread * amount_per_thread;
	param -> size = amount_per_thread;
	if (num_of_current_thread == num_of_threads - 1)
	{
		param -> size = num_of_words - 
						(num_of_current_thread * amount_per_thread);
	}
}

size_t *CountingLetters(size_t num_of_threads)
{
	size_t i = 0;
	size_t file_size = 0;
	char **arr_of_strings = NULL;
	size_t num_of_words = 0;
	pthread_t *threads = NULL;
	size_t amount_per_thread = 0;
	thread_param_t *params = NULL;
	char *word_list_for_count = CopyDictToStringIMP(&file_size);
	char *word_list_for_assign_pointers = CopyDictToStringIMP(&file_size);
	size_t *res_histogram = NULL;
	void *thread_histogram = NULL;
	
	assert(num_of_threads);
	
	num_of_words = CountWordsIMP(word_list_for_count) - WORDS_TO_DISREGARD;

	arr_of_strings = (char **)malloc(MULTIPLYER * 
									 num_of_words * 
									 sizeof(void *));
	if (NULL == arr_of_strings)
	{
		return NULL;
	}
	
	InsertWordsToArrayIMP(arr_of_strings, 
					   word_list_for_assign_pointers, 
					   num_of_words);
	
	threads = (pthread_t *)malloc(num_of_threads * sizeof(pthread_t));
	if (NULL == threads)
	{
		free(arr_of_strings);
		
		return NULL;
	}
	
	num_of_words *= MULTIPLYER;
	amount_per_thread = num_of_words / num_of_threads;
	params = (thread_param_t *)malloc(num_of_threads * sizeof(thread_param_t)); 
	if (NULL == params)
	{
		free(threads);
		free(arr_of_strings);
		
		return NULL;
	}
	
	for (i = 0; i < num_of_threads; ++i)
	{
		UpdateThreadParamsIMP(params + i, 
							  arr_of_strings, 
							  amount_per_thread,
							  num_of_words,
							  i,
							  num_of_threads);
		pthread_create(threads + i, NULL, CountingLettersInEachThreadIMP, 
										  params + i);						  
	}
	
	res_histogram = (size_t *)calloc(NUM_OF_LETTERS_IN_ENGLISH, 
									 sizeof(size_t));
	if (NULL == res_histogram)
	{
		free(threads);
		free(arr_of_strings);
		free(params);
		
		return NULL;
	}
	
	for (i = 0; i < num_of_threads; ++i)
	{
		pthread_join(threads[i], &thread_histogram);
		SumAllThreadsResultsIMP(res_histogram, (size_t *)thread_histogram);
		free(thread_histogram);
	}
	
	if (-1 == munmap(word_list_for_count, file_size))
	{
		perror ("munmap failed\n");
    	printf( "Value of errno: %d\n", errno);
	}
	if (-1 == munmap(word_list_for_assign_pointers, file_size))
	{
		perror ("munmap failed\n");
    	printf( "Value of errno: %d\n", errno);
	}
	
	free(arr_of_strings);
	free(threads);
	free(params);
	
	return res_histogram;
}
