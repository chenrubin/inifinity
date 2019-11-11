/************************************
*		Author: ChenR				*
*		Reviewer: Dvir				*
*		ds_CB						*
*		10/11/2019					*
*									*
************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "cbuff.h"

#define MIN2(a,b) a <= b ? a : b

/* 
* advance read/write pointer by one char while taking under consideration
* the cyclic manner of the buffer. +2 means 1 for offset that begins from
* begin[1] (and not from begin[0]) and 1 for advancing one char
*/
static char *OneCyclicAdvance(size_t capacity, char *begin, size_t offset);
/* 
* copy read_ptr to buffer given by user (with memcpy) and advancing
* it accordingly while taking under consideration the cyclic manner 
* of this data structure	
*/
static void CpyAndPtrAdvanceSource(cbuff_t *cbuff, void *dest, 
							 	   char **source, size_t num);
/* 
* copy buffer given by user to write_ptr (with memcpy) and advancing
* it accordingly while taking under consideration the cyclic manner 
* of this data structure	
*/							 	   
static void CpyAndPtrAdvanceDest(cbuff_t *cbuff, char **dest, 
							 const void *source, size_t num);

struct CB
{
    size_t capacity;
    char *write_ptr;
    char *read_ptr;
    char begin[1];
};

cbuff_t *CBuffCreate(size_t capacity)
{
	cbuff_t *new_CBuff = (cbuff_t *)malloc(sizeof(cbuff_t) + capacity);
	if (NULL == new_CBuff)
	{
		return NULL;
	}
	
	new_CBuff -> capacity = capacity;
	new_CBuff -> write_ptr = new_CBuff -> begin;
	new_CBuff -> read_ptr = new_CBuff -> begin;	
	
	return new_CBuff;
}

void CBuffDestroy(cbuff_t *cbuff)
{
	free(cbuff);
}

ssize_t CBuffWrite(cbuff_t *cbuff, const void *src, size_t nbytes)
{
	size_t num_to_write = MIN2(CBuffSpaceLeft(cbuff), nbytes);
	size_t steps_until_end = cbuff -> begin + (cbuff -> capacity)
							 - (cbuff -> write_ptr) + 1;
	
	assert(cbuff);
	assert(src);
	
	if (steps_until_end >= num_to_write)
	{
		CpyAndPtrAdvanceDest(cbuff, &(cbuff -> write_ptr), 
							 src, num_to_write);
	}
	else
	{
		CpyAndPtrAdvanceDest(cbuff,  &(cbuff -> write_ptr),
							 src, steps_until_end);					 
		CpyAndPtrAdvanceDest(cbuff, &(cbuff -> write_ptr),
							(char *)src + steps_until_end, 
							num_to_write - steps_until_end);
	}
	
	return num_to_write;
}

ssize_t CBuffRead(cbuff_t *cbuff, void *dest, size_t nbytes)
{
	size_t num_to_read = 
	MIN2((cbuff -> capacity) - CBuffSpaceLeft(cbuff), nbytes);
	size_t steps_until_end = (cbuff -> begin) + (cbuff -> capacity)
							 - (cbuff -> read_ptr) + 1;
	
	assert(cbuff);
	assert(dest);
	
	if (steps_until_end >= num_to_read)
	{
		CpyAndPtrAdvanceSource(cbuff, dest, &(cbuff -> read_ptr), 
							   num_to_read);
	}
	else
	{
		CpyAndPtrAdvanceSource(cbuff, dest, 
						 &(cbuff -> read_ptr), steps_until_end);
		CpyAndPtrAdvanceSource(cbuff, (char *)dest + steps_until_end,
						 &(cbuff -> read_ptr), 
						 num_to_read - steps_until_end);
	}
	
	return num_to_read;
}

int CBuffIsEmpty(const cbuff_t *cbuff)
{
	assert(cbuff);
	
	return ((cbuff -> write_ptr) == (cbuff -> read_ptr));	
}

size_t CBuffSpaceLeft(const cbuff_t *cbuff)
{
	assert(cbuff);
	
	if ((cbuff -> write_ptr) == (cbuff -> read_ptr))
	{
		return (cbuff -> capacity);
	}
	else if ((cbuff -> write_ptr) < (cbuff -> read_ptr))
	{
		return ((cbuff -> read_ptr) - (cbuff -> write_ptr) - 1);
	}
	else
	{
		return ((cbuff -> capacity) - 
				((cbuff -> write_ptr) - (cbuff -> read_ptr)));
	}
}

size_t CBuffCapacity(const cbuff_t *cbuff)
{
	assert(cbuff);
	
	return (cbuff -> capacity); 
}

static char *OneCyclicAdvance(size_t capacity, char *begin, size_t offset)
{
	return &begin[0] + ((offset + 1) % (capacity + 1));
}

static void CpyAndPtrAdvanceSource(cbuff_t *cbuff, void *dest, 
							 	   char **source, size_t num)
{
	size_t offset = 0;
	memcpy(dest, *source, num);
	*source = *source + num - 1;
	offset = *source - &(cbuff -> begin[0]);
	*source = 
	OneCyclicAdvance(cbuff -> capacity, cbuff -> begin, offset);
}

static void CpyAndPtrAdvanceDest(cbuff_t *cbuff, char **dest, 
							 const void *source, size_t num)
{
	size_t offset = 0;
	memcpy(*dest, source, num);
	*dest = *dest + num - 1;
	offset = *dest - &(cbuff -> begin[0]);
	*dest = 
	OneCyclicAdvance(cbuff -> capacity, cbuff -> begin, offset);
}
