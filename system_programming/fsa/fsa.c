/************************************
*		Author: ChenR				  *
*		Reviewer: Dvir				  *
*		fsa							  *
*		26/11/2019					  *
*									  *
************************************/

#include <assert.h> /* assert */

#include "fsa.h"
#include "MyUtils.h" /* MAX2,MIN2 */

#define WORD sizeof(size_t)
#define BLOCK_POINTER fsa_block_header_t *

struct fsa
{
	size_t next_free;
	size_t block_size;
} ;

typedef struct block_header
{
	size_t offset;
} fsa_block_header_t;

/* calculating block size icluding all padding and header */
static size_t calculateActualBlockSizeIMP(size_t block_size);

/* swap offsets between blocks */
static void SwapOffsetsIMP(size_t *offset1, size_t *offset2);

/* Creates block with a struct that includes offset */
BLOCK_POINTER CreateBlockIMP(BLOCK_POINTER new_block, 
								   size_t offset);

fsa_t *FSAInit(void *memory_pool, size_t pool_size, size_t block_size)
{
	size_t actual_block_size = calculateActualBlockSizeIMP(block_size);
	size_t num_of_blocks = pool_size / actual_block_size;
	fsa_t new_fsa = {0};
	size_t start_address = 0;
	fsa_block_header_t new_block = {0};
	size_t fsa_size = sizeof(fsa_t);
	size_t i = 0;

	assert(memory_pool);
	assert(pool_size > block_size);
	
	start_address = (size_t)memory_pool + fsa_size;
	new_fsa.next_free = fsa_size;
	new_fsa.block_size = actual_block_size;
	*(fsa_t *)memory_pool = new_fsa;
	
	for (i = 0; i < num_of_blocks - 1; ++i)
	{
		*(BLOCK_POINTER)((char *)
								(start_address + i * actual_block_size)) =
		*(CreateBlockIMP(&new_block, fsa_size + (i + 1) * actual_block_size));
	}
	*(BLOCK_POINTER)((char *)(start_address + i * actual_block_size)) = 
				  	  	    *(CreateBlockIMP(&new_block, 0));
				  	  	  
	return (fsa_t *)memory_pool;
}

size_t FSASuggestedSize(size_t num_of_blocks, size_t block_size)
{
	size_t suggested_size = 
	calculateActualBlockSizeIMP(block_size) * num_of_blocks;
	
	return (suggested_size + sizeof(fsa_t));
}

void *FSAAlloc(fsa_t *fsa)
{
	size_t free_block_offset  = 0;
	void *res = NULL;
	
	assert(fsa);
	
	free_block_offset = (fsa -> next_free);
		
	if (fsa -> next_free)
	{
		SwapOffsetsIMP(&(fsa -> next_free), 
		&(((BLOCK_POINTER)((char *)fsa + (fsa -> next_free))) -> offset));

		res = ((char *)fsa + free_block_offset + sizeof(fsa_block_header_t));
	} 

	return res;
}

void FSAFree(void *allocated_address)
{	
	size_t address_to_swap = 0;
	size_t fsa_address = 0;
	
	if (allocated_address)
	{
		address_to_swap = (size_t)allocated_address - 
						   sizeof(fsa_block_header_t);
		fsa_address = address_to_swap - 
		   	((BLOCK_POINTER)address_to_swap) -> offset;		
		
		if (((BLOCK_POINTER)address_to_swap) -> offset)
			{	
				SwapOffsetsIMP(&(((BLOCK_POINTER)address_to_swap) -> offset), 
							   &(((fsa_t *)fsa_address) -> next_free));
			} 
	}
}

size_t FSACountFree(fsa_t *fsa)
{
	size_t counter = 1;
	BLOCK_POINTER runner = 
	(BLOCK_POINTER)((char *)fsa + (fsa -> next_free));
	
	assert(fsa);
	
	if (!(fsa -> next_free))
	{
		counter = 0;
	}
	
	while (runner -> offset)
	{
		runner = (BLOCK_POINTER)((char *)fsa + runner -> offset);
		++counter;
	}
	
	return counter;
}

static size_t calculateActualBlockSizeIMP(size_t block_size)
{
	size_t actual_block_size = sizeof(fsa_block_header_t) + block_size;
	
	if (block_size % WORD)
	{
		actual_block_size = ((actual_block_size / WORD) * WORD) + WORD;
	}
	
	return actual_block_size;
}

static void SwapOffsetsIMP(size_t *offset1, size_t *offset2)
{
	size_t temp_offset = *offset1;
	*offset1 = *offset2;
	*offset2 = temp_offset;
}

BLOCK_POINTER CreateBlockIMP(BLOCK_POINTER new_block, size_t offset)
{
	new_block -> offset = offset;
	
	return new_block;
}
