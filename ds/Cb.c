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

#define MIN2(a,b) (a) <= (b) ? (a) : (b)

/* 
* advance pointer by one char while taking under consideration
* the cyclic manner of the buffer. +2 means 1 for offset that begins from
* begin[1] (and not from begin[0]) and 1 for advancing one char
*/
static char *OneCyclicAdvance(cbuff_t *cbuff, size_t offset);

/* 
* advance pointer by num Bytes
*/
static void PtrAdvance(cbuff_t *cbuff, char **ptr, size_t num);

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
	size_t num_to_write = 0;
	size_t steps_until_end = 0;
	
	assert(cbuff);
	assert(src);
	
	num_to_write = MIN2(CBuffSpaceLeft(cbuff), nbytes);
	steps_until_end = cbuff -> begin + (cbuff -> capacity)
							 - (cbuff -> write_ptr) + 1;
	
	if (steps_until_end >= num_to_write)
	{
		memcpy((cbuff -> write_ptr), src, num_to_write);
		PtrAdvance(cbuff, &(cbuff -> write_ptr), num_to_write);
	}
	else
	{
		memcpy((cbuff -> write_ptr), src, steps_until_end);
		PtrAdvance(cbuff, &(cbuff -> write_ptr), steps_until_end);
		memcpy((cbuff -> write_ptr), (char *)src + steps_until_end, 
				num_to_write - steps_until_end);
		PtrAdvance(cbuff, &(cbuff -> write_ptr), 
				   num_to_write - steps_until_end);
	}
	
	return num_to_write;
}

ssize_t CBuffRead(cbuff_t *cbuff, void *dest, size_t nbytes)
{
	size_t num_to_read = 0;
	size_t steps_until_end = 0;
	
	assert(cbuff);
	assert(dest);
	
	num_to_read = MIN2((cbuff -> capacity) - CBuffSpaceLeft(cbuff), nbytes);
	steps_until_end = (cbuff -> begin) + (cbuff -> capacity)
							 - (cbuff -> read_ptr) + 1;
	
	if (steps_until_end >= num_to_read)
	{
		memcpy(dest, (cbuff -> read_ptr), num_to_read);
		PtrAdvance(cbuff, &(cbuff -> read_ptr), num_to_read);
	}
	else
	{
		memcpy(dest, (cbuff -> read_ptr), steps_until_end);
		PtrAdvance(cbuff, &(cbuff -> read_ptr), steps_until_end);
		memcpy((char *)dest + steps_until_end, (cbuff -> read_ptr), 
				num_to_read - steps_until_end);
		PtrAdvance(cbuff, &(cbuff -> read_ptr), 
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

static char *OneCyclicAdvance(cbuff_t *cbuff, size_t offset)
{
	return (&cbuff -> begin[0] + ((offset + 1) % (cbuff -> capacity + 1)));
}

static void PtrAdvance(cbuff_t *cbuff, char **ptr, size_t num)
{
	size_t offset = 0;
	*ptr = *ptr + num - 1;
	offset = *ptr - &(cbuff -> begin[0]);
	*ptr = OneCyclicAdvance(cbuff, offset);
}
