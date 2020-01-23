#include <sys/mman.h> /* mmap, munmap */
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> /* strtok */
#include <stdlib.h> /* malloc */
#include <ctype.h> /* toupper */
#include <pthread.h>

#define MULTIPLYER (8)
#define NUM_OF_LETTERS_IN_ENGLISH (26)
#define ASCII_ZERO_POINT ('A')
#define WORDS_TO_DISREGARD 18

typedef struct wrap
{
	char **str;
	size_t size;
}thread_param_t;

static char *CopyDictToString();
static size_t GetFileSizeIMP(int file_descriptor);
static size_t CountWords(char *str);
static void InsertWordsToArray(char **arr, char *word_list, size_t size);
static void *CountingLettersInEachThread(void *param);
static void *CountingLettersInEachThread(void *param);
static void CountingLettersInEachWord(char *str, size_t *histogram);
static size_t *SumAllThreadsResults(size_t *res_histo, size_t *thread_histo);
size_t *CountingLetters(size_t num_of_threads);

static char *CopyDictToString()
{
	char *word_list = NULL;
	int fd = open("/usr/share/dict/american-english", O_RDONLY);
	size_t fsize = GetFileSizeIMP(fd);
	
    word_list = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	return word_list;
}

static size_t GetFileSizeIMP(int file_descriptor)
{
	size_t size = lseek(file_descriptor, 0, SEEK_END);
	
	lseek(file_descriptor, 0, SEEK_SET);
	
	return size;
}

static size_t CountWords(char *str)
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

static void InsertWordsToArray(char **arr, char *word_list, size_t size)
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

static void *CountingLettersInEachThread(void *param)
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
		CountingLettersInEachWord(*str + i, quantity_histogram);
	}

	return quantity_histogram;
}

static void CountingLettersInEachWord(char *str, size_t *histogram)
{
	while ('\0' != *str)
	{
		if (0 != isalpha(*str))
		{
			histogram[toupper(*str) - ASCII_ZERO_POINT] += 1;
		}
		++str;
/*		printf("counter = %ld\n", histogram[toupper(*str) - ASCII_ZERO_POINT]);*/
	}
}

static size_t *SumAllThreadsResults(size_t *res_histo, size_t *thread_histo)
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
	char **ptr_to_words = NULL;
	size_t counter = 0;
	pthread_t *threads = NULL;
	size_t amount_per_thread = 0;
	thread_param_t *params = NULL;
	char *word_list_for_count = CopyDictToString();
	char *word_list_for_assign_pointers = CopyDictToString();
	size_t *res_histogram = NULL;
	void *thread_histo = NULL;
	size_t current_state = 0;
	
	counter = CountWords(word_list_for_count) - WORDS_TO_DISREGARD;

	ptr_to_words = (char **)malloc(MULTIPLYER * counter * sizeof(void *));
	if (NULL == ptr_to_words)
	{
		return 0;
	}
	
	InsertWordsToArray(ptr_to_words, 
					   word_list_for_assign_pointers, 
					   counter);
	
	threads = (pthread_t *)malloc(num_of_threads * sizeof(pthread_t));
	if (NULL == threads)
	{
		return 0;
	}
	
	amount_per_thread = MULTIPLYER * counter / num_of_threads;
	params = (thread_param_t *)malloc(sizeof(thread_param_t)); 
	
	for (i = 0; i < num_of_threads; ++i)
	{
		current_state = i * amount_per_thread;
		params[i].str = ptr_to_words + (i * amount_per_thread);
		params[i].size = amount_per_thread;
		if (i == num_of_threads - 1)
		{
			params[i].size = MULTIPLYER * counter - current_state;
		}
		pthread_create(threads + i, NULL, CountingLettersInEachThread, 
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
		SumAllThreadsResults(res_histogram, (size_t *)thread_histo);
		free(thread_histo);
	}
	
	return res_histogram;
}
