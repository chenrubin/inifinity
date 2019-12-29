/************************************
*		Author: ChenR				  *
*		Reviewer: EyalR				  *
*		hashtable					  *
*		7/11/2019					  *
*									  *
************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <math.h> /* pow , sqrt */
#include <stdio.h> /* malloc */

#include "hashtable.h"
#include "../dllist/dllist.h"

#define BUCKET(index) (*((hash_table -> table) + index))

static size_t GetHashIndexIMP(const hash_table_t *hash_table, const void *data);

struct hash
{
	hash_func_t hash_func;
	is_match_t match_func;
	size_t num_of_buckets;
	dl_list_t **table;
};

enum STATUS
{
	SUCCESS = 0,
	FAIL = 1
};

static void FreeDllistsIMP(dl_list_t **table, size_t num);

hash_table_t *HashCreate(size_t num_of_buckets,
                         hash_func_t hash_func,
                         is_match_t match_func)
{
	size_t i = 0;
	hash_table_t *new_htbl = NULL;
	
	assert(hash_func);
	assert(match_func);
	assert(num_of_buckets);
	
	new_htbl = (hash_table_t *)malloc(sizeof(hash_table_t));
	if (NULL == new_htbl)
	{
		return NULL;
	}
	new_htbl -> table = (dl_list_t **)malloc(num_of_buckets * 
											 sizeof(dl_list_t**));
	if (NULL == new_htbl -> table)
	{
		free(new_htbl);
		new_htbl = NULL;
		
		return NULL;
	}										
	
	for (i = 0; i < num_of_buckets; ++i)
	{
		*((new_htbl -> table) + i) = DLListCreate();
		if (NULL == *((new_htbl -> table) + i))
		{
			FreeDllistsIMP(new_htbl -> table, i);
			free(new_htbl -> table);
			free(new_htbl);
			
			return NULL;
		}
	}
	
	new_htbl -> num_of_buckets = num_of_buckets; 
	new_htbl -> hash_func = hash_func;
	new_htbl -> match_func = match_func;
	
	return new_htbl;
}

void HashDestroy(hash_table_t *hash_table)
{
	assert(hash_table);
	
	FreeDllistsIMP(hash_table -> table, hash_table -> num_of_buckets);
	free(hash_table -> table);
	free(hash_table);
}

int HashInsert(hash_table_t *hash_table, void *data)
{
	size_t index = 0;
	
	assert(hash_table);
	assert(data);
	 
	index = GetHashIndexIMP(hash_table, data);
	
	return (!(DLListEnd(BUCKET(index)) != 
			  DLListPushBack(BUCKET(index), data)));
}

int HashForEach(hash_table_t *hash_table,
                action_func_t action_func,
                void *action_param)
{                
	size_t i = 0;
	
	assert(hash_table);
	assert(action_func);
	
	for (i = 0; i < hash_table -> num_of_buckets; ++i)
	{
		if (FAIL == DLListForEach(DLListBegin(BUCKET(i)), 
							   DLListEnd(BUCKET(i)),
				 			   action_param, action_func))
		{
			return FAIL;
		}		 			   
	}
	
	return SUCCESS;
}

void HashRemove(hash_table_t *hash_table, const void *data)
{
	size_t index = 0;
	dll_iter_t iter = NULL;
	
	assert(hash_table);
	assert(data);
	
	index = GetHashIndexIMP(hash_table, data);
	
	iter = DLListFind(DLListBegin(BUCKET(index)), 
					  DLListEnd(BUCKET(index)), 
					  data, hash_table -> match_func);
	
	if (DLListEnd(BUCKET(index)) != iter)
	{
		DLListRemove(iter);
	}				  	 
}

size_t HashSize(const hash_table_t *hash_table)
{
	size_t i = 0;
	size_t size = 0;
	
	assert(hash_table);
	
	for (i = 0; i < hash_table -> num_of_buckets; ++i)
	{
		size += DLListSize(BUCKET(i));
	}
	
	return size;
}

int HashIsEmpty(const hash_table_t *hash_table)
{
	size_t i = 0;
	int is_empty = 1;
	
	assert(hash_table);

	for (i = 0; i < hash_table -> num_of_buckets; ++i)
	{
		is_empty &= DLListIsEmpty(BUCKET(i));
	}
	
	return is_empty;
}

void *HashFind(const hash_table_t *hash_table, const void *data)
{
	size_t index = 0;
	dll_iter_t iter = NULL;
	
	assert(hash_table);
	assert(data);
	
	index = GetHashIndexIMP(hash_table, data);
	
	iter = DLListFind(DLListBegin(BUCKET(index)), 
					  DLListEnd(BUCKET(index)), 
				  	  data, hash_table -> match_func);
	
	if (DLListEnd(BUCKET(index)) != iter)
	{
		return(DLListGetData(iter));
	}

	return NULL;
}

double HashGetLoadFactor(const hash_table_t *hash_table)
{
	double size = 0;
	double num_of_buckets = 0;
	
	assert(hash_table);
	
	size = ((double)HashSize(hash_table));
	num_of_buckets = (double)(hash_table -> num_of_buckets);
	
	return size / num_of_buckets;
}

double HashGetStandardDeviation(const hash_table_t *hash_table)
{
	size_t i = 0;
	double sum = 0;
	double diff = 0;
	double mean = 0;
	size_t num_of_buckets = 0;
	
	assert(hash_table);
	
	mean = HashGetLoadFactor(hash_table);
	num_of_buckets = hash_table -> num_of_buckets;
	
	for (i = 0; i < (hash_table -> num_of_buckets); ++i)
	{
		diff = DLListSize(BUCKET(i)) - mean;
		sum += pow(diff, 2);
	}
	
	return (sqrt(sum / num_of_buckets));
}

static void FreeDllistsIMP(dl_list_t **table, size_t num)
{
	size_t i = 0;
	
	for (i = 0; i < num; ++i)
	{
		DLListDestroy(*(table + i));
	}
}

static size_t GetHashIndexIMP(const hash_table_t *hash_table, const void *data)
{
	return (hash_table -> hash_func(data));
}
