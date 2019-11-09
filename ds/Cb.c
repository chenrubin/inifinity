/************************************
*		Author: ChenR				*
*		Reviewer: 				*
*		ds_CB					*
*							*
*									*
************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "CBuff.h"

#define CHANGE_ACTION_CONDITION \
((CB -> write_ptr) == (CB -> read_ptr))
#define WRITE_CONDITION \
(!CHANGE_ACTION_CONDITION) || \
((CHANGE_ACTION_CONDITION) && (0 == CB -> begin[0]))
#define READ_CONDITION \
(!CHANGE_ACTION_CONDITION) || \
((CHANGE_ACTION_CONDITION) && (1 == CB -> begin[0]))
#define LSB 1
#define W_BIT (CB -> begin[0] & LSB)
#define R_BIT ((CB -> begin[0] >> 1) & LSB)

/* 
* advance read/write pointer by one char while taking under consideration
* the cyclic manner of the buffer. +2 means 1 for offset that begins from
* begin[1] (and not from begin[0]) and 1 for advancing one char
*/
static char *OneCyclicAdvance(size_t capacity, char *begin, size_t offset);
/*
* returns position of ptr2 relative to ptr1 while taking under
* consideration the cyclic manner of the buffer
*/
static int RelativePosition(CBuff_t *CB, char *ptr1, char *ptr2);

struct CBuff
{
    size_t capacity;
    char *write_ptr;
    char *read_ptr;
    char begin[1];
};

CBuff_t *CBuffCreate(size_t capacity)
{
	CBuff_t *new_CBuff = (CBuff_t *)malloc(sizeof(CBuff_t) + capacity);
	if (NULL == new_CBuff)
	{
		return NULL;
	}
	
	new_CBuff -> capacity = capacity;
	new_CBuff -> write_ptr = (new_CBuff -> begin) + 1;
	new_CBuff -> read_ptr = (new_CBuff -> begin) + 1;	
	new_CBuff -> begin[0] = 1;
	
	return new_CBuff;
}

void CBuffDestroy(CBuff_t *CB)
{
	free(CB);
}

size_t CBuffWrite(CBuff_t *CB, const char *data, size_t num)
{
	size_t i = 0;
	size_t offset = CB -> write_ptr - &(CB -> begin[1]);
	
	for (i = 0; i < num; ++i)
	{
		if (-1 == RelativePosition(CB, CB -> read_ptr, CB -> write_ptr))
		{
			*(CB -> write_ptr) = *(data + i);
			CB -> write_ptr = 
			OneCyclicAdvance(CB -> capacity, CB -> begin, offset + i);
			CB -> begin[0] &= (~LSB);
			CB -> begin[0] |= (LSB << 1);
		}
		else if (((CB -> read_ptr) == (CB -> write_ptr)) && (0 == W_BIT))
		{
			return i;
		}
		else
		{
			*(CB -> write_ptr) = *(data + i);
			CB -> write_ptr = 
			OneCyclicAdvance(CB -> capacity, CB -> begin, offset + i);
		}
	}

	return i;
}

size_t CBuffRead(CBuff_t *CB, char *data, size_t num)
{
	size_t i = 0;
	size_t offset = CB -> read_ptr - &(CB -> begin[1]);
	
	for (i = 0; i < num; ++i)
	{
		if (-1 == RelativePosition(CB, CB -> write_ptr, CB -> read_ptr))
		{
			*(data + i) = *(CB -> read_ptr);
			CB -> read_ptr = 
			OneCyclicAdvance(CB -> capacity, CB -> begin, offset + i);
			CB -> begin[0] &= (~(LSB << 1));
			CB -> begin[0] |= LSB;
		}
		else if (((CB -> read_ptr) == (CB -> write_ptr)) && (0 == W_BIT))
		{
			return i;
		}
		else
		{
			*(data + i) = *(CB -> read_ptr);
			CB -> read_ptr = 
			OneCyclicAdvance(CB -> capacity, CB -> begin, offset + i);
		}
	}

	return i;
}

static char *OneCyclicAdvance(size_t capacity, char *begin, size_t offset)
{
	return &begin[1] + (offset + 1) % capacity;
}

static int RelativePosition(CBuff_t *CB, char *ptr1, char *ptr2)
{
	if (ptr2 <= ptr1)
	{
		return (ptr2 - ptr1);
	}
	else
	{
		return (-(ptr1 - &(CB -> begin[1]) + 
				  (&(CB -> begin[1]) + (CB -> capacity) - ptr2)));
	}
}
/*
void QDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SListFreeAll(queue -> front);
	free(queue);	
}

int QEnqueue(queue_t *queue, void *data)
{
	sl_node_t *new_node = NULL;
	
	assert(NULL != queue);
	assert(NULL != data);
	
	if (NULL == queue || NULL == data)
	{
		return 1;
	}
	
	new_node = SListCreateNode(data, NULL);
	
	SListInsert(new_node, queue -> rear);
	queue -> rear = (queue -> rear) -> next;
	
	return 0;
}

void *QPeek(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (queue -> front) -> data;
}

void QDequeue(queue_t *queue)
{
	sl_node_t *temp_node = NULL;
	
	assert(NULL != queue);
	
	temp_node = (queue -> front) -> next;
	free(queue -> front);
	queue -> front = temp_node;
}

int QIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	
	return ((queue -> front) == (queue -> rear));
}

size_t QSize(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (SListCount(queue -> front) - 1);
}

void QAppend(queue_t *dest, queue_t *src)
{
	sl_node_t *removed_node = (src -> front);
	
	assert(dest);
	assert(src);	
	
	(dest -> rear) -> next = (src -> front);
	removed_node = (src -> front);
	SListRemove(dest -> rear);
	free(removed_node);
	(dest -> rear) = (src -> rear);
	free(src);	
}*/

