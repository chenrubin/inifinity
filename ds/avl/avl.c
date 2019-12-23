/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		avl							  *
*		7/11/2019					  *
*									  *
************************************/
#include <stdlib.h>

#include "avl.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

typedef struct avl_node avl_node_t;

struct avl_node
{
	avl_node_t *children[2];
	void *data;
	size_t height; 
};

struct avl
{
	comparison_func comparison_func;
	avl_node_t *root;
};

static void RecDestroyIMP(avl_node_t *node);
static avl_node_t *CreateNodeIMP(void *data);
int GetDirectionIMP(int result);
static void UpdateHeightIMP();
static avl_node_t *BalanceTreeIMP();
avl_node_t *RecInsertIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func);

avl_t *AVLCreate(comparison_func func)
{
	avl_t *new_avl = (avl_t *)malloc(sizeof(avl_t));
	if (NULL == new_avl)
	{
		return NULL;
	}
	
	new_avl -> comparison_func = func;
	new_avl -> root = NULL;
	
	return new_avl;
}

void AVLDestroy(avl_t *tree)
{
	avl_node_t *node_runner = tree -> root;
	
	RecDestroyIMP(node_runner);
	
	free(tree);
}

static void RecDestroyIMP(avl_node_t *node)
{
	if (NULL != node)
	{
		RecDestroyIMP(node -> children[0]);
		RecDestroyIMP(node -> children[1]);
		free(node);
		node = NULL;
	}
}

int AVLInsert(avl_t *tree, void *data)
{
	avl_node_t *node = tree -> root;
	
	node = RecInsertIMP(node, data, tree -> comparison_func);
	if (NULL == node)
	{
		return 1;
	}
	
	if (NULL == tree -> root)
	{
		(tree -> root) = node;
	}
	
	return 0;	
}

avl_node_t *RecInsertIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func)
{
	int cmp_res = 0;
	int direction = 0;
	
	if (NULL == node)
	{
		node = CreateNodeIMP(data);
		if (NULL == node)
		{
			return NULL;
		}
	}
	else
	{
		cmp_res = comparison_func(data, node -> data);
		direction = GetDirectionIMP(cmp_res);
		node -> children[direction] = RecInsertIMP(node -> children[direction], 
												   data, 
												   comparison_func);
	}
	
	BalanceTreeIMP();
	UpdateHeightIMP();
	
	return node;
}

static avl_node_t *CreateNodeIMP(void *data)
{
	avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node -> children[0] = NULL;
	new_node -> children[1] = NULL;
	new_node -> data = data;
	new_node -> height = 0;
	
	return new_node;
}

static void UpdateHeightIMP()
{
	
}

static avl_node_t *BalanceTreeIMP()
{
	return NULL;
}

int GetDirectionIMP(int result)
{
	if (-1 == result)
	{
		return 0;
	}
	
	return 1;
}




