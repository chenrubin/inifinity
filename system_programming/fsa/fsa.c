/*****************************************/
/* OL79                                  */
/* FSA                                   */
/* 26/11/19                              */
/* Author- Sharon Rottner                */
/* Reviewer- Chen Rubin                  */
/*****************************************/
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */

#include "fsa.h" 
#define WORD_SIZE sizeof(size_t)

/* The function returns the actual size of block*/
static size_t TotalSizeOfBlockIMP(size_t block_size);
/* The function swaps two variables */
static void SwapIMP(size_t *offset, size_t *next_free);

struct fsa
{
	size_t next_free;
	size_t block_size;
};

typedef struct block_header
{
	size_t offset;
} fsa_block_header_t;

fsa_t *FSAInit(void *memory_pool, size_t pool_size, size_t block_size)
{
	fsa_t fsa = {0};
	size_t i = 0;
	size_t num_of_block = 0;
	char *run_pool = memory_pool;
	fsa_block_header_t block_header = {0};
	
	assert(memory_pool);
	assert(pool_size > block_size);
	
	fsa.next_free = sizeof(fsa_t);
	fsa.block_size = TotalSizeOfBlockIMP(block_size);
	*(fsa_t*)run_pool = fsa;
	run_pool += sizeof(fsa_t);
	num_of_block = (pool_size - sizeof(fsa_t)) / (fsa.block_size); 
	
	for (i = 0; i < (num_of_block - 1); ++i)
	{
	
		block_header.offset = (run_pool + (fsa.block_size)) - 
		                      (char *)memory_pool;
		*(fsa_block_header_t *)run_pool = block_header;
		run_pool = run_pool + (fsa.block_size);
	}
	
	block_header.offset = 0;
	*(fsa_block_header_t *)run_pool = block_header;
	
	return memory_pool;	
}

size_t FSASuggestedSize(size_t num_of_blocks, size_t block_size)
{
	size_t total_size_of_block = TotalSizeOfBlockIMP(block_size);
	size_t total_size_of_pool = (total_size_of_block * num_of_blocks) + 
	                             sizeof(fsa_t);
	
	return total_size_of_pool;	 
}

void *FSAAlloc(fsa_t *fsa)
{	
	void *allocated_address = NULL;

	assert(fsa);
	
	if (0 == FSACountFree(fsa))
	{
		return NULL;
	}
	allocated_address = (char *)fsa + fsa->next_free; 
	SwapIMP((size_t *)allocated_address, &(fsa->next_free));
	
	return ((char *)allocated_address + sizeof(fsa_block_header_t));				
}

void FSAFree(void *allocated_address)
{
	fsa_block_header_t *header = NULL;
	fsa_t *fsa = NULL;
	
	if (NULL == allocated_address)
	{
		return;
	}
	
	header = (fsa_block_header_t *)
	         ((char *)allocated_address - sizeof(fsa_block_header_t));
	fsa = (fsa_t *)((char *)header - (header->offset));
	SwapIMP(&(header->offset), &(fsa->next_free));	
}

size_t FSACountFree(fsa_t *fsa)
{
	size_t next_free = 0;
	size_t counter = 0;
	fsa_block_header_t *header = NULL;
	
	assert(fsa);
	
	next_free = fsa->next_free;
	while (0 != next_free)
	{
		header = (fsa_block_header_t *)((char*)fsa + next_free);
		next_free = header->offset; 
		++counter;
	}
	
	return counter;
}

static size_t TotalSizeOfBlockIMP(size_t block_size)
{
	size_t size_of_block_no_padding = block_size + sizeof(fsa_block_header_t);
	size_t size_of_padding = 0;
	size_t total_size_of_block = 0;
	
	if(0 != (size_of_block_no_padding % WORD_SIZE))
	{
		size_of_padding = WORD_SIZE - (size_of_block_no_padding % WORD_SIZE);
	}
	
	total_size_of_block = size_of_block_no_padding + size_of_padding;
	
	return total_size_of_block;
}

static void SwapIMP(size_t *offset, size_t *next_free)
{
	size_t temp = *offset;
	
	*offset = *next_free;
	*next_free = temp;
}
