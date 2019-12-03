/************************************
*		Author: ChenR				  *
*		Reviewer: Erez				  *
*		vsa							  *
*		3/12/2019					  *
*									  *
************************************/
#include <stdlib.h> /* NULL */
#include <assert.h> /* assert */

#include "vsa.h"
#include "MyUtils.h" /* MAX2,MIN2 */

#define MAGIC 0x12345678
#define WORDSIZE sizeof(size_t)

typedef struct block_header
{
	ssize_t block_size;
	#ifdef NDEBUG
        size_t magic_number;
    #endif
}vsa_block_header_t;

#define WORD_SIZE sizeof(size_t)

/* align size to word size */
static size_t WordAlignIMP(size_t size);

/* finds the first header that points to a free space */
static vsa_t *FindFirstFreeSpaceIMP(vsa_t *vsa);

/* determines if there is enough free space to allocate for size satrting vsa
   is_size_relevant is used in Largest chunk when I look for largest */
static size_t FreeSpaceToUse(vsa_t *vsa, size_t size, int is_size_relevant);

/* advances header in step */
static vsa_t *AdvanceHeader(vsa_t *header, size_t step);

vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	size_t actual_size = pool_size - 2*sizeof(vsa_t);
	vsa_block_header_t *header = NULL;	
	
	assert(memory_pool);
	
	header = (vsa_t *)memory_pool;
	header -> block_size = (-1) * actual_size;
	
	#ifdef NDEBUG
	((vsa_t *)memory_pool) -> magic_number = MAGIC;
	#endif
	
	#ifdef NDEBUG
	((vsa_t *)((char *)memory_pool + 
					   pool_size - 
					   sizeof(vsa_t))) -> magic_number = MAGIC;
	#endif
	
	header = AdvanceHeader(header, pool_size - sizeof(vsa_t));
	header -> block_size = 0;
	
	return (vsa_t *)memory_pool;
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	size_t size_after_padding = WordAlignIMP(block_size);
	vsa_t *first_header = NULL;
	ssize_t block_size_after = 0;
	size_t space_to_allocate = 0;
	size_t addition = 0;
	vsa_t *header_after = NULL;
	
	assert(vsa);
	
	first_header = vsa;
	addition = sizeof(vsa_t) + size_after_padding;
	
	if (0 == block_size)
	{
		return NULL;
	}

	while (first_header -> block_size)
	{
		first_header = FindFirstFreeSpaceIMP(first_header);
		space_to_allocate = FreeSpaceToUse(first_header, block_size, 1);
		
		if ((size_after_padding + sizeof(vsa_t) + WORDSIZE) <= 
										 space_to_allocate)
		{
			header_after = (vsa_t *)((char *)first_header + addition);
			block_size_after =  (-1) * (space_to_allocate - addition);
			first_header -> block_size = size_after_padding;
			header_after -> block_size = block_size_after;
			#ifdef NDEBUG
			first_header -> magic_number = MAGIC;
			#endif
			
			return ((char *)first_header + sizeof(vsa_t));
		}
		else if (size_after_padding <= space_to_allocate)
		{
			first_header -> block_size = space_to_allocate;
			#ifdef NDEBUG
			first_header -> magic_number = MAGIC;
			#endif
			
			return (void *)((char *)first_header + sizeof(vsa_t));
		}
		
		first_header = AdvanceHeader(first_header, 
										 sizeof(vsa_t) + space_to_allocate);
	}
	
	return NULL;
}

void VSAFree(void *allocated_address)
{
	if (allocated_address)
	{
		allocated_address = (char *)allocated_address - sizeof(vsa_t);
		
		#ifdef NDEBUG
		if (MAGIC == (((vsa_t *)allocated_address) -> magic_number))
		{
			((vsa_t *)allocated_address) -> block_size *= (-1);
		}
		#endif
		
		#ifndef NDEBUG
		((vsa_t *)allocated_address) -> block_size *= (-1);
		#endif
	}	
}

size_t VSALargestChunk(vsa_t *vsa)
{
	vsa_t *runner = NULL;
	size_t space_to_allocate = 0;
	size_t max_space = 0;
	
	assert(vsa);
	
	runner = vsa;
	
	while (0 != runner -> block_size)
	{
		runner = FindFirstFreeSpaceIMP(runner);
		space_to_allocate = FreeSpaceToUse(runner, 0, 0);
		max_space = MAX2(max_space, space_to_allocate);
		runner = AdvanceHeader(runner, space_to_allocate + sizeof(vsa_t));
	}
	
	return max_space;	
}

static size_t WordAlignIMP(size_t size)
{
	size_t size_of_padding = 0;
	size_t size_after_padding = 0;
	
	if(0 != (size % WORD_SIZE))
	{
		size_of_padding = WORD_SIZE - (size % WORD_SIZE);
	}
	
	size_after_padding = size + size_of_padding;

	return size_after_padding;
}

static vsa_t *FindFirstFreeSpaceIMP(vsa_t *vsa)
{	
	while (0 != (vsa -> block_size))
	{		
		while (vsa -> block_size > 0)
		{
			vsa = AdvanceHeader(vsa, (vsa -> block_size) + sizeof(vsa_t));
		}
		
		return vsa;
	}
	
	return NULL;
}

static size_t FreeSpaceToUse(vsa_t *vsa, size_t size, int is_size_relevant)
{
	size_t free_space = 0;
	ssize_t num_of_headers = -1;
		
	while ((vsa -> block_size) < 0)
	{
		++num_of_headers;
		free_space += 
		(-1) * (vsa -> block_size) + num_of_headers * sizeof(vsa_t);
		vsa = AdvanceHeader(vsa, (-1) * (vsa -> block_size) + sizeof(vsa_t));
		
		if (free_space >= size && is_size_relevant)
		{
			break;
		}				
	}
	
	return free_space;
}

static vsa_t *AdvanceHeader(vsa_t *header, size_t step)
{
	if (sizeof(vsa_t) != step)
	{
		header = (vsa_t *)((char *)header + step);
	}
	
	return header;
}
