/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		dhcp						  *
*		7/11/2019					  *
*									  *
************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "trie.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define CHILD(i) (node -> children[i])
#define FLAG(node) (node -> is_subtree_full)
#define LSB 1
#define BITS_IN_IP 32

typedef enum children_number
{
	LEFT = 0,
	RIGHT = 1,
	BOTH = 2,
	NON = 3
} child_num_t;

static unsigned int GetBitIMP(unsigned int ip, size_t index_of_bit);
static child_num_t NumOfChildrenIMP(node_t *node);
static alloc_status_t RecTrieInsertIMP(node_t *node, 
									   size_t level, 
									   unsigned int requested_ip);
static void UpdateFlagIMP(node_t *node);
static node_t *CreateNodeIMP();
static unsigned int ConvertLevelToBinaryIMP(size_t level);
static alloc_status_t CreateChildIfNecessaryIMP(node_t *node, 
												unsigned int child_num);
static alloc_status_t RecInsertAnyAddressIMP(node_t *node, size_t level);												
static node_t *RecTrieDestroyIMP(node_t *node);

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
	unsigned int result_ip = 0;
	unsigned int binary_level = ConvertLevelToBinaryIMP(level);
	unsigned int bc_binary_ip = binary_level;
	unsigned int subnet_binary_ip = 0;
	unsigned int gateway_binary_ip = binary_level - 1;
	
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
 
	TrieInsert(new_trie, bc_binary_ip, &result_ip);
	TrieInsert(new_trie, subnet_binary_ip, &result_ip);
	TrieInsert(new_trie, gateway_binary_ip, &result_ip);						  
	
	return new_trie;
}
/*SUCCESS_ALLOCATED_REQUSTED = 0,
	SUCCESS_ALLOCATED_AVAILABLE = 1,
	MALLOC_FAIL = 2,
	TRIE_FULL = 3*/
alloc_status_t TrieInsert(trie_t *trie, 
						  unsigned int requested_ip, 
						  unsigned int *result)
{
	int status = SUCCESS_ALLOCATED_REQUSTED;
	
	assert(trie);
	
	if (1 == FLAG(trie -> node))
	{
		return TRIE_FULL;
	}
	
	status = RecTrieInsertIMP(trie -> node, trie -> level, requested_ip);

	if ((SUCCESS_ALLOCATED_REQUSTED == status) || (MALLOC_FAIL == status))
	{
		*result = requested_ip;
		
		return status;
	}
	
	if (SUCCESS_ALLOCATED_AVAILABLE == status)
	{
		return (RecInsertAnyAddressIMP(trie -> node, trie -> level));
	}

	return status;
}

void TrieDestroy(trie_t *trie)
{
	assert(trie);
	
	trie -> node = RecTrieDestroyIMP(trie -> node);
	free(trie);
	trie = NULL;
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
		node -> children[0] = RecTrieDestroyIMP(node -> children[0]);
		node -> children[1] = RecTrieDestroyIMP(node -> children[1]);
		free(node);
		node = NULL;
	}
	
	return node;
}

static alloc_status_t RecInsertAnyAddressIMP(node_t *node, size_t level)
{
	int status = SUCCESS_ALLOCATED_AVAILABLE;
	
	if (0 == level)
	{
		FLAG(node) = 1;
		
		return status;
	}
	
	CreateChildIfNecessaryIMP(node, 0);
	if (0 == FLAG(CHILD(0)))
	{
		status = RecInsertAnyAddressIMP(CHILD(0), level - 1);
		UpdateFlagIMP(node);
	}
	else
	{
		CreateChildIfNecessaryIMP(node, 1);	
		status = RecInsertAnyAddressIMP(CHILD(1), level - 1);
		UpdateFlagIMP(node);
	}
	
	return status;
}

static alloc_status_t CreateChildIfNecessaryIMP(node_t *node, 
												unsigned int child_num)
{
	if (NULL == CHILD(child_num))
	{
		CHILD(child_num) = CreateNodeIMP();
		if (NULL == (CHILD(child_num)))
		{
			return MALLOC_FAIL;
		}
	}
	
	return SUCCESS_ALLOCATED_AVAILABLE;
}

static alloc_status_t RecTrieInsertIMP(node_t *node, 
									   size_t level, 
									   unsigned int requested_ip)
{
	int status = SUCCESS_ALLOCATED_AVAILABLE;
	unsigned int child_bit = 0;
	
	if (0 == level)
	{
		if (1 == (node -> is_subtree_full))
		{
			status = SUCCESS_ALLOCATED_AVAILABLE;
		}
		else
		{
			node -> is_subtree_full = 1;
			status = SUCCESS_ALLOCATED_REQUSTED;
		}
	}
	else
	{
		child_bit = GetBitIMP(requested_ip, level - 1);
		status = CreateChildIfNecessaryIMP(node, child_bit);
		if (MALLOC_FAIL == status)
		{
			return status;
		}

		status = RecTrieInsertIMP((CHILD(child_bit)), level - 1, requested_ip);
		UpdateFlagIMP(node);	
	}
	
	return status;
}

static void UpdateFlagIMP(node_t *node)
{
	if ((BOTH == NumOfChildrenIMP(node)) && 
		(1 == FLAG(CHILD(0))) && 
		(1 == FLAG(CHILD(1))))
	{
		FLAG(node) = 1;
	}
}

static node_t *CreateNodeIMP()
{
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	if (NULL == new_node)
	{
		return NULL;
	}

	new_node -> children[0] = NULL;
	new_node -> children[1] = NULL;
	new_node -> is_subtree_full = 0;
	
	return new_node;
}

static child_num_t NumOfChildrenIMP(node_t *node)
{
	if ((NULL != node -> children[0]) && 
		(NULL == node -> children[1]))
	{
		return LEFT;
	}
	if ((NULL == node -> children[0]) && 
		(NULL != node -> children[1]))
	{
		return RIGHT;
	}
	if ((NULL != node -> children[0]) && 
		(NULL != node -> children[1]))
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

static unsigned int ConvertLevelToBinaryIMP(size_t level)
{
	unsigned int level_bin = (0xFFFFFFFF << (BITS_IN_IP - level)) >> 
									  		(BITS_IN_IP - level);
									  
	return level_bin;								  
}
