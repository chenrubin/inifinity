	/************************************
*		Author: ChenR				  *
*		Reviewer: EyalF				  *
*		trie						  *
*		6/1/2020					  *
*									  *
************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <math.h> /* pow */

#include "trie.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define CHILD(i) (node -> children[i])
#define FLAG(node) (node -> is_subtree_full)
#define LSB 1
#define BITS_IN_IP 32
#define FULL 1
#define EMPTY 0

typedef enum children_number
{
	LEFT = 0,
	RIGHT = 1,
	BOTH = 2,
	NON = 3
} child_num_t;

static unsigned int GetBitIMP(unsigned int ip, size_t index_of_bit);
static child_num_t NumOfChildrenIMP(node_t *node);
static trie_alloc_status_t RecTrieInsertIMP(node_t *node, 
									   		size_t level, 
									   		unsigned int requested_ip,
									   		unsigned int *result);
static void UpdateFlagIMP(node_t *node);
static node_t *CreateNodeIMP();
static trie_alloc_status_t CreateChildIfNecessaryIMP(node_t *node, 
													 unsigned int child_num);
static trie_alloc_status_t RecInsertAnyAddressIMP(node_t *node, 
											 	  size_t level, 
											 	  unsigned int *result);
static node_t *RecTrieDestroyIMP(node_t *node);
static node_t *RecTrieDeallocateIMP(node_t *node, 
						  			unsigned int ip, 
						  			size_t level, 
						  			int *status);
static size_t RecTrieCountAllocIMP(node_t *node, size_t level);

struct trie 
{
	node_t *node;
	size_t level;
};

struct node 
{
	node_t *children[2];
	int is_subtree_full;
};

trie_t *TrieCreate(size_t level)
{
	trie_t *new_trie = NULL;
	
	new_trie = (trie_t *)malloc(sizeof(trie_t));
	if (NULL == new_trie)
	{
		return NULL;
	}
	new_trie -> node = CreateNodeIMP();
	if (NULL == new_trie -> node)
	{
		free(new_trie);
		new_trie = NULL;
		
		return NULL;
	}
	
	new_trie -> level = level;
	
	return new_trie;
}

trie_alloc_status_t TrieInsert(trie_t *trie, 
						  	   unsigned int requested_ip, 
						  	   unsigned int *result)
{	
	assert(trie);
	
	return (RecTrieInsertIMP(trie -> node, 
							trie -> level, 
							requested_ip, 
							result));
}

void TrieDestroy(trie_t *trie)
{
	assert(trie);
	
	trie -> node = RecTrieDestroyIMP(trie -> node);
	free(trie);
	trie = NULL;
}

trie_free_status_t TrieDeallocate(trie_t *trie, unsigned int ip)
{
	int status = 0;

	trie -> node = RecTrieDeallocateIMP(trie -> node, 
									 	ip, 
									 	trie -> level, 
									 	&status);
	UpdateFlagIMP(trie -> node);								 	
									 
	return status;								 
}

int TrieIsFull(trie_t *trie)
{
	assert(trie);
	
	if (NULL != trie -> node)
	{
		return (FLAG(trie -> node));
	}
	
	return 0;
}

size_t TrieCountAlloc(trie_t *trie)
{
	assert(trie);
	
	return RecTrieCountAllocIMP(trie -> node, trie -> level);
}

static size_t RecTrieCountAllocIMP(node_t *node, size_t level)
{	
	if (NULL == node)
	{
		return 0;
	}
	if (0 == level)
	{
		return FLAG(node);
	}
	else if (1 == FLAG(node))
	{
		return (pow(2, level));
	}
	
	return (RecTrieCountAllocIMP(node -> children[LEFT], level - 1) +
		   (RecTrieCountAllocIMP(node -> children[RIGHT], level - 1)));
}

static node_t *RecTrieDeallocateIMP(node_t *node, 
						  	 		unsigned int ip, 
						  	 		size_t level, 
						  	 		int *status)
{
	unsigned int child_bit = 0;

	if (NULL == node)
	{
		*status = T_IP_NOT_FOUND;
	}
	else if (0 == level)
	{
		switch (FLAG(node))
		{
			case 1:
			FLAG(node) = 0;
			*status = T_SUCCESS;
			break;
			
			default:
			*status = T_DOUBLE_FREE;
			break;
		}
	}
	else
	{
		child_bit = GetBitIMP(ip, level - 1);
		CHILD(child_bit) = RecTrieDeallocateIMP(CHILD(child_bit), 
												ip, 
												level - 1, 
												status);
		UpdateFlagIMP(node);										
	}
	
	return node;
}

