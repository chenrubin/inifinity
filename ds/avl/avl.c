/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		avl							  *
*		7/11/2019					  *
*									  *
************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <stdio.h> /* assert */

#include "avl.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

typedef struct avl_node avl_node_t;

typedef enum childrens_number
{
	LEFT = 0,
	RIGHT = 1,
	BOTH = 2
}num_of_children_t;

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
static size_t UpdateHeightIMP(avl_node_t *node);
int GetDirectionIMP(void *new_data, void *src_data, 
					comparison_func comparison_func);
static avl_node_t *BalanceTreeIMP();
avl_node_t *RecInsertIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func);
static size_t RecAvlSizeIMP(avl_node_t *node);
static int RecForEachIMP(avl_node_t *node, action_func func, void *param);
static int IsLeafIMP(avl_node_t *node);
void *AVLFind(const avl_t *tree, const void *data);
static void *RecFindIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func);
size_t MyMax(size_t x, size_t y);
avl_node_t *RecFindNodeToRemove(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func);
static num_of_children_t WhichChildrenExistIMP(avl_node_t *node);	
static avl_node_t *TraverseUntilLefmostLeafIMP(avl_node_t *node);					 

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
		direction = GetDirectionIMP(data, node -> data, comparison_func);
		node -> children[direction] = RecInsertIMP(node -> children[direction], 
												   data, 
												   comparison_func);										   								   
	}
	
	BalanceTreeIMP();
	node -> height = UpdateHeightIMP(node);
	
	return node;
}

void AVLRemove(avl_t *tree, const void *data)
{
	int direction = 1;
	size_t which_children = 0;
	avl_node_t *node_to_remove = NULL;
	avl_node_t *leftmost_node = NULL;
	
	if (*(int *)tree -> root ->data == *(int *)data)
	{
		node_to_remove = tree -> root;
	}
	else
	{
		avl_node_t *parent_of_node_to_remove = RecFindNodeToRemove(tree -> root, 
						 				 (void *)data, tree -> comparison_func);
	}
	
	if (IsLeafIMP(parent_of_node_to_remove))
	{
		return;
	}
	direction = GetDirectionIMP((void *)data, parent_of_node_to_remove -> data,
								tree -> comparison_func);
	node_to_remove = parent_of_node_to_remove -> children[direction];							
		
	if (IsLeafIMP(node_to_remove))
	{
		free(node_to_remove);
		node_to_remove = NULL;	
	}
	else 
	{
		which_children = WhichChildrenExistIMP(node_to_remove);
							 								  		
		if (BOTH == which_children)
		{
			leftmost_node = TraverseUntilLefmostLeafIMP
							(node_to_remove -> children[RIGHT]);
			leftmost_node -> children[LEFT] = node_to_remove -> children[LEFT];
		}
		
		parent_of_node_to_remove -> children[direction] = 
		node_to_remove -> children[RIGHT];					
		free(node_to_remove);
		node_to_remove = NULL;	
	}			 				   					 				   					 		
}

static avl_node_t *TraverseUntilLefmostLeafIMP(avl_node_t *node)
{
	if (NULL == (node -> children[LEFT]))
	{
		return node;
	}
	
	return (TraverseUntilLefmostLeafIMP(node -> children[LEFT]));
}

avl_node_t *RecFindNodeToRemove(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func)
{
	int direction = GetDirectionIMP(data, node -> data, comparison_func);

	if (NULL == node)
	{
		return NULL;
	}
	else if (0 == comparison_func(data, node -> children[direction] -> data))
	{
		return node;
	}
	else
	{
		return (RecFindNodeToRemove(node -> children[direction], 
							data, 
							comparison_func));										   								   
	}
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

static size_t UpdateHeightIMP(avl_node_t *node)
{
	size_t temp = 0;
	if (NULL == node)
	{
		return 0;
	}
	else if (IsLeafIMP(node))
	{
		return 0;
	}
	else
	{
		temp = (1 + MyMax(UpdateHeightIMP(node -> children[0]),
					 	  UpdateHeightIMP(node -> children[1])));		 	 
		return temp;
	}
}

static avl_node_t *BalanceTreeIMP()
{
	return NULL;
}

int GetDirectionIMP(void *new_data, void *src_data, 
					comparison_func comparison_func)
{
	if (-1 == comparison_func(new_data, src_data))
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

void *AVLFind(const avl_t *tree, const void *data)
{
	return (RecFindIMP(tree -> root, (void *)data, tree -> comparison_func));
}

static void *RecFindIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func)
{
	int direction = 0;
	
	if (NULL == node)
	{
		return NULL;
	}
	else if (0 == comparison_func(data, node -> data))
	{
		return node -> data;
	}
	else
	{
		direction = GetDirectionIMP(data, node -> data, comparison_func);
	
		return (RecFindIMP(node -> children[direction], data, comparison_func));
	}
}

size_t MyMax(size_t x, size_t y)
{
	if (x > y)
	{
		return x;
	}
	
	return y;
}

/* under the assumption that node is not a leaf*/
static num_of_children_t WhichChildrenExistIMP(avl_node_t *node)
{
	if ((NULL != node -> children[0]) && (NULL != node -> children[1])) /* both children exist */
	{
		return BOTH;
	}
	else if (NULL != node -> children[0]) /* only left child exist */
	{
		return LEFT;
	}
	
	return RIGHT; /*only right chuld exist*/
}
















