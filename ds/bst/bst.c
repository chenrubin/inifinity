/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		bst							  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "bst.h"
#include "MyUtils.h" /* MAX2,MIN2 */



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

/* checks if iterator has a right child */
static int HasRightChildIMP(bst_iter_t);

/* checks if iterator has a left child */
static int HasLeftChildIMP(bst_iter_t);

/* checks if child is the right child of parent or not */
static int IsRightChildIMP(bst_iter_t parent, bst_iter_t child);

/* checks if child is the left child of parent or not */
static int IsLeftChildIMP(bst_iter_t parent, bst_iter_t child);

/* Creates node */
static bst_iter_t CreateNodeIMP(void *data, bst_iter_t parent);

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
		if (HasRightChildIMP(runner))
		{
			runner = runner -> right;
		}
		else
		{
			if (HasLeftChildIMP(runner))
			{
				runner = runner -> left;
			}
			else
			{
				is_right_child = IsRightChildIMP(runner -> parent, runner);
				runner = runner -> parent;
				
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
			runner = runner -> left;
			is_parent_right_child = 0;
		}
		else
		{
			runner = runner -> right;
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

static int HasRightChildIMP(bst_iter_t iter)
{
	return (NULL != (iter -> right));
}

static int HasLeftChildIMP(bst_iter_t iter)
{
	return (NULL != (iter -> left));
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
	if (HasRightChildIMP(iter))
	{
		iter = iter -> right;
		while (HasLeftChildIMP(iter))
		{
			iter = (iter -> left);
		}
		
		return iter;
	}
	else
	{
		while (IsRightChildIMP(iter -> parent, iter))
		{
			iter = (iter -> parent);
		}
		
		return (iter -> parent);
	}
	
	return NULL;
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
	if (HasLeftChildIMP(iter))
	{
		iter = iter -> left;
		while (HasRightChildIMP(iter))
		{
			iter = (iter -> right);
		}
		
		return iter;
	}
	else
	{
		while (IsLeftChildIMP(iter -> parent, iter))
		{
			iter = (iter -> parent);
		}
		
		return (iter -> parent);
	}
	
	return NULL;
}

int BSTIsSameIterator(bst_iter_t iter1, bst_iter_t iter2)
{
	if (((iter1 -> parent) == (iter2 -> parent)) &&
		((iter1 -> left) == (iter2 -> left)) &&
		((iter1 -> right) == (iter2 -> right)) &&
		(BSTGetData(iter1) == BSTGetData(iter2)))
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
	
	while (NULL != (runner -> left))
	{
		runner = (runner -> left);
	}
	
	return runner;
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
	
	for (runner = BSTBegin(tree); 
		 !BSTIsSameIterator(runner, BSTEnd(tree));
		 runner = BSTNext(runner))
	{
		if (0 == (tree -> comparison_func(data, 
									BSTGetData(runner), 
									tree -> comparison_param)))
		{
			return runner; 
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
