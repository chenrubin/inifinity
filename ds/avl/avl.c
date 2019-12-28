/************************************
*		Author: ChenR				  *
*		Reviewer: Sharon			  *
*		avl							  *
*		26/12/2019					  *
*									  *
************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

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
	comparison_func_t comparison_func;
	avl_node_t *root;
};

/* recursively destroys tree */
static void RecDestroyIMP(avl_node_t *node);

/* Creates node */
static avl_node_t *CreateNodeIMP(void *data);

/* Updates node height */
static void UpdateHeightIMP(avl_node_t *node);

/* gets direction if where to go when serching for a number or looking where 
   to insert it */
static int GetDirectionIMP(void *new_data, void *src_data, 
					comparison_func_t comparison_func);
/* balance tree after insert or remove */					
static avl_node_t *BalanceTreeIMP(avl_node_t *node);

/* recursively insert node to tree */
static avl_node_t *RecInsertIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func_t comparison_func);
						 
/* recursively getting size */						 
static size_t RecAvlSizeIMP(avl_node_t *node);

/* recursively issiung function for each node in tree */
static int RecForEachIMP(avl_node_t *node, action_func_t func, void *param);

/* return 1 if leaf and 0 if not */
static int IsLeafIMP(avl_node_t *node);

/* recursively 1 finds element in tree */
static void *RecFindIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func_t comparison_func);
						 
/* return max between two numbers */						 
static size_t MyMaxIMP(size_t x, size_t y);

/* recursively remvove node from tree */
static avl_node_t *RecRemoveIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func_t comparison_func);
						 
/* 0 means only left child, 1 meand only right child and 2 means both */
static num_of_children_t WhichChildrenExistIMP(avl_node_t *node);

/* finding the left most child of a node and connecting it to node's
   right child  */
static void TraverseUntilLeftmostLeafAndConnectIMP(avl_node_t *node, 
												avl_node_t *node_to_connect_to);

/* Get's tree'd root height */												
static int GetHeightIMP(avl_node_t *node);

/* returns right child's hight minus left child's height */
static int GetDiffFactorIMP(avl_node_t *node);

/* one rotation to left or to right  */
static avl_node_t *SimpleRotationIMP(avl_node_t *node, int direction);

/* double rotation left and right ot right and left */
static void DoubleRotationIMP(avl_node_t *node, int direction);

/* If node is a parent it will be removed using this function */
static avl_node_t *RemoveIsParentIMP(avl_node_t *node, int which_children);

/* removes a leaf */
static avl_node_t *RemoveLeafIMP(avl_node_t *node);

/* checks whether or not parent have same diffFactor sign or not */
static int IsUnbalancedToSameDirectionIMP(avl_node_t *parent, 
										  avl_node_t *child);

avl_t *AVLCreate(comparison_func_t func)
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
	avl_node_t *node_runner = NULL;
	
	assert(tree);
	
	node_runner = tree -> root;
	RecDestroyIMP(node_runner);
	
	free(tree);
}

int AVLInsert(avl_t *tree, void *data)
{
	avl_node_t *temp_node = NULL;
	
	assert(tree);
	assert(data);
	
	temp_node = tree -> root;
	
	temp_node = RecInsertIMP(tree -> root, data, tree -> comparison_func);
	if (NULL == temp_node)
	{
		return 1;
	}
	
	tree -> root = temp_node;
	
	return 0;	
}

void AVLRemove(avl_t *tree, const void *data)
{
	assert(tree);
	assert(data);
	
	tree -> root = RecRemoveIMP(tree -> root, (void *)data, 
								tree -> comparison_func);
	tree -> root = BalanceTreeIMP(tree -> root);							
}

static avl_node_t *RecRemoveIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func_t comparison_func)
{
	int direction = GetDirectionIMP(data, node -> data, comparison_func);
	int which_children = 0;

	if (NULL == node)
	{
		return NULL;
	}
	else if (0 != comparison_func(data, node -> data))
	{
		node -> children[direction] = RecRemoveIMP(node -> children[direction], 
												   data, 
												   comparison_func);
		node = BalanceTreeIMP(node);										   
		UpdateHeightIMP(node);
		
		return node;					
	}
	else
	{
		which_children = WhichChildrenExistIMP(node);
		
		if (IsLeafIMP(node))
		{
			return (RemoveLeafIMP(node));
		}
		else
		{
			return (RemoveIsParentIMP(node, which_children));
		}
	}
}

static avl_node_t *RemoveLeafIMP(avl_node_t *node)
{
	free(node);
	node = NULL;
	
	return node;
}

static avl_node_t *RemoveIsParentIMP(avl_node_t *node, int which_children)
{
	avl_node_t *temp_node = NULL;
	
	if (BOTH != which_children)
	{
		temp_node = node -> children[which_children];
		free(node);
		node = NULL;
		UpdateHeightIMP(temp_node);
		
		return temp_node;
	}
	else
	{
		temp_node = node -> children[RIGHT];
		TraverseUntilLeftmostLeafAndConnectIMP
							(node -> children[RIGHT], node -> children[LEFT]);
		free(node);
		node = NULL;
			
		return temp_node;
	}
}

static avl_node_t *BalanceTreeIMP(avl_node_t *node)
{
	int direction = 0;
	avl_node_t *pivot = NULL;
	int diff = GetDiffFactorIMP(node);
	
	if (diff > 1)
	{
		direction = RIGHT;
	}
	else if (diff < -1)
	{
		direction  = LEFT;
	}
	else
	{
		return node;
	}
	
	if (IsUnbalancedToSameDirectionIMP(node, node -> children[direction]))
	{
		pivot = SimpleRotationIMP(node, direction);
	}
	else
	{
		DoubleRotationIMP(node, direction);
		pivot = SimpleRotationIMP(node, direction);
	}

	return pivot;
}

