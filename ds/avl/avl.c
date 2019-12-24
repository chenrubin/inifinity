/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		avl							  *
*		7/11/2019					  *
*									  *
************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

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
static size_t RecAvlSizeIMP(avl_node_t *node);
static int RecForEachIMP(avl_node_t *node, action_func func, void *param);
static int IsLeafIMP(avl_node_t *node);

avl_t *AVLCreate(comparison_func func)
{
	avl_t *new_avl = (avl_t *)malloc(sizeof(avl_t));
	if (NULL == new_avl)
	{
		return NULL;
	}
	
	assert(func);
	
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
	assert(tree);
	
	tree -> root = RecInsertIMP(tree -> root, data, tree -> comparison_func);
	if (NULL == tree -> root)
	{
		return 1;
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

size_t AVLSize(const avl_t *tree)
{
	return RecAvlSizeIMP(tree -> root);
}

static size_t RecAvlSizeIMP(avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	else
	{
		return (RecAvlSizeIMP(node -> children[0]) + 
				RecAvlSizeIMP(node -> children[1]) + 1);
	}
}

int AVLForEach(avl_t *tree, action_func func, void *param)
{
	assert(tree);
	
	if (NULL == tree -> root)
	{
		return 0;
	}
	
	return RecForEachIMP(tree -> root, func, param);
}

static int RecForEachIMP(avl_node_t *node, action_func func, void *param)
{
	int status = 0;
	
	if (NULL == node)
	{
		return 0;
	}
	
	status |= RecForEachIMP(node -> children[0], func, param);
	
	if (func(node -> data, param) == 1)
	{
		return 1;
	}
	
	status |= RecForEachIMP(node -> children[1], func, param);
	
	return status;
}

static int IsLeafIMP(avl_node_t *node)
{
	return (NULL == node -> children[0] &&
			NULL == node -> children[1]);
}
















