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
#define LETTERS_IN_ENGLISH (26)
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
static char **InsertWordsToArrayIMP(size_t *num_of_words, 
									size_t *file_size,
									char *word_list_for_count,
									char *word_list_for_assign_pointers);

/* thread routine that counts letters in a given chunk in an array of strings */
static void *CountingLettersInEachThreadIMP(void *param);

/* count the letters in each string */
static void CountingLettersInEachWordIMP(char *str, size_t *histogram);

/* sum all threads' histograms to one */
static size_t *SumAllThreadsResultsIMP(size_t *res_histo, 
									   size_t num_of_threads,
									   pthread_t *threads);

/* This is the main function for counting letters from dictionary */
size_t *CountingLetters(size_t num_of_threads);

/* function that manages all threads that count each segment */
static int ThreadsCountingLetters(pthread_t **threads, 
								  size_t num_of_words, 
								  size_t *num_of_threads, 
								  char **strings_ptr,
								  thread_param_t **params);

/* update thread params */
static void UpdateThreadParamsIMP(thread_param_t *param, 
								  char **arr_of_strings, 
								  size_t amount_per_thread, 
								  size_t num_of_words,
								  size_t num_of_current_thread,
								  size_t num_of_threads);

/* munmap  word_list_count and word_list_pointers */								  
static void FreeMapping(char *word_list_count, 
						char *word_list_pointers, 
						size_t file_size);

/* Free all allocations */
static void FreeAllocations(char **strings_ptr, 
							pthread_t *threads, 
							thread_param_t *params);													  
								  
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

static char **InsertWordsToArrayIMP(size_t *num_of_words, 
									size_t *file_size,
									char *word_list_for_count,
									char *word_list_for_assign_pointers)
{
	int i = 0;
	char *token = NULL;
	char **strings_ptr = NULL;
	
	word_list_for_count = CopyDictToStringIMP(file_size);
	word_list_for_assign_pointers = CopyDictToStringIMP(file_size);
	
	*num_of_words = CountWordsIMP(word_list_for_count) - WORDS_TO_DISREGARD;
	strings_ptr = (char **)malloc(MULTIPLYER * 
								 (*num_of_words) * 
								 sizeof(void *));
	if (NULL == strings_ptr)
	{
		return NULL;
	}
	
	token = strtok(word_list_for_assign_pointers, ",. !\n");
	while (NULL != token)
	{
		strings_ptr[i] = token;
		++i;
		token = strtok(NULL, ",. !\n");
	}
	
	for (i = 1; i < MULTIPLYER; ++i)
	{
		memcpy(strings_ptr + (i * (*num_of_words)), 
			   strings_ptr, 
			   *num_of_words * sizeof(void *));
	}
	
	return strings_ptr;
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
	quantity_histogram = (size_t *)calloc(LETTERS_IN_ENGLISH, 
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
									   size_t num_of_threads,
									   pthread_t *threads)
{
	size_t i = 0;
	size_t j = 0;
	void *thread_histogram = NULL;
	
	assert(res_histo);
	assert(num_of_threads);
	
	for (i = 0; i < num_of_threads; ++i)
	{
		pthread_join(threads[i], &thread_histogram);

		for (j = 0; j < LETTERS_IN_ENGLISH; ++j)
		{
			res_histo[j] += *((size_t *)thread_histogram + j);
		}
		
		free(thread_histogram);
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

static int ThreadsCountingLetters(pthread_t **threads, 
								  size_t num_of_words, 
								  size_t *num_of_threads, 
								  char **strings_ptr,
								  thread_param_t **params)
{
	size_t i = 0;
	size_t amount_per_thread = 0;
/*	thread_param_t *params = NULL;*/
	
	*threads = (pthread_t *)malloc(*num_of_threads * sizeof(pthread_t));
	if (NULL == *threads)
	{	
		return 1;
	}
	
	num_of_words *= MULTIPLYER;
	amount_per_thread = num_of_words / (*num_of_threads);
	*params = (thread_param_t *)malloc(*num_of_threads * sizeof(thread_param_t)); 
	if (NULL == params)
	{
		return 1;
	}
	
	for (i = 0; i < (*num_of_threads); ++i)
	{
		UpdateThreadParamsIMP(*params + i, 
							  strings_ptr, 
							  amount_per_thread,
							  num_of_words,
							  i,
							  *num_of_threads);
		pthread_create(*threads + i, 
					   NULL, 
					   CountingLettersInEachThreadIMP, 
					   *params + i);						  
	}
	
	return 0;
}

static void FreeMapping(char *word_list_count, 
						char *word_list_pointers, 
						size_t file_size)
{
	if (-1 == munmap(word_list_count, file_size))
	{
		perror ("munmap failed\n");
    	printf( "Value of errno: %d\n", errno);
	}
	
	if (-1 == munmap(word_list_pointers, file_size))
	{
		perror ("munmap failed\n");
    	printf( "Value of errno: %d\n", errno);
	}
}

static void FreeAllocations(char **strings_ptr, 
							pthread_t *threads, 
							thread_param_t *params)
{
	free(strings_ptr);
	free(threads);
	free(params);
}

size_t *CountingLetters(size_t num_of_threads)
{
	size_t file_size = 0;
	char **strings_ptr = NULL;
	size_t num_of_words = 0;
	pthread_t *threads = NULL;
	thread_param_t *params = NULL;
	size_t *res_histogram = NULL;
	char *word_list_count = NULL;
	char *word_list_pointers = NULL;
	
	assert(num_of_threads);
	
	strings_ptr = InsertWordsToArrayIMP(&num_of_words, 
										&file_size, 
										word_list_count, 
										word_list_pointers);
	if (NULL == strings_ptr)
	{
		return NULL;
	}
	
	if (1 == ThreadsCountingLetters(&threads,
									num_of_words, 
									&num_of_threads, 
									strings_ptr,
									&params))
	{
		free(threads);
		free(strings_ptr);
		
		return NULL;
	}								

	res_histogram = (size_t *)calloc(LETTERS_IN_ENGLISH, sizeof(size_t));
	if (NULL == res_histogram)
	{
		FreeAllocations(strings_ptr, threads, params);

		return NULL;
	}

	SumAllThreadsResultsIMP(res_histogram, num_of_threads,threads);
	FreeMapping(word_list_count, word_list_pointers, file_size);
	FreeAllocations(strings_ptr, threads, params);
	
	return res_histogram;
}
