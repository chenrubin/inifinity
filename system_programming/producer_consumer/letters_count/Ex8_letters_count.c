#include <sys/mman.h> /* mmap, munmap */
#include <stdio.h>
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
static size_t *SumAllThreadsResultsIMP(size_t *res_histo, size_t *thread_histo);

/* This is the main function for counting letters from dictionary */
size_t *CountingLetters(size_t num_of_threads);

static char *CopyDictToStringIMP(size_t *fsize)
{
	char *word_list = NULL;
	int fd = open("/usr/share/dict/american-english", O_RDONLY);
	
	*fsize = GetFileSizeIMP(fd);
    word_list = mmap(NULL, *fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	return word_list;
}

static size_t GetFileSizeIMP(int file_descriptor)
{
	size_t size = lseek(file_descriptor, 0, SEEK_END);
	
	lseek(file_descriptor, 0, SEEK_SET);
	
	return size;
}

static size_t CountWordsIMP(char *str)
{
	size_t counter = 0;
	char *token = NULL;
	char *tmp_str = str;
	
	token = strtok(tmp_str, ",. !\n");
	while (NULL != token)
	{
		token = strtok(NULL, ",. !\n");
		++(counter);
	}
	
	return counter;
}

static void InsertWordsToArrayIMP(char **arr, char *word_list, size_t size)
{
	int i = 0;
	char *token = NULL;
	
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
	char **str = ((thread_param_t *)param) -> str;
	size_t size = ((thread_param_t *)param) -> size;
	size_t *quantity_histogram = (size_t *)calloc(NUM_OF_LETTERS_IN_ENGLISH, 
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
	while ('\0' != *str)
	{
		if (0 != isalpha(*str))
		{
			histogram[toupper(*str) - ASCII_ZERO_POINT] += 1;
		}
		++str;
	}
}

static size_t *SumAllThreadsResultsIMP(size_t *res_histo, size_t *thread_histo)
{
	size_t i = 0;
	
	for (i = 0; i < NUM_OF_LETTERS_IN_ENGLISH; ++i)
	{
		res_histo[i] += thread_histo[i];
	}
	
	return res_histo;
}

size_t *CountingLetters(size_t num_of_threads)
{
	size_t i = 0;
	size_t fsize = 0;
	char **ptr_to_words = NULL;
	size_t counter = 0;
	pthread_t *threads = NULL;
	size_t amount_per_thread = 0;
	thread_param_t *params = NULL;
	char *word_list_for_count = CopyDictToStringIMP(&fsize);
	char *word_list_for_assign_pointers = CopyDictToStringIMP(&fsize);
	size_t *res_histogram = NULL;
	void *thread_histo = NULL;
	size_t current_state = 0;
	
	assert(num_of_threads);
	
	counter = CountWordsIMP(word_list_for_count) - WORDS_TO_DISREGARD;

	ptr_to_words = (char **)malloc(MULTIPLYER * counter * sizeof(void *));
	if (NULL == ptr_to_words)
	{
		return 0;
	}
	
	InsertWordsToArrayIMP(ptr_to_words, 
					   word_list_for_assign_pointers, 
					   counter);
	
	threads = (pthread_t *)malloc(num_of_threads * sizeof(pthread_t));
	if (NULL == threads)
	{
		return 0;
	}
	
	amount_per_thread = MULTIPLYER * counter / num_of_threads;
	params = (thread_param_t *)malloc(num_of_threads * sizeof(thread_param_t)); 
	
	for (i = 0; i < num_of_threads; ++i)
	{
		current_state = i * amount_per_thread;
		params[i].str = ptr_to_words + current_state;
		params[i].size = amount_per_thread;
		if (i == num_of_threads - 1)
		{
			params[i].size = MULTIPLYER * counter - current_state;
		}
		pthread_create(threads + i, NULL, CountingLettersInEachThreadIMP, 
										  params + i);						  
	}
	
	res_histogram = (size_t *)calloc(NUM_OF_LETTERS_IN_ENGLISH, 
									 sizeof(size_t));
	if (NULL == res_histogram)
	{
		return 0;
	}
	
	for (i = 0; i < num_of_threads; ++i)
	{
		pthread_join(threads[i], &thread_histo);
		SumAllThreadsResultsIMP(res_histogram, (size_t *)thread_histo);
		free(thread_histo);
	}
	
	if (-1 == munmap(word_list_for_count, fsize))
	{
		perror ("munmap failed\n");
    	printf( "Value of errno: %d\n", errno);
	}
	if (-1 == munmap(word_list_for_assign_pointers, fsize))
	{
		perror ("munmap failed\n");
    	printf( "Value of errno: %d\n", errno);
	}
	
	free(ptr_to_words);
	free(threads);
	free(params);
	
	return res_histogram;
}
