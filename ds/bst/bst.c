/************************************
*		Author: ChenR				  *
*		Reviewer: Tamir				  *
*		bst							  *
*		15/12/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "bst.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define NEXT 1
#define PREV 0

struct bst_node
{
	struct bst_node *parent;
	struct bst_node *left;
	struct bst_node *right;
	void *data;
};

struct bst
{
	comparison_func comparison_func;
	void *comparison_param;
	struct bst_node stub;
};

/* checks if child is the right child of parent or not */
static int IsRightChildIMP(bst_iter_t parent, bst_iter_t child);

/* checks if child is the left child of parent or not */
static int IsLeftChildIMP(bst_iter_t parent, bst_iter_t child);

/* Creates node */
static bst_iter_t CreateNodeIMP(void *data, bst_iter_t parent);

/* assign NULL to all iter fields  */
static void NullifyIterFieldsIMP(bst_iter_t iter);

/* connect parent to child */
static void ConnectParentToChildIMP(bst_iter_t iter, 
									const char *str, bst_iter_t parent);

/* Advance through nodes to the right or left until reaching a leaf*/
static bst_iter_t AdvanceThroughNodesIMP(bst_iter_t iter, char *str);

static int IsChildIMP(bst_iter_t parent, bst_iter_t child, char *direction);

int HasChildIMP(bst_iter_t iter, char *direction);

static void MoveToOtherNode(bst_iter_t *iter, char *direction);

static bst_iter_t GoToDirectionIMP(bst_iter_t iter, int next_prev);

bst_t *BSTCreate(comparison_func func, void *param)
{
	bst_t *new_bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == new_bst)
	{
		return NULL;
	}
	
	new_bst -> comparison_func = func;
	new_bst -> comparison_param = param;
	(new_bst -> stub).data = (void *)0xDEADBEEF;
	(new_bst -> stub).parent = NULL;
	(new_bst -> stub).left = NULL;
	(new_bst -> stub).right = NULL;
	
	return new_bst;
}

void BSTDestroy(bst_t *tree)
{
	bst_iter_t runner = NULL;
	int is_right_child = 0;
	
	assert(tree);
	
	runner = &(tree -> stub);
	
	while (NULL != BSTEnd(tree) -> left)
	{
		if (HasChildIMP(runner, "right"))
		{
			MoveToOtherNode(&runner, "right");
		}
		else
		{
			if (HasChildIMP(runner, "left"))
			{
				MoveToOtherNode(&runner, "left");
			}
			else
			{
				is_right_child = IsChildIMP(runner -> parent, runner, "right");
				MoveToOtherNode(&runner, "parent");
				
				if (is_right_child)
				{
					free(runner -> right);
					runner -> right = NULL;
				}
				else
				{
					free(runner -> left);
					runner -> left = NULL;
				}
			}
		}
	}
	
	free(tree);
}

bst_iter_t BSTInsert(bst_t *tree, void *data)
{
	bst_iter_t parent = NULL;
	bst_iter_t runner = NULL;
	int child_type_flag = 0;
	int is_parent_right_child = 0;
	
	assert(tree);
	
	parent = &(tree -> stub);
	runner = (tree -> stub.left);
	
	while (NULL != runner)
	{
		parent = runner;
		child_type_flag = tree -> comparison_func(data, 
												  runner -> data, 
												  tree -> comparison_param);
		if (child_type_flag < 0)												  
		{
			MoveToOtherNode(&runner, "left");
			is_parent_right_child = 0;
		}
		else
		{
			MoveToOtherNode(&runner, "right");
			is_parent_right_child = 1;
		}
	}
	
	if (is_parent_right_child)
	{
		parent -> right = CreateNodeIMP(data, parent);
		
		return parent -> right;
	}
	else
	{
		parent -> left = CreateNodeIMP(data, parent);
		
		return parent -> left;
	}
}

bst_iter_t BSTEnd(const bst_t *tree)
{
	assert(tree);
	
	return (bst_iter_t)(&(tree -> stub));
}

void *BSTGetData(bst_iter_t iter)
{
	return (iter -> data);
}

int HasChildIMP(bst_iter_t iter, char *direction)
{	
	if ("right" == direction)
	{
		return (NULL != (iter -> right));
	}
	else
	{
		return (NULL != (iter -> left));
	}
}

static void MoveToOtherNode(bst_iter_t *iter, char *direction)
{
	if ("right" == direction)
	{
		*iter = (*iter) -> right;
	}
	else if ("left" == direction)
	{
		*iter = (*iter) -> left;
	}
	else
	{
		*iter = (*iter) -> parent;
	}
}

static bst_iter_t GoToDirectionIMP(bst_iter_t iter, int next_prev)
{
	int has_child = 0;
	bst_iter_t temp = NULL;
	char *direction1 = NULL;
	char *direction2 = NULL;
	
	switch (next_prev)
	{
		case NEXT:
			direction1 = "right";
			direction2 = "left";
			break;
		
		default:
			direction1 = "left";
			direction2 = "right";	
	}

	if (HasChildIMP(iter, direction1))
	{
		MoveToOtherNode(&iter, direction1);

		while (HasChildIMP(iter, direction2))
		{
			MoveToOtherNode(&iter, direction2);
		}
		
		return iter;
	}
	else
	{
		while (IsChildIMP(iter -> parent, iter, direction1))
		{
			MoveToOtherNode(&iter, "parent");
		}

		return (iter -> parent);
	}

	return NULL;
}

bst_iter_t BSTNext(bst_iter_t iter)
{
	return (GoToDirectionIMP(iter, NEXT));
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
	return (GoToDirectionIMP(iter, PREV));
}

