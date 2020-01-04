/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		dhcp							  *
*		7/11/2019					  *
*									  *
************************************/

#include "dhcp.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define SUB_TREE_FULL(x) (1 == (node -> x -> is_subtree_full))

static int IsExistIMP(node_t *node, unsigned int ip, size_t level);
static void CheckSubTreeFullFlagIMP(node_t *node);

static int CreateAddress(node_t *node, unsigned int ip, size_t level)
{
	
}

static void CheckSubTreeFullFlagIMP(node_t *node)
{
	if ((1 == SUB_TREE_FULL(children[current_bit])) &&
		(1 == SUB_TREE_FULL(children[!current_bit])))
	{
		node -> is_subtree_full = 1;
	}
}
