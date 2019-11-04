/********************************
*	Author: ChenR				*
*	Reviewer: EyalR				*
*	ds_vector					*
*	3/11/2019					*
*								*
********************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */
#include "vector.h" /* stack functions */

#define EXTEND_SHRINK_FACTOR 2
#define SHRINK_LIMIT 4 
#define MAX(a,b) (a) >= (b) ? a : b 

struct d_vector_t
{
	size_t capacity;
	size_t element_size;
	size_t size;
	void *head;
};

d_vector_t *VectorCreate(size_t capacity, size_t elements_size)
{
	size_t num_of_Bytes = capacity * elements_size;
	char *actual_vector_t = (char *)malloc(num_of_Bytes);
	d_vector_t *new_vector = (d_vector_t *)malloc(sizeof(d_vector_t));
	if (NULL == actual_vector_t)
	{
		printf("no allocation was made to vector\n");
	}
	if (NULL == new_vector)
	{
		printf("no allocation was made to struct\n");
	}
	
	assert(0 != capacity);
	assert(0 != elements_size);
	
	new_vector -> capacity = capacity;
	new_vector -> element_size = elements_size;
	new_vector -> size = 0;
	new_vector -> head = actual_vector_t;
	
	return new_vector;
}

int VectorReserve(d_vector_t *vector, size_t new_capacity)
{
	void *new_head = NULL;
	
	assert(NULL != vector);
	assert(0 != new_capacity);
	
	new_capacity = MAX((vector -> size), new_capacity);
	
	new_head = realloc(vector -> head, 
					   new_capacity * vector -> element_size);
	if (NULL == new_head)
	{
		printf("Unable to realocate vector\n");
		
		return 1;
	}
	
	vector -> head = new_head;
	vector -> capacity = new_capacity;
	
	return 0;
}

void VectorDestroy(d_vector_t *vector)
{
	assert(NULL != vector);
	
	free(vector -> head);
	free(vector);	
}

int VectorPushBack(d_vector_t *vector, const void *n)
{
	assert(NULL != vector);
	
	if ((vector -> size) == (vector -> capacity))
	{
		if (VectorReserve(vector, EXTEND_SHRINK_FACTOR * (vector -> capacity)))
		{
			printf("unable to push");
			printf(" ,vector is full and reallocation is impossible\n");
			
			return 1;
		}
	}
	
	memcpy(
	((char *)(vector -> head) + (vector -> size) * (vector -> element_size)), 
	n, vector -> element_size);
	(vector -> size) += 1;
	
	return 0;
}

void *VectorGetItemAddress(const d_vector_t *vector, size_t index)
{
	assert(NULL != vector);
	assert(index <= ((vector -> size) - 1));
	
	return ((char *)(vector -> head) + index * (vector -> element_size));
}

void VectorPopBack(d_vector_t *vector)
{
	assert(NULL != vector);
	
	if ((vector -> size) <= ((vector -> capacity) / SHRINK_LIMIT))
	{
		(vector -> capacity) /= EXTEND_SHRINK_FACTOR;
	}
	
	(vector -> size) -= 1;
}

size_t VectorCapacity(const d_vector_t *vector)
{
	assert(NULL != vector);
	
	return (vector -> capacity);
}

size_t VectorSize(const d_vector_t *vector)
{
	assert (NULL != vector);
	
	return (vector -> size);
}
