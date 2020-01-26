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
#include <strings.h> /* strcmp */

#define MULTIPLYER (8)
#define LETTERS_IN_ENGLISH (26)
#define ASCII_ZERO_POINT ('A')
#define WORDS_TO_DISREGARD 0
#define MAX_BUFF 30

typedef int (*compare)(const void *, const void *);

typedef struct wrap
{
	char **str;
	size_t size;
	size_t element_size;
	compare func;
}thread_param_t;

typedef struct wrap1
{
	char **arr;
	size_t num_of_threads; 
	size_t size_per_thread; 
	size_t num_of_words;
	compare func;
}merge_param_t;

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
static void *SortingChunkInEachThreadIMP(void *param);

void MergeTwoChucksIMP(char **arr1, 
					   size_t size1, 
					   char **arr2, 
					   size_t size2, 
					   compare func);

/* count the letters in each string *//*
static void CountingLettersInEachWordIMP(char *str, size_t *histogram);
*/
/* sum all threads' histograms to one *//*
static size_t *SumAllThreadsResultsIMP(size_t *res_histo, 
									   size_t num_of_threads,
									   pthread_t *threads);
*/
/* This is the main function for counting letters from dictionary */
FILE *Sort(size_t num_of_threads, compare func);


void qsort(void *base, size_t nitems, size_t size, 
		   int (*compar)(const void *, const void*));

/* function that manages all threads that count each segment */
static int ThreadsSortingWords(pthread_t **threads, 
								  size_t num_of_words, 
								  size_t *num_of_threads, 
								  char **strings_ptr,
								  thread_param_t **params,
								  compare func,
								  size_t *amount_per_thread);

/* update thread params */
static void UpdateThreadParamsIMP(thread_param_t *param, 
								  char **arr_of_strings, 
								  size_t amount_per_thread, 
								  size_t num_of_words,
								  size_t num_of_current_thread,
								  size_t num_of_threads,
								  compare func);

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
	int fd = open("resDict.txt", O_RDONLY);
	
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

static void *SortingChunkInEachThreadIMP(void *param)
{
	char **str = NULL;
	size_t size = 0;
	size_t element_size = 0;
	compare func = NULL;
	size_t i = 0;
	
	assert(param);
	str = ((thread_param_t *)param) -> str;
	size = ((thread_param_t *)param) -> size;
	element_size = ((thread_param_t *)param) -> element_size;
	func = ((thread_param_t *)param) -> func;
	printf("str = %p\n", str);
	for (i = 0; i < 24; ++i)
	{
		printf("word = %s\n", *(str + i));
	}
	printf("\n\n");
	printf("size = %ld, element_size = %ld\n", size, element_size);
	qsort(str, size, element_size, func);
	for (i = 0; i < 24; ++i)
	{
		printf("word = %s\n", *(str + i));
	}
	printf("\n");
/*
	int func (const void *str1, const void *str2) 
	{
	   return (strcmp(str1, str2));
	}
*/	return 0;
}
/*
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
*/
/*
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
*/
static void UpdateThreadParamsIMP(thread_param_t *param, 
								  char **arr_of_strings, 
								  size_t amount_per_thread, 
								  size_t num_of_words,
								  size_t num_of_current_thread,
								  size_t num_of_threads,
								  compare func)
{	
	assert(param);
	assert(arr_of_strings);
	
	param -> str = arr_of_strings + num_of_current_thread * amount_per_thread;
	param -> element_size = sizeof(void *);
	param -> func = func;
	param -> size = amount_per_thread;
	
	if (num_of_current_thread == num_of_threads - 1)
	{
		param -> size = num_of_words - 
						(num_of_current_thread * amount_per_thread);
	}
}

void *MergeAllChunks(char **string_ptr, 
					 size_t num_of_words, 
					 size_t num_of_threads, 
					 size_t size_per_thread, 
					 compare func)
{
	size_t i = 0;
	size_t size_first_chunk = 0;
	size_t size_second_chunk = 0;
	size_t num_of_current_thread = 0;
/*	size_t size_per_thread = 0;
	size_t num_of_threads = 0;
	size_t num_of_words = 0;
	char **string_arr = NULL;
	compare func = NULL;
*/	
/*	assert(param);*/
	
/*	size_per_thread = ((merge_param_t *)param) -> size_per_thread;
	num_of_threads = ((merge_param_t *)param) -> num_of_threads;
	num_of_words = ((merge_param_t *)param) -> num_of_words;
	string_arr = ((merge_param_t *)param) -> arr;
	func = ((merge_param_t *)param) -> func;
*/	
	for (i = 0; i < num_of_threads - 1; ++i)
	{
		size_first_chunk += size_per_thread;
		size_second_chunk = size_per_thread;
		num_of_current_thread = i + 1;
		
		if ((num_of_threads - 1) == i)
		{
			size_second_chunk = num_of_words - 
								(num_of_current_thread * size_per_thread);
		}
		
		MergeTwoChucksIMP(string_ptr, 
					   	  size_first_chunk, 
					   	  string_ptr + size_first_chunk, 
					   	  size_second_chunk, 
					   	  func);	
	}
}				   