int BSTIsSameIterator(bst_iter_t iter1, bst_iter_t iter2)
{
	if (iter1 == iter2)
	{
		return 1;
	}
	
	return 0;
}

bst_iter_t BSTBegin(const bst_t *tree)
{
	bst_iter_t runner = NULL;
	
	assert(tree);
	
	runner = (bst_iter_t)(&(tree -> stub));
	
	return (AdvanceThroughNodesIMP(runner, "left"));
}

size_t BSTSize(const bst_t *tree)
{
	size_t counter = 0;
	bst_iter_t runner = NULL;
	
	assert(tree);
	
	runner = BSTBegin(tree);
	while (runner != BSTEnd(tree))
	{
		runner = BSTNext(runner);
		++counter;
	}
	
	return counter;
}

int BSTIsEmpty(const bst_t *tree)
{
	assert(tree);
	
	return (NULL == (tree -> stub.left));
}

bst_iter_t BSTFind(bst_t *tree, void *data)
{
	bst_iter_t runner = NULL;
	
	assert(tree);
	
	runner = tree -> stub.left;
	while (NULL != runner)	
	{
		if (0 == (tree -> comparison_func(data, 
									BSTGetData(runner), 
									tree -> comparison_param)))
		{
			return runner; 
		}
		else
		{
			if (0 > (tree -> comparison_func(data, 
									BSTGetData(runner), 
									tree -> comparison_param)))
			{
				if (HasChildIMP(runner, "left"))
				{
					MoveToOtherNode(&runner, "left");
				}
				else
				{
					return BSTEnd(tree);
				}
			}
			else
			{
				if (HasChildIMP(runner, "right"))
				{
					MoveToOtherNode(&runner, "right");
				}
				else
				{
					return BSTEnd(tree);
				}
			}
		}
	}
	
	return BSTEnd(tree);
}

int BSTForEach(bst_iter_t begin, bst_iter_t end, action_func func, void *param)
{
	bst_iter_t runner = NULL;
	(void)param;

	for (runner = begin; 
		 !BSTIsSameIterator(runner, end);
		 runner = BSTNext(runner))
	{
		if (0 != func(runner -> data, param))
		{
			return 1;
		}
	}
	
	return 0;
}

void BSTRemove(bst_iter_t iter)
{
	bst_iter_t last_left_child = NULL;
	
	if (HasChildIMP(iter, "left") && HasChildIMP(iter, "right"))
	{
		ConnectParentToChildIMP(iter, "RightChild", iter -> parent);
		
		last_left_child = AdvanceThroughNodesIMP(iter -> right, "left");
		last_left_child -> left = iter -> left;
		(iter -> left) -> parent = last_left_child;
	}
	else if (HasChildIMP(iter, "left"))
	{
		ConnectParentToChildIMP(iter, "LeftChild", iter -> parent);
	}
	else if (HasChildIMP(iter, "right"))
	{
		ConnectParentToChildIMP(iter, "RightChild", iter -> parent);
	}
	else
	{
		if (IsChildIMP(iter -> parent, iter, "right"))
		{
			(iter -> parent) -> right = NULL;
		}
		else
		{
			(iter -> parent) -> left = NULL;
		}
	}
	
	NullifyIterFieldsIMP(iter);
	free(iter);
	iter = NULL;
}

static bst_iter_t AdvanceThroughNodesIMP(bst_iter_t iter, char *str)
{
	bst_iter_t runner = iter;
	
	if ("right" == str)
	{
		while (NULL != (runner -> right))
		{
			MoveToOtherNode(&runner, str);
		}
	}
	else
	{
		while (NULL != (runner -> left))
		{
			MoveToOtherNode(&runner, str);
		}
	}
	
	return runner;
}

static void ConnectParentToChildIMP(bst_iter_t iter, const char *str, bst_iter_t parent)
{
	if (IsRightChildIMP(iter -> parent, iter))
	{
		if ("RightChild" == str)
		{
			(iter -> parent) -> right = (iter -> right);
			(iter -> right) -> parent = iter -> parent;
		}
		else
		{
			(iter -> parent) -> right = (iter -> left);
			(iter -> left) -> parent = iter -> parent;
		}
	}
	else
	{
		if ("RightChild" == str)
		{
			(iter -> parent) -> left = (iter -> right);
			(iter -> right) -> parent = iter -> parent;
		}
		else
		{
			(iter -> parent) -> left = (iter -> left);
			(iter -> left) -> parent = iter -> parent;
		}
	} 
}

static void NullifyIterFieldsIMP(bst_iter_t iter)
{
	iter -> parent = NULL;
	iter -> left = NULL;
	iter -> right = NULL;
}

static int IsRightChildIMP(bst_iter_t parent, bst_iter_t child)
{
	if ((parent -> right) == child)
	{
		return 1;
	}
	
	return 0;
}

static int IsLeftChildIMP(bst_iter_t parent, bst_iter_t child)
{
	if ((parent -> left) == child)
	{
		return 1;
	}
	
	return 0;
}

static int IsChildIMP(bst_iter_t parent, bst_iter_t child, char *direction)
{
	int res = 0;
	
	if ("left" == direction)
	{
		if ((parent -> left) == child)
		{
			return 1;
		}	
	}
	else
	{
		if ((parent -> right) == child)
		{
			return 1;
		}	
	}
	
	return 0;
}

static bst_iter_t CreateNodeIMP(void *data, bst_iter_t parent)
{
	bst_iter_t new_node_iter = (bst_iter_t)malloc(sizeof(bst_node_t));
	if (NULL == new_node_iter)
	{
		return NULL;
	}
	
	new_node_iter -> data = data;
	new_node_iter -> parent = parent;
	new_node_iter -> left = NULL;
	new_node_iter -> right = NULL;
	
	return new_node_iter;
}
