/****************************************************************************
*                                                                           *
*                               OL79 - 02/01/19                             *
*                                                                           *
*                                 Trie										*
*                                                                           *
*****************************************************************************/

#ifndef ILRD_TRIE_H
#define ILRD_TRIE_H

#include <stddef.h> /* size_t */


typedef struct trie trie_t; 
typedef struct node node_t; 

/* * * * * * MOVE TO SOURCE FILE !! * * * * 
struct trie 
{
	node_t *node;
	size_t level;
};

struct node 
{
	node_t children[2];
	int is_subtree_full;
};
* * * * * * * * * * * * * * * * * * * * */
typedef enum alloc_status
{
	SUCCESS_ALLOCATED_REQUSTED = 0,
	SUCCESS_ALLOCATED_AVAILABLE = 1,
	MALLOC_FAIL = 2,
	TRIE_FULL = 3
}alloc_status_t;


/* Function creates a trie.
 * Receives a level.
 * Return Values:
 *               - returns pointer to the trie.
 *               - if the function fails, it returns NULL. */
trie_t *TrieCreate(size_t level);

/* The function destroys the trie and frees all memory allocations. */
void TrieDestroy(trie_t *trie);


/*	The function recieves requested_ip and result (binary ip).
*	Function allocates ip address.
*	The allocated address will be saved in the result address  
*	If requested address equals to 0, the result address will not be affected by it.
*	If allocation fails result address will be NULL
*	Inside functions are the following actions:
*	Checks if address is valid
*	Checks if address is already allocated and either allocates it or chooses
*	another one.
*	If trie is full returns TRIE_FULL
*/
alloc_status_t TrieInsert(trie_t *trie, unsigned int requested_ip, unsigned int *result);

/* reset all flags of nodes in the path of the ip edge */
alloc_status_t TrieDeallocate(trie_t *trie, unsigned int ip);





#endif
