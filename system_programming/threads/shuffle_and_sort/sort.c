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

#include "../../../ds/sorting/sorting.h"

#define MULTIPLYER (1)
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
static char *CopyDictToStringIMP(size_t *file_size, char *src_file);

/* get file size for mmap */
static size_t GetFileSizeIMP(int file_descriptor);

/* count words in dictionary */
static size_t CountWordsIMP(char *str);

/* insert strings from buffer to array */
static char **InsertWordsToArrayIMP(size_t *num_of_words, 
									size_t *file_size,
									char *word_list_for_count,
									char *word_list_for_assign_pointers,
									char *src_file);

/* thread routine that counts letters in a given chunk in an array of strings */
static void *SortingChunkInEachThreadIMP(void *param);

/* merge two chicks using one thread */
void MergeTwoChucksIMP(char **arr1, 
					   size_t size1, 
					   char **arr2, 
					   size_t size2, 
					   compare func);

/* merge all chunks using several threads */					   
void MergeAllChunks(char **string_ptr, 
					 size_t num_of_words, 
					 size_t num_of_threads, 
					 size_t size_per_thread, 
					 compare func);

/* compare funtion for qsort */
int func (const void *str1, const void *str2) ;

/* This is the main function for counting letters from dictionary */
FILE *Sort(size_t num_of_threads, char *func_name, compare func);


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

/* Free all allocations */
static void FreeAllocations(char **strings_ptr, 
							pthread_t *threads, 
							thread_param_t *params,
							FILE *fp);
							
static int IsBeforeIMP(const void *new_data, 
			  	 	   const void *src_data, 
			   		   void *param);							
			   		   
/* copy buffer to file with length of num_of_words */
void CopyBufferToFile(size_t num_of_words, char **strings_ptr, FILE *fp);

int main(int argc, char *argv[])
{
	int num_of_threads = atoi(argv[1]);
	
	Sort(num_of_threads, argv[2], func);
	
	(void)argc;
	
	return 0;
}
  
static char *CopyDictToStringIMP(size_t *file_size, char *src_file)
{
	char *word_list = NULL;
	int fd = open(src_file, O_RDONLY);
	
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
									char *word_list_for_assign_pointers,
									char *src_file)
{
	int i = 0;
	char *token = NULL;
	char **strings_ptr = NULL;
	
	word_list_for_count = CopyDictToStringIMP(file_size, src_file);
	word_list_for_assign_pointers = CopyDictToStringIMP(file_size, src_file);
	
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
	int heap_param = 0;
	
	assert(param);
	
	str = ((thread_param_t *)param) -> str;
	size = ((thread_param_t *)param) -> size;
	element_size = ((thread_param_t *)param) -> element_size;
	func = ((thread_param_t *)param) -> func;
/*	
	HeapSort(str, size, element_size, IsBeforeIMP, &heap_param);
*/	

	qsort(str, size, element_size, func);
	
	return 0;
}

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

void MergeAllChunks(char **string_ptr, 
					 size_t num_of_words, 
					 size_t num_of_threads, 
					 size_t size_per_thread, 
					 compare func)
{
	size_t i = 0;
	size_t size_first_chunk = 0;
	size_t size_second_chunk = 0;
	
	for (i = 0; i < num_of_threads; ++i)
	{
		size_first_chunk += size_per_thread;
		size_second_chunk = size_per_thread;

		if ((num_of_threads - 1) == i)
		{
			size_second_chunk = num_of_words - size_first_chunk;
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
	char **ptr_arr = (char **)malloc((sizeof(void *)) * (size1 + size2));

	while ((i < size1) && (j < size2))
	{
		if (func((arr1 + i),(arr2 + j)) < 0)
		{
			ptr_arr[i + j] = arr1[i];
			++i;
		}
		else
		{
			ptr_arr[i + j] = arr2[j];
			++j;
		}
	}
	
	while (i < size1)
	{
		ptr_arr[i + j] = arr1[i];
		++i;
	}
	while (j < size2)
	{
		ptr_arr[i + j] = arr2[j];
		++j;
	}
	
	memcpy(arr1, ptr_arr, sizeof(void *) * (size1 + size2));
	
	free(ptr_arr);
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

		pthread_create(*threads + i, 
					   NULL, 
					   SortingChunkInEachThreadIMP, 
					   *params + i);					  
	}
	
	for (i = 0; i < (*num_of_threads); ++i)
	{
		pthread_join(*(*threads + i), NULL);
	}
	
	return 0;
}

static void FreeAllocations(char **strings_ptr, 
							pthread_t *threads, 
							thread_param_t *params,
							FILE *fp)
{
	free(strings_ptr);
	free(threads);
	free(params);
	fclose(fp);
}

FILE *Sort(size_t num_of_threads, char *file_name, compare func)
{
	size_t file_size = 0;
	char **strings_ptr = NULL;
	size_t num_of_words = 0;
	pthread_t *threads = NULL;
	thread_param_t *params = NULL;
	char *word_list_count = NULL;
	char *word_list_pointers = NULL;
	FILE *new_fp = NULL;
	size_t amount_per_thread = 0;
	int i = 0;
	
	assert(num_of_threads);
	
	strings_ptr = InsertWordsToArrayIMP(&num_of_words, 
										&file_size, 
										word_list_count, 
										word_list_pointers,
										file_name);
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
	
	for (i = 0; i< num_of_words; ++i)
	{
		printf("%s, ", *(strings_ptr + i));
	}
	
	MergeAllChunks(strings_ptr, num_of_words * MULTIPLYER, num_of_threads, 
				   amount_per_thread, func);

	new_fp = fopen("sorted_words.txt", "w+");
	if (NULL == new_fp)
	{
		free(threads);
		free(strings_ptr);
		
		return NULL;
	}
	
	CopyBufferToFile(num_of_words, strings_ptr, new_fp);
	
	FreeAllocations(strings_ptr, threads, params, new_fp);
		
	return new_fp;
}

void CopyBufferToFile(size_t num_of_words, char **strings_ptr, FILE *fp)
{
	size_t i = 0;
	
	assert(num_of_words);
	assert(strings_ptr);
	assert(fp);
	
	for (i = 0; i < num_of_words * MULTIPLYER; ++i)
	{
		fputs(*(char **)(strings_ptr + i), fp);
		fputs("\n", fp);
	}
}

static int IsBeforeIMP(const void *new_data, 
			  	 	   const void *src_data, 
			   		   void *param)
{
	(void)param;
	
	if ((0 <= strcmp(*(char **)new_data, *(char **)src_data)))
	{
		return 1;
	}
	
	return 0;
}
			   				  
int func(const void *str1, const void *str2) 
{
/*	printf("str1 = %s, str2 = %s\n", *(char **)str1, *(char **)str2);*/
   return (strcmp(*(char **)str1, *(char **)str2));
}