void MergeTwoChucksIMP(char **arr1, 
					   size_t size1, 
					   char **arr2, 
					   size_t size2, 
					   compare func)
{
	size_t i = 0;
	size_t j = 0;
	void **ptr_arr = (void *)malloc(size1 + size2);
	
	while ((i < size1) && (j < size2))
	{
		if (func((arr1 + i),(arr2 + j)) < 0)
		{
			ptr_arr[i + j] = arr1 + i;
			++i;
		}
		else
		{
			ptr_arr[i + j] = arr2 + j;
			++j;
		}
	}
	
	while (i < size1)
	{
		ptr_arr[i + j] = arr1 + i;
		++i;
	}
	while (j < size2)
	{
		ptr_arr[i + j] = arr2 + j;
		++j;
	}
	
	memcpy(arr1, ptr_arr, size1 + size2);
}

static int ThreadsSortingWords(pthread_t **threads, 
								  size_t num_of_words, 
								  size_t *num_of_threads, 
								  char **strings_ptr,
								  thread_param_t **params,
								  compare func,
								  size_t *amount_per_thread)
{
	size_t i = 0;
	
	*threads = (pthread_t *)malloc(*num_of_threads * sizeof(pthread_t));
	if (NULL == *threads)
	{	
		return 1;
	}
	
	num_of_words *= MULTIPLYER;
	*amount_per_thread = num_of_words / (*num_of_threads);
	*params = (thread_param_t *)malloc(*num_of_threads * sizeof(thread_param_t)); 
	if (NULL == params)
	{
		return 1;
	}
	
	for (i = 0; i < (*num_of_threads); ++i)
	{
		UpdateThreadParamsIMP(*params + i, 
							  strings_ptr, 
							  *amount_per_thread,
							  num_of_words,
							  i,
							  *num_of_threads,
							  func);
		printf("before thread create\n");
		pthread_create(*threads + i, 
					   NULL, 
					   SortingChunkInEachThreadIMP, 
					   *params + i);					  
	}
	
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!before join!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n");
	for (i = 0; i < (*num_of_threads); ++i)
	{
		pthread_join(*(*threads + i), NULL);
	}
	printf("after join %d\n");
	
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

FILE *Sort(size_t num_of_threads, compare func)
{
	size_t file_size = 0;
	char **strings_ptr = NULL;
	size_t num_of_words = 0;
	pthread_t *threads = NULL;
	thread_param_t *params = NULL;
	char *word_list_count = NULL;
	char *word_list_pointers = NULL;
	FILE *new_fp = NULL;
	size_t i = 0;
	size_t amount_per_thread = 0;
	
	assert(num_of_threads);
	
	strings_ptr = InsertWordsToArrayIMP(&num_of_words, 
										&file_size, 
										word_list_count, 
										word_list_pointers);
	if (NULL == strings_ptr)
	{
		return NULL;
	}
	
	if (1 == ThreadsSortingWords(&threads,
								 num_of_words, 
								 &num_of_threads, 
								 strings_ptr,
								 &params,
								 func,
								 &amount_per_thread))
	{
		free(threads);
		free(strings_ptr);
		
		return NULL;
	}
	printf("before MergeAllChunks\n");
	MergeAllChunks(strings_ptr, num_of_words, num_of_threads, 
				   amount_per_thread, func);
		printf("after MergeAllChunks\n");
	new_fp = fopen("sorted_words.txt", "w+");
	if (NULL == new_fp)
	{
		free(threads);
		free(strings_ptr);
		
		return NULL;
	}
	printf("\n\nFinal result\n\n");
	for (i = 0; i < num_of_words * MULTIPLYER; ++i)
	{
		printf("word = %s\n", *(char **)(strings_ptr + i));
	}
	
/*	while (fgets(*(char **)(strings_ptr + i), MAX_BUFF, new_fp) != NULL) 
	{
    	fputs(line,targetFile);
    }*/        
/*	for (i = 0; i < num_of_words * MULTIPLYER; ++i)
	{
		fgets(*(char **)(strings_ptr + i), MAX_BUFF, new_fp);
	}	
*/	
/*	
	if (0 > fprintf(new_fp, (char *)strings_ptr))
	{
		free(threads);
		free(strings_ptr);
		fclose(new_fp);
	}*/
/*	
	FreeMapping(word_list_count, word_list_pointers, file_size);
	FreeAllocations(strings_ptr, threads, params);
*/	
	return new_fp;
}
