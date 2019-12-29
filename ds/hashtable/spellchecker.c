/************************************
*		Author: ChenR				  *
*		Reviewer: EyalR				  *
*		hashtable					  *
*		29/12/2019					  *
*									  *
************************************/
#include <stdio.h>
#include <string.h> /* strtok */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */ 	
#include <sys/mman.h> /* mmap, munmap */
#include <unistd.h>
#include<sys/stat.h> /* open */
#include <fcntl.h> /* O_RDONLY */
#include <ctype.h> /* toupper */
#include <strings.h> /* strcasecmp */
#include "../../chen/MyUtils.h" /* UNUSED */ 

#include "hashtable.h"
#include "../dllist/dllist.h"

#define BUCKET(index) (*((hash_table -> table) + index))
#define CHAR_SIZE (sizeof(char))

/* insert to hash table */
hash_table_t *InsertToHashTable(hash_table_t *hash_table);

/* Spell checker */
int SpellChecker(const hash_table_t *hash_table, char *str);

/* Action function  */
int MyActionFunction(void *data, void *action_param);

/* is match function */
int MyIsMatchFunctionIMP(const void *current_data, const void *data_to_compare);

/* hash function */
size_t MyHashFunctionIMP(const void *key);

/* getting file size */
static size_t GetFileSizeIMP(int file_descriptor);

int main()
{	
	size_t num_of_buckets = 26;
	char str[256] = {0};
	int status = 0;
	hash_table_t *new_htbl = HashCreate(num_of_buckets,
                         				MyHashFunctionIMP,
                         				MyIsMatchFunctionIMP);
	                    				
    new_htbl = InsertToHashTable(new_htbl); 
	printf("Please enter string\n");
	scanf("%[^\n]s", str);
	printf("Spell checking your string\n");
    status = SpellChecker(new_htbl, str);

    HashDestroy(new_htbl);
    
    return status;
}

hash_table_t *InsertToHashTable(hash_table_t *hash_table)
{
	int status = 0;
	char *token = NULL;
	char *str_temp = NULL;
	int fd = open("/usr/share/dict/american-english", O_RDONLY);
	size_t fsize = GetFileSizeIMP(fd);
		
    str_temp = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	
	token = strtok(str_temp, ",. !\n");
	while (token != NULL)
	{
		status |= HashInsert(hash_table, token);
		token = strtok(NULL, ",. !\n");
	}

	return hash_table;
}

static size_t GetFileSizeIMP(int file_descriptor)
{
	size_t size = lseek(file_descriptor, 0, SEEK_END);
	lseek(file_descriptor, 0, SEEK_SET);
	
	return size;
}

int SpellChecker(const hash_table_t *hash_table, char *str)
{
	char *token;
	int status = 0;
	
	assert(hash_table);
	
	token = strtok(str, ",. !");
	while (token != NULL)
	{
		if (NULL == HashFind(hash_table, token))
		{
			printf("%s, ", token);
			status = 1;
		}
		token = strtok(NULL, ",. !");
	}
	printf("\n");
	
	return status;
}

size_t MyHashFunctionIMP(const void *key)
{
	size_t index = (size_t)toupper(*(char *)key) - 'A';
	if (index > 26)
	{
		index = 23;
	}
	
	return index;
}

int MyIsMatchFunctionIMP(const void *current_data, const void *data_to_compare)
{
	return (0 == strcasecmp((char *)current_data, (char *)data_to_compare));
}

int MyActionFunction(void *data, void *action_param)
{
	UNUSED(action_param);
	
	printf("%s\n", (char *)data);
	
	return 0;
} 