static int IsUnbalancedToSameDirectionIMP(avl_node_t *parent, 
										  avl_node_t *child)
{
	if (((GetDiffFactorIMP(parent) < 0) && (GetDiffFactorIMP(child) < 0)) ||
		((GetDiffFactorIMP(parent) > 0) && (GetDiffFactorIMP(child) > 0)))
	{	
		return 1;
	}
	
	return 0;
}

size_t AVLSize(const avl_t *tree)
{
	assert(tree);
	
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
		return (RecAvlSizeIMP(node -> children[LEFT]) + 
				RecAvlSizeIMP(node -> children[RIGHT]) + 1);
	}
}

int AVLForEach(avl_t *tree, action_func_t func, void *param)
{
	assert(tree);
	
	return RecForEachIMP(tree -> root, func, param);
}

size_t AVLGetHeight(const avl_t *tree)
{
	assert(tree);
	
	return (tree -> root -> height);
}

int AVLIsEmpty(const avl_t *tree)
{
	assert(tree);
	
	return (NULL == (tree -> root));
}

static int RecForEachIMP(avl_node_t *node, action_func_t func, void *param)
{
	int status = 0;

	assert(func);
	
	if (NULL == node)
	{
		return 0;
	}
	
	status |= RecForEachIMP(node -> children[LEFT], func, param);
	
	if (func(node -> data, param) == 1)
	{
		return 1;
	}
	
	status |= RecForEachIMP(node -> children[RIGHT], func, param);
	
	return status;
}

void *AVLFind(const avl_t *tree, const void *data)
{
	assert(tree);
	assert(data);
	
	return (RecFindIMP(tree -> root, (void *)data, tree -> comparison_func));
}

static int IsLeafIMP(avl_node_t *node)
{
	assert(node);
	
	return (NULL == node -> children[LEFT] &&
			NULL == node -> children[RIGHT]);
}

static void *RecFindIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func_t comparison_func)
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

static size_t MyMaxIMP(size_t x, size_t y)
{
	if (x > y)
	{
		return x;
	}
	
	return y;
}

static num_of_children_t WhichChildrenExistIMP(avl_node_t *node)
{
	if ((NULL != node -> children[LEFT]) && (NULL != node -> children[RIGHT]))
	{
		return BOTH;
	}
	else if (NULL != node -> children[LEFT])
	{
		return LEFT;
	}
	
	return RIGHT;
}

static void RecDestroyIMP(avl_node_t *node)
{
	if (NULL != node)
	{
		RecDestroyIMP(node -> children[LEFT]);
		RecDestroyIMP(node -> children[RIGHT]);
		free(node);
		node = NULL;
	}
}

static avl_node_t *RecInsertIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func_t comparison_func)
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
	
	node = BalanceTreeIMP(node);
	UpdateHeightIMP(node);
	
	return node;
}

static void TraverseUntilLeftmostLeafAndConnectIMP(avl_node_t *node, 
									 avl_node_t *node_to_connect_to)
{
	if (NULL == (node -> children[LEFT]))
	{
		node -> children[LEFT] = node_to_connect_to;
		UpdateHeightIMP(node);
	}
	else
	{
		TraverseUntilLeftmostLeafAndConnectIMP(node -> children[LEFT], 
										   	   node_to_connect_to);
		UpdateHeightIMP(node);
	}	
}

static int GetHeightIMP(avl_node_t *node)
{
	if (NULL == node)
	{
		return -1;
	}
	
	return node -> height;
}

static avl_node_t *CreateNodeIMP(void *data)
{
	avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node -> children[LEFT] = NULL;
	new_node -> children[RIGHT] = NULL;
	new_node -> data = data;
	new_node -> height = 0;
	
	return new_node;
}

static void UpdateHeightIMP(avl_node_t *node)
{
	int which_children = 0;
	
	if (IsLeafIMP(node))
	{
		node -> height = 0;
	}
	else
	{
		which_children = WhichChildrenExistIMP(node);
		
		if (BOTH != which_children)
		{
			node -> height = (node -> children[which_children] -> height) + 1;
		}
		else
		{
			node -> height = MyMaxIMP(node -> children[LEFT] -> height, 
								  	  node -> children[RIGHT] -> height) + 1;
		}	
	}	  
}

static int GetDiffFactorIMP(avl_node_t *node)
{
	int factor = 0;
	
	if ((NULL == node) || IsLeafIMP(node))
	{
		return 0;
	}
	else
	{
		factor = GetHeightIMP(node -> children[RIGHT]) - 
				 GetHeightIMP(node -> children[LEFT]);
	}

	return factor;
}

static avl_node_t *SimpleRotationIMP(avl_node_t *node, int direction)
{
	avl_node_t *pivot = node -> children[direction];
	
	node -> children[direction] = pivot -> children[!direction];
	pivot -> children[!direction] = node;
	
	UpdateHeightIMP(node);
	
	return pivot;									  
}

static void DoubleRotationIMP(avl_node_t *node, int direction)
{
	avl_node_t *pivot = node -> children[direction];
	avl_node_t *node_to_connect = node -> children[direction] -> 
										  children[!direction];
										  
	node -> children[direction] = node_to_connect;
	pivot -> children[!direction] = node_to_connect -> children[direction];
	node_to_connect -> children[direction] = pivot;
	
	UpdateHeightIMP(node_to_connect);
	UpdateHeightIMP(pivot);
}

static int GetDirectionIMP(void *new_data, void *src_data, 
					comparison_func_t comparison_func)
{
	if (-1 == comparison_func(new_data, src_data))
	{
		return 0;
	}
	
	return 1;
}
