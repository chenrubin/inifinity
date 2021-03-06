/************************************
*		Author: ChenR				  *
*		Reviewer: Tamir				  *
*		bst							  *
*		16/12/2019					  *
*									  *
************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <string.h> /* strcmp */

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

/* Creates node */
static bst_iter_t CreateNodeIMP(void *data, bst_iter_t parent);

/* assign NULL to all iter fields  */
static void NullifyIterFieldsIMP(bst_iter_t iter);

/* connect parent to child */
static void ConnectParentToChildIMP(bst_iter_t iter, const char *str);

/* Advance through nodes to the right or left until reaching a leaf*/
static bst_iter_t AdvanceThroughNodesIMP(bst_iter_t iter, char *str);

/* checks if child is lefor right child */
static int IsChildIMP(bst_iter_t child, char *direction);

/* checks if iter has a child left or right */
static int HasChildIMP(bst_iter_t iter, char *direction);

/* move to adjacent node , left child , right child or parent */
static void MoveToAdjacentNodeIMP(bst_iter_t *iter, char *direction);

/* move either next or prev */
static bst_iter_t GoToDirectionIMP(bst_iter_t iter, int next_prev);

/* increments counter for the size function using forEach */
int MyIncrementFuncIMP(void *data, void *for_each_param);

/* The function returns the root node */ 
static bst_iter_t GetRootIMP(const bst_t *tree);

/* Initialize all tub's fields */
static void InitStubIMP(bst_t *tree);

/* Destroy node */
static bst_iter_t DestroyNode(bst_iter_t iter);

bst_t *BSTCreate(comparison_func func, void *param)
{
	bst_t *new_bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == new_bst)
	{
		return NULL;
	}
	
	assert(func);
	
	new_bst -> comparison_func = func;
	new_bst -> comparison_param = param;
	
	InitStubIMP(new_bst);
	
	return new_bst;
}

void BSTDestroy(bst_t *tree)
{
	bst_iter_t runner = NULL;
	
	assert(tree);
	
	runner = &(tree -> stub);
	
	while (NULL != BSTEnd(tree) -> left)
	{
		if (!HasChildIMP(runner, "left") && !HasChildIMP(runner, "right"))
		{
			runner = DestroyNode(runner);
		}
		else if (HasChildIMP(runner, "right"))
		{
			MoveToAdjacentNodeIMP(&runner, "right");
		}
		else
		{
			MoveToAdjacentNodeIMP(&runner, "left");
		}
	}
	
	free(tree);
	tree = NULL;
}

