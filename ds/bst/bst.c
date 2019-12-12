/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		bst							  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

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
	
	return new_bst;
}

void BSTDestroy(bst_t *tree);
{
	bst_iter_t runner = tree -> stub.left;
	
	while (NULL != runner)
	{
		if (DoesHaveRightChild(runner))
		{
			if (IsThereAnyChild(runner -> right))
			{
				runner = runner -> right;
			}
			else
			{
					
			}
		}
		else
		{
			
		}
	}
}
