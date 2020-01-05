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

typedef enum alloc_status3
{
	TRIE_SUCCESS_ALLOCATED_REQUESTED = 0,
	TRIE_MALLOC_FAIL = 1,
	TRIE_REQUESTED_IP_OCCUPIED = 2
}trie_alloc_status_t;

typedef enum status4
{
	TRIE_SUCCESS = 0,
	TRIE_DOUBLE_FREE = 1,
	TRIE_IP_NOT_FOUND = 2
}trie_free_status_t;

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
trie_alloc_status_t TrieInsert(trie_t *trie, 
						  unsigned int requested_ip, 
						  unsigned int *result);

/* reset all flags of nodes in the path of the ip edge */
trie_free_status_t TrieDeallocate(trie_t *trie, unsigned int ip);

/* */
size_t TrieCountAlloc(trie_t *trie);

/* */
size_t TrieCountFree(trie_t *trie, size_t height);

/* */ 
int TrieIsFull(trie_t *trie);

#endif