bst_iter_t BSTInsert(bst_t *tree, void *data)
{
	bst_iter_t parent = NULL;
	bst_iter_t runner = NULL;
	int child_type_flag = 0;
	int is_parent_right_child = 0;
	
	assert(tree);
	assert(data);
	
	parent = &(tree -> stub);
	runner = GetRootIMP(tree);
	
	while (NULL != runner)
	{
		parent = runner;
		child_type_flag = tree -> comparison_func(data, 
												  runner -> data, 
												  tree -> comparison_param);
		if (child_type_flag < 0)												  
		{
			MoveToAdjacentNodeIMP(&runner, "left");
			is_parent_right_child = 0;
		}
		else
		{
			MoveToAdjacentNodeIMP(&runner, "right");
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
	return (iter1 == iter2);
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
	
	assert(tree);
	
	BSTForEach(BSTBegin(tree), BSTEnd(tree), MyIncrementFuncIMP, &counter);
	
	return counter;
}

int BSTIsEmpty(const bst_t *tree)
{
	assert(tree);
	
	return (NULL == GetRootIMP(tree));
}

bst_iter_t BSTFind(bst_t *tree, void *data)
{
	bst_iter_t runner = NULL;
	int result = 0;
	
	assert(tree);
	assert(data);
	
	runner = GetRootIMP(tree);
	while (NULL != runner)	
	{
		result = (tree -> comparison_func(data, 
										  BSTGetData(runner), 
										  tree -> comparison_param));
		switch (result)
		{
			case 1:
				MoveToAdjacentNodeIMP(&runner, "right");
				break;
				
			case -1:
				MoveToAdjacentNodeIMP(&runner, "left");
				break;
				
			default:
				return runner;				
		}
	}
	
	return BSTEnd(tree);
}

int BSTForEach(bst_iter_t begin, bst_iter_t end, action_func func, void *param)
{
	bst_iter_t runner = NULL;
/*	(void)param;*/
	
	assert(begin);
	assert(end);
	assert(func);

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
		ConnectParentToChildIMP(iter, "RightChild");
		
		last_left_child = AdvanceThroughNodesIMP(iter -> right, "left");
		last_left_child -> left = iter -> left;
		(iter -> left) -> parent = last_left_child;
	}
	else if (HasChildIMP(iter, "left"))
	{
		ConnectParentToChildIMP(iter, "LeftChild");
	}
	else if (HasChildIMP(iter, "right"))
	{
		ConnectParentToChildIMP(iter, "RightChild");
	}
	else
	{
		if (IsChildIMP(iter, "right"))
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

static int HasChildIMP(bst_iter_t iter, char *direction)
{	
	if (0 == strcmp("right" ,direction))
	{
		return (NULL != (iter -> right));
	}
	else
	{
		return (NULL != (iter -> left));
	}
}

static void MoveToAdjacentNodeIMP(bst_iter_t *iter, char *direction)
{
	if (0 == strcmp("right" ,direction))
	{
		*iter = (*iter) -> right;
	}
	else if (0 == strcmp("left" ,direction))
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
		MoveToAdjacentNodeIMP(&iter, direction1);

		while (HasChildIMP(iter, direction2))
		{
			MoveToAdjacentNodeIMP(&iter, direction2);
		}
		
		return iter;
	}
	else
	{
		while (IsChildIMP(iter, direction1))
		{
			MoveToAdjacentNodeIMP(&iter, "parent");
		}

		return (iter -> parent);
	}

	return NULL;
}

static bst_iter_t AdvanceThroughNodesIMP(bst_iter_t iter, char *str)
{
	bst_iter_t runner = iter;
	
	if (0 == strcmp("right" ,str))
	{
		while (NULL != (runner -> right))
		{
			MoveToAdjacentNodeIMP(&runner, str);
		}
	}
	else
	{
		while (NULL != (runner -> left))
		{
			MoveToAdjacentNodeIMP(&runner, str);
		}
	}
	
	return runner;
}

static void ConnectParentToChildIMP(bst_iter_t iter, const char *str)
{
	if (IsChildIMP(iter, "right"))
	{
		if (0 == strcmp("RightChild" ,str))
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
		if (0 == strcmp("RightChild" ,str))
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

static int IsChildIMP(bst_iter_t child, char *direction)
{
	if (0 == strcmp("left" ,direction))
	{
		if (((child -> parent) -> left) == child)
		{
			return 1;
		}	
	}
	else
	{
		if (((child -> parent) -> right) == child)
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

int MyIncrementFuncIMP(void *data, void *for_each_param)
{	
	(void)data;
	
	*(size_t *)for_each_param += 1;

	return 0;
}

static bst_iter_t GetRootIMP(const bst_t *tree)
{
	return (tree->stub).left;
}

static void InitStubIMP(bst_t *tree)
{
	(tree -> stub).data = (void *)0xDEADBEEF;
	(tree -> stub).parent = NULL;
	(tree -> stub).left = NULL;
	(tree -> stub).right = NULL;
}

static bst_iter_t DestroyNode(bst_iter_t iter)
{
	bst_iter_t parent = iter -> parent;
	
	if (IsChildIMP(iter, "right"))
	{
		parent -> right = NULL;
	}
	else
	{
		parent -> left = NULL;
	}
	
	NullifyIterFieldsIMP(iter);
	free(iter);
	
	return parent;
}
