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
#define HEADER_SIZE sizeof(vsa_t)

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
static size_t FreeSpaceToUseIMP(vsa_t *vsa, size_t size, int is_size_relevant);

/* advances header in step */
static vsa_t *AdvanceHeaderIMP(vsa_t *header, size_t step);
ssize_t MaxSize(ssize_t a, ssize_t b);

vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	size_t actual_size = pool_size - 2*HEADER_SIZE;
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
					   HEADER_SIZE)) -> magic_number = MAGIC;
	#endif
	
	header = AdvanceHeaderIMP(header, pool_size - HEADER_SIZE);
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
	addition = HEADER_SIZE + size_after_padding;
	
	if (0 == block_size)
	{
		return NULL;
	}

	while (first_header -> block_size)
	{
		first_header = FindFirstFreeSpaceIMP(first_header);
		space_to_allocate = FreeSpaceToUseIMP(first_header, block_size, 1);
		
		if ((size_after_padding + HEADER_SIZE + WORDSIZE) <= 
										 space_to_allocate)
		{
			header_after = (vsa_t *)((char *)first_header + addition);
			block_size_after =  (-1) * (space_to_allocate - addition);
			first_header -> block_size = size_after_padding;
			header_after -> block_size = block_size_after;
			#ifdef NDEBUG
			first_header -> magic_number = MAGIC;
			#endif
			
			return ((char *)first_header + HEADER_SIZE);
		}
		else if (size_after_padding <= space_to_allocate)
		{
			first_header -> block_size = space_to_allocate;
			#ifdef NDEBUG
			first_header -> magic_number = MAGIC;
			#endif
			
			return (void *)((char *)first_header + HEADER_SIZE);
		}
		
		first_header = AdvanceHeaderIMP(first_header, 
										 HEADER_SIZE + space_to_allocate);
	}
	
	return NULL;
}

void VSAFree(void *allocated_address)
{
	if (allocated_address)
	{
		allocated_address = (char *)allocated_address - HEADER_SIZE;
		
		#ifdef NDEBUG
		if (MAGIC == (((vsa_t *)allocated_address) -> magic_number))
		{
			((vsa_t *)allocated_address) -> block_size = 
				-abs(((vsa_t *)allocated_address) -> block_size);
		}
		#endif
		
		#ifndef NDEBUG
		((vsa_t *)allocated_address) -> block_size = 
			-abs(((vsa_t *)allocated_address) -> block_size);
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
		space_to_allocate = FreeSpaceToUseIMP(runner, 0, 0);
		max_space = MAX2(max_space, space_to_allocate);
		runner = AdvanceHeaderIMP(runner, space_to_allocate + HEADER_SIZE);
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
			vsa = AdvanceHeaderIMP(vsa, (vsa -> block_size) + HEADER_SIZE);
		}
		
		return vsa;
	}
	
	return NULL;
}

static size_t FreeSpaceToUseIMP(vsa_t *vsa, size_t size, int is_size_relevant)
{
	size_t free_space = 0;
	ssize_t num_of_headers = -1;
		
	while ((vsa -> block_size) < 0)
	{
		++num_of_headers;
		free_space += (-1) * (vsa -> block_size);
		vsa = AdvanceHeaderIMP(vsa, (-1) * (vsa -> block_size) + HEADER_SIZE);
		
		if (free_space >= size && is_size_relevant)
		{
			break;
		}				
	}
/*	MAX2(num_of_headers * (ssize_t)HEADER_SIZE, 0)*/
	return (free_space + MaxSize(num_of_headers * HEADER_SIZE, 0));
}

static vsa_t *AdvanceHeaderIMP(vsa_t *header, size_t step)
{
	if (HEADER_SIZE != step)
	{
		header = (vsa_t *)((char *)header + step);
	}
	
	return header;
}

ssize_t MaxSize(ssize_t a, ssize_t b)
{
	return (a >= b ? a : b);
}
