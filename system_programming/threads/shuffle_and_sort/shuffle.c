/****************************************
*	Author : ChenR						*
*	Reviewer: Tsisana					*
*	Counting letters in dictionary		*
*	24/1/2020 							*
****************************************/

#include <sys/mman.h> /* mmap */
#include <stdio.h>	/* printing errno */
#include <unistd.h> /* lseek */
#include <fcntl.h> /* O_RDONLY */
#include <string.h> /* strtok */
#include <stdlib.h> /* malloc */
#include <ctype.h> /* toupper */
#include <assert.h> /* assert */
#include <errno.h> /* errno */
#include <strings.h> /* strcmp */
#include <time.h> /* time */

#define MULTIPLYER (1)
#define LETTERS_IN_ENGLISH (26)
#define ASCII_ZERO_POINT ('A')
#define WORDS_TO_DISREGARD 0
#define MAX_BUFF 30
#define SHUFFLE_ITER ((num_of_words) / (2))

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

/* This is the main function for counting letters from dictionary */
FILE *MyShuffle(char *file_name);

/* swap between two pointers */
void SwapPointers(char **ptr1, char **ptr2);

/* Free all allocations */
static void FreeAllocations(char **strings_ptr, FILE *new_fp);													  

int main(int argc, char *argv[])
{
	MyShuffle(argv[1]);
	
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

static void FreeAllocations(char **strings_ptr, FILE *new_fp)
{
	free(strings_ptr);
	fclose(new_fp);
}

void Shuffle_array(char **strings_ptr, size_t num_of_words)
{
	size_t index1 = 0;
	size_t index2 = 0;
	size_t i = 0;
	
	assert(strings_ptr);
	assert(num_of_words);
	
	srand(time(NULL));
	
	for (i = 0; i < SHUFFLE_ITER * 10; ++i)
	{
		index1 = rand() % num_of_words;
		index2 = rand() % num_of_words;
		SwapPointers(strings_ptr + index1, strings_ptr + index2);
	}	
}

void SwapPointers(char **ptr1, char **ptr2)
{
	char *temp = *ptr1;
	
	*ptr1 = *ptr2;
	*ptr2 = temp;
}

FILE *MyShuffle(char *file_name)
{
	size_t file_size = 0;
	char **strings_ptr = NULL;
	size_t num_of_words = 0;
	char *word_list_count = NULL;
	char *word_list_pointers = NULL;
	FILE *new_fp = NULL;
	size_t i = 0;
	
	assert(file_name);
	
	strings_ptr = InsertWordsToArrayIMP(&num_of_words, 
										&file_size, 
										word_list_count, 
										word_list_pointers,
										file_name);
	if (NULL == strings_ptr)
	{
		return NULL;
	}
	
	Shuffle_array(strings_ptr, num_of_words);
		
	new_fp = fopen("MyShuffle.txt", "w+");
	if (NULL == new_fp)
	{
		free(strings_ptr);
		
		return NULL;
	}
	
	for (i = 0; i < num_of_words * MULTIPLYER; ++i)
	{
		fputs(*(char **)(strings_ptr + i), new_fp);
		fputs("\n", new_fp);
	}
	
	FreeAllocations(strings_ptr, new_fp);
	
	return new_fp;
}
