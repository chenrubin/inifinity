#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "barr.h"

#define BYTE 8
#define BITS_IN_ARRAY (sizeof(size_t) * BYTE)


bitarray BArrSetAll(bitarray arr)
{	
	return ((size_t)(pow(2, BITS_IN_ARRAY) - 1) | arr);
}

bitarray BArrResetAll(bitarray arr)
{
	return (arr & 0);
}

bitarray BArrSetBit(bitarray arr, size_t loc, int mod)
{
	assert(BITS_IN_ARRAY > loc);
	assert(0 == mod || 1 == mod);
	
	if (mod)
	{
		return (arr | (1 << loc));
	}
	else
	{
		return arr & (~(1 << loc));	
	}
}

bitarray BArrSetOn(bitarray arr, size_t loc)
{
	assert(BITS_IN_ARRAY > loc);
	
	return (arr | ((size_t)1 << loc));
}

bitarray BArrSetOff(bitarray arr, size_t loc)
{
	assert(BITS_IN_ARRAY > loc);
	
	return (arr & ~((size_t)1 << loc));
}

int BArrIsOn(bitarray arr, size_t loc)
{
	assert(BITS_IN_ARRAY > loc);
	
	return ((((size_t)1 << loc) & arr) ? 1 : 0);
}

int BArrIsOff(bitarray arr, size_t loc)
{
	assert(BITS_IN_ARRAY > loc);
	
	return ((((size_t)1 << loc) & arr) ? 0 : 1);
}

size_t BArrCountOn(bitarray arr)
{
	size_t i = 0;
	size_t counter = 0;
	size_t mask = 1;
	
	for (i = 0; i < BITS_IN_ARRAY; ++i)
	{
		if (mask & arr)
		{
			++counter;
		}
		
		mask <<= 1;
	}
	
	return counter;
}

size_t BArrCountOff(bitarray arr)
{	
	return (BITS_IN_ARRAY - BArrCountOn(arr));
}

bitarray BArrRotR(bitarray arr, size_t num)
{
	int num_of_rot = num % BITS_IN_ARRAY;
		
	return ((arr >> num_of_rot) | 
			(arr << (BITS_IN_ARRAY - num_of_rot)));
}

bitarray BArrRotL(bitarray arr, size_t num)
{
	int num_of_rot = num % BITS_IN_ARRAY;
	
	return ((arr << num_of_rot) | 
			(arr >> (BITS_IN_ARRAY - num_of_rot)));
}

bitarray BArrMirror(bitarray arr)
{
	bitarray res = 0;
	size_t i = 0;
		
	for (i = 0; i < BITS_IN_ARRAY; ++i)
	{
		res = res << 1;
		res = res | (arr & 1);
		arr = arr >> 1;
	}
	
	return res;
}

char* BArrToString(char* buffer ,bitarray arr)
{
	size_t i = 0;
	size_t bit_mask = (size_t)1 << (BITS_IN_ARRAY - 1);
	char buf[2];

	assert(buffer);
	
	*(buffer) = '\0';
	
	for (i = 0; i < BITS_IN_ARRAY; ++i)
	{
		sprintf(buf, "%ld", ((arr & bit_mask) >> (BITS_IN_ARRAY - 1)));
		strcat(buffer, buf);
		arr <<= 1;
	}
	
	return buffer;
}

bitarray BArrFlip(bitarray arr, size_t loc)
{
	return (arr ^ ((size_t)1 << loc));
}
