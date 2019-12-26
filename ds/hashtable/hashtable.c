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

#include "hashtable.h"
#include "../dllist/dllist.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define BUCKET(index) (*((hash_table -> table) + index))
struct hash
{
	hash_func_t hash_func;
	is_match_t match_func;
	size_t num_of_buckets;
	dl_list_t **table;
};

struct dl_list
{
	dll_node_t *begin;
	dll_node_t *end;
};

/* free num of Dllist pointers starting from table */
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
											 sizeof(dl_list_t));
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
		}
	}
	
	new_htbl -> num_of_buckets = num_of_buckets; 
	new_htbl -> hash_func = hash_func;
	new_htbl -> match_func = match_func;
	
	return new_htbl;
}

void HashDestroy(hash_table_t *hash_table)
{
	FreeDllistsIMP(hash_table -> table, hash_table -> num_of_buckets);
	free(hash_table -> table);
	free(hash_table);
}

int HashInsert(hash_table_t *hash_table, void *data)
{
	size_t index = hash_table -> hash_func(data);
	
	return (!(DLListEnd(BUCKET(index)) != 
			  DLListPushBack(BUCKET(index), data)));
}

int HashForEach(hash_table_t *hash_table,
                action_func_t action_func,
                void *action_param)
{                
	size_t i = 0;
	
	for (i = 0; i < hash_table -> num_of_buckets; ++i)
	{
		if (1 == DLListForEach(DLListBegin(BUCKET(i)), 
							   DLListEnd(*((hash_table -> table) + i)),
				 			   action_param, action_func))
		{
			return 1;
		}		 			   
	}
	
	return 0;
}

void HashRemove(hash_table_t *hash_table, const void *data)
{
	size_t index = hash_table -> hash_func(data);
	
	dll_iter_t iter = DLListFind(DLListBegin(BUCKET(index)), 
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
	
	for (i = 0; i < hash_table -> num_of_buckets; ++i)
	{
		is_empty &= DLListIsEmpty(BUCKET(i));
	}
	
	return is_empty;
}

void *HashFind(const hash_table_t *hash_table, const void *data)
{
	size_t i = 0;
	
	for (i = 0; i < hash_table -> num_of_buckets; ++i)
	{
		dll_iter_t iter = DLListFind(DLListBegin(BUCKET(i)), 
						 DLListEnd(BUCKET(i)), 
					  	 data, hash_table -> match_func);
		
		if (DLListEnd(BUCKET(i)) != iter)
		{
			return(DLListGetData(iter));
		}
	}
	
	return NULL;
}

double HashGetLoadFactor(const hash_table_t *hash_table)
{
	double size = ((double)HashSize(hash_table));
	double num_of_buckets = (double)(hash_table -> num_of_buckets);
	
	return size / num_of_buckets;
}

double HashGetStandardDeviation(const hash_table_t *hash_table)
{
	size_t i = 0;
	double sum = 0;
	double diff = 0;
	double mean = HashGetLoadFactor(hash_table);
	size_t num_of_buckets = hash_table -> num_of_buckets;
	
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

