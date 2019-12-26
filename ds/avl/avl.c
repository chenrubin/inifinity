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

/* recursively destroys tree */
static void RecDestroyIMP(avl_node_t *node);

/* Creates node */
static avl_node_t *CreateNodeIMP(void *data);

/* Updates node height */
static void UpdateHeightIMP(avl_node_t *node);

/* gets direction if where to go when serching for a number or looking where 
   to insert it */
int GetDirectionIMP(void *new_data, void *src_data, 
					comparison_func comparison_func);
/* balance tree after insert or remove */					
static avl_node_t *BalanceTreeIMP(avl_node_t *node);

/* recursively insert node to tree */
avl_node_t *RecInsertIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func);
						 
/* recursively getting size */						 
static size_t RecAvlSizeIMP(avl_node_t *node);

/* recursively issiung function for each node in tree */
static int RecForEachIMP(avl_node_t *node, action_func func, void *param);

/* return 1 if leaf and 0 if not */
static int IsLeafIMP(avl_node_t *node);

/* recursively 1 finds element in tree */
static void *RecFindIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func);
						 
/* return max between two numbers */						 
size_t MyMaxIMP(size_t x, size_t y);

/* recursively remvove node from tree */
static avl_node_t *RecRemoveIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func);
						 
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
static avl_node_t *SimpleRotation(avl_node_t *node, int direction);

/* double rotation left and right ot right and left */
static void DoubleRotation(avl_node_t *node, int direction);

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
	
	node = BalanceTreeIMP(node);
	UpdateHeightIMP(node);
	
	return node;
}

void AVLRemove(avl_t *tree, const void *data)
{
	tree -> root = RecRemoveIMP(tree -> root, (void *)data, 
								tree -> comparison_func);
	tree -> root = BalanceTreeIMP(tree -> root);							
}


static void TraverseUntilLeftmostLeafAndConnectIMP(avl_node_t *node, 
											avl_node_t *node_to_connect_to)
{
	if (NULL == (node -> children[LEFT]))
	{
		node -> children[LEFT] = node_to_connect_to;
		UpdateHeightIMP(node);
		
		return;
	}
	
	TraverseUntilLeftmostLeafAndConnectIMP(node -> children[LEFT], 
										   node_to_connect_to);
	UpdateHeightIMP(node);
	
	return;
}

static avl_node_t *RecRemoveIMP(avl_node_t *node, 
						 void *data, 
						 comparison_func comparison_func)
{
	int direction = GetDirectionIMP(data, node -> data, comparison_func);
	int which_children = 0;
	avl_node_t *temp_node = NULL;

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
			temp_node = node;
			free(node);
			node = NULL;
			
			return node;	
		}
		else if (BOTH != which_children)
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
			node -> height = MyMaxIMP(node -> children[0] -> height, 
								  	  node -> children[1] -> height) + 1;
		}	
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
	
	if (direction == RIGHT)
	{
		if (GetDiffFactorIMP(node -> children[direction]) >= 0)
		{
			pivot = SimpleRotation(node, direction);
		}
		else
		{
			DoubleRotation(node, direction);
			pivot = SimpleRotation(node, direction);
		}
	}
	else
	{
		if (GetDiffFactorIMP(node -> children[direction]) <= 0)
		{
			pivot = SimpleRotation(node, direction);
		}
		else
		{
			DoubleRotation(node, direction);
			pivot = SimpleRotation(node, direction);
		}
	}
	
	return pivot;
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

static avl_node_t *SimpleRotation(avl_node_t *node, int direction)
{
	avl_node_t *pivot = node -> children[direction];
	
	node -> children[direction] = pivot -> children[!direction];
	pivot -> children[!direction] = node;
	
	UpdateHeightIMP(node);
	
	return pivot;									  
}

static void DoubleRotation(avl_node_t *node, int direction)
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

size_t AVLGetHeight(const avl_t *tree)
{
	return (tree -> root -> height);
}

int AVLIsEmpty(const avl_t *tree)
{
	return (NULL == (tree -> root));
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

size_t MyMaxIMP(size_t x, size_t y)
{
	if (x > y)
	{
		return x;
	}
	
	return y;
}

static num_of_children_t WhichChildrenExistIMP(avl_node_t *node)
{
	if ((NULL != node -> children[0]) && (NULL != node -> children[1]))
	{
		return BOTH;
	}
	else if (NULL != node -> children[0])
	{
		return LEFT;
	}
	
	return RIGHT;
}

static int GetHeightIMP(avl_node_t *node)
{
	if (NULL == node)
	{
		return -1;
	}
	
	return node -> height;
}