static node_t *RecTrieDestroyIMP(node_t *node)
{
	if (NULL == node)
	{
		return NULL;
	}
	if (NON == NumOfChildrenIMP(node))
	{
		free(node);
		node = NULL;
	}
	else
	{
		node -> children[LEFT] = RecTrieDestroyIMP(node -> children[LEFT]);
		node -> children[RIGHT] = RecTrieDestroyIMP(node -> children[RIGHT]);
		free(node);
		node = NULL;
	}
	
	return node;
}

static trie_alloc_status_t CreateChildIfNecessaryIMP(node_t *node, 
													 unsigned int child_num)
{
	if (NULL == CHILD(child_num))
	{
		CHILD(child_num) = CreateNodeIMP();
		if (NULL == (CHILD(child_num)))
		{
			return T_MALLOC_FAIL;
		}
	}
	
	return T_SUCCESS_ALLOCATED_REQUESTED;
}

static trie_alloc_status_t RecInsertAnyAddressIMP(node_t *node, 
											 	  size_t level, 
											 	  unsigned int *result)
{
	int status = T_SUCCESS_ALLOCATED_REQUESTED;
	
	if (0 == level)
	{
		FLAG(node) = FULL;
		
		return status;
	}
	
	CreateChildIfNecessaryIMP(node, 0);
	if (EMPTY == FLAG(CHILD(0)))
	{
		*result &= ~(LSB << (level - 1));
		status = RecInsertAnyAddressIMP(CHILD(0), level - 1, result);
	}
	else
	{
		*result |= LSB << (level - 1);
		CreateChildIfNecessaryIMP(node, 1);	
		status = RecInsertAnyAddressIMP(CHILD(1), level - 1, result);
	}
	UpdateFlagIMP(node);
	
	return status;
}
	
static trie_alloc_status_t RecTrieInsertIMP(node_t *node, 
									   size_t level, 
									   unsigned int requested_ip,
									   unsigned int *result)
{
	trie_alloc_status_t status = T_REQUESTED_IP_OCCUPIED;
	unsigned int child_bit = 0;
	
	if (0 != requested_ip)
	{
		if (0 == level)
		{
			if (FULL == (node -> is_subtree_full))
			{
				status = T_REQUESTED_IP_OCCUPIED;
			}
			else
			{
				node -> is_subtree_full = FULL;
				*result = requested_ip;
				status = T_SUCCESS_ALLOCATED_REQUESTED;
			}
		}
		else
		{
			child_bit = GetBitIMP(requested_ip, level - 1);
			status = CreateChildIfNecessaryIMP(node, child_bit);
			if (T_MALLOC_FAIL == status)
			{
				return status;
			}

			status = RecTrieInsertIMP((CHILD(child_bit)), 
									  level - 1, 
									  requested_ip, result);
			UpdateFlagIMP(node);	
		}
	}
	else
	{
		status = RecInsertAnyAddressIMP(node, level, result);
		if (status == T_SUCCESS_ALLOCATED_REQUESTED)
		{
			status = T_REQUESTED_IP_OCCUPIED;
		}
	}
	
	return status;
}

static void UpdateFlagIMP(node_t *node)
{
	if ((BOTH == NumOfChildrenIMP(node)) && 
		(FULL == FLAG(CHILD(LEFT))) && 
		(FULL == FLAG(CHILD(RIGHT))))
	{
		FLAG(node) = FULL;
	}
	else
	{
		FLAG(node) = EMPTY;
	}
}

static node_t *CreateNodeIMP()
{
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	if (NULL == new_node)
	{
		return NULL;
	}

	new_node -> children[LEFT] = NULL;	
	new_node -> children[RIGHT] = NULL;
	new_node -> is_subtree_full = EMPTY;
	
	return new_node;
}

static child_num_t NumOfChildrenIMP(node_t *node)
{
	if ((NULL != node -> children[LEFT]) && 
		(NULL == node -> children[RIGHT]))
	{
		return LEFT;
	}
	if ((NULL == node -> children[LEFT]) && 
		(NULL != node -> children[RIGHT]))
	{
		return RIGHT;
	}
	if ((NULL != node -> children[LEFT]) && 
		(NULL != node -> children[RIGHT]))
	{
		return BOTH;
	}
	
	return NON;
}

static unsigned int GetBitIMP(unsigned int ip, size_t index_of_bit)
{
	unsigned int bit_res = LSB << index_of_bit;
	
	bit_res &= ip;
	bit_res >>= index_of_bit;
	
	return bit_res;
}
