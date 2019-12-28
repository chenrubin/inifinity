/************************************
*		Author: ChenR				  *
*		Reviewer: EyalR				  *
*		hashtable					  *
*		7/11/2019					  *
*									  *
************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */ 	
#include <sys/mman.h> /* mmap, munmap */
#include <unistd.h>
#include<sys/stat.h>
#include <ctype.h> /* toupper */
#include <strings.h> /* strcasecmp */
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#include "hashtable.h"
#include "../dllist/dllist.h"

#define BUCKET(index) (*((hash_table -> table) + index))
#define CHAR_SIZE (sizeof(char))

hash_table_t *InsertToHashTable(hash_table_t *hash_table, char **str);
int SpellChecker(const hash_table_t *hash_table, char *str);
int MyActionFunction(void *data, void *action_param);
int MyIsMatchFunction(const void *current_data, const void *data_to_compare);
size_t MyHashFunction(const void *key);
int MyActionFunction(void *data, void *action_param);

int main()
{	
	size_t num_of_buckets = 26;
/*	char str[] = "a becaus";*/
	char str[256] = {0};
	int status = 0;
	int action_param = 0;
	char *file_as_string = NULL;
	hash_table_t *new_htbl = HashCreate(num_of_buckets,
                         				MyHashFunction,
                         				MyIsMatchFunction);
	                    				
    new_htbl = InsertToHashTable(new_htbl, &file_as_string); 
/*    HashForEach(new_htbl, MyActionFunction, &action_param);*/
	printf("Please enter string\n");
	scanf("%[^\n]s", str);
	printf("Spell checking your string\n");
    status = SpellChecker(new_htbl, str);
	
	free(file_as_string);
    HashDestroy(new_htbl);
    
    return status;
}

hash_table_t *InsertToHashTable(hash_table_t *hash_table, char **str)
{
	int status = 0;
	size_t size = 0;
	char *token = NULL;
	size_t result = 0;
	
	
	FILE *fp = fopen("/usr/share/dict/american-english","r");
	if (NULL == fp)
	{
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    *str = (char *)calloc(size + 1, CHAR_SIZE);
    if (NULL == str)
    {
    	return NULL;
    }
    
    result = fread(*str, CHAR_SIZE, size, fp);
    if (size != result)
    {
    	return NULL;
    }
    fclose(fp);	
	
	token = strtok(/*file_as_string*/*str, ",. !\n");
	while (token != NULL)
	{
		status |= HashInsert(hash_table, token);
		token = strtok(NULL, ",. !\n");
	}
	
	return hash_table;	
}

int SpellChecker(const hash_table_t *hash_table, char *str)
{
	char *token;
	int status = 0;
	
	token = strtok(str, ",. !");
	
	while (token != NULL)
	{
		printf("%s, ", token);
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

size_t MyHashFunction(const void *key)
{
	size_t index = (size_t)toupper(*(char *)key) - 'A';
	
	return index;
}

int MyIsMatchFunction(const void *current_data, const void *data_to_compare)
{
	return (0 == strcasecmp((char *)current_data, (char *)data_to_compare));
}

int MyActionFunction(void *data, void *action_param)
{
	UNUSED(action_param);
	
	printf("%s\n", (char *)data);
	
	return 0;
} 
