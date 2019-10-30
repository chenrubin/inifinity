#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "barr.h"

#define LSB 1
#define BITS_IN_ARRAY (sizeof(size_t) * CHAR_BIT)
#define WORD sizeof(size_t)
#define MASK_0xFF 0xFF
#define LUT_COUNT 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3,\
2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4,\
4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4,\
2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3,\
3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6,\
5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4,\
4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,\
3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,\
4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5,\
4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7,\
7, 8
#define LUT_MIRROR 0x0, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90,\
0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 0x8, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68,\
0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 0x4, 0x84, 0x44, 0xC4,\
0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 0xC,\
0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC,\
0x7C, 0xFC, 0x2, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52,\
0xD2, 0x32, 0xB2, 0x72, 0xF2, 0xA, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,\
0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, 0x6, 0x86, 0x46, 0xC6, 0x26,\
0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 0xE, 0x8E,\
0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E,\
0xFE, 0x1, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1,\
0x31, 0xB1, 0x71, 0xF1, 0x9, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19,\
0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 0x5, 0x85, 0x45, 0xC5, 0x25, 0xA5,\
0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, 0xD, 0x8D, 0x4D,\
0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,\
0x3, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33,\
0xB3, 0x73, 0xF3, 0xB, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B,\
0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 0x7, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67,\
0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 0xF, 0x8F, 0x4F, 0xCF,\
0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF

unsigned char lut_table_count[] = {LUT_COUNT};
unsigned char lut_table_mirror[] = {LUT_MIRROR};

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
		return (arr | (LSB << loc));
	}
	else
	{
		return arr & (~(LSB << loc));	
	}
}

bitarray BArrSetOn(bitarray arr, size_t loc)
{
	assert(BITS_IN_ARRAY > loc);
	
	return (arr | ((size_t)LSB << loc));
}

bitarray BArrSetOff(bitarray arr, size_t loc)
{
	assert(BITS_IN_ARRAY > loc);
	
	return (arr & ~((size_t)LSB << loc));
}

int BArrIsOn(bitarray arr, size_t loc)
{
	assert(BITS_IN_ARRAY > loc);
	
	return ((((size_t)LSB << loc) & arr) ? LSB : 0);
}

int BArrIsOff(bitarray arr, size_t loc)
{
	assert(BITS_IN_ARRAY > loc);
	
	return ((((size_t)LSB << loc) & arr) ? 0 : 1);
}

size_t BArrCountOn(bitarray arr)
{
	size_t i = 0;
	size_t counter = 0;
	size_t mask = LSB;
	
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

size_t BArrCountOnLUT(bitarray arr)
{
	unsigned char *arr_p = (unsigned char *)&arr;
	size_t counter = 0;
	size_t i = 0;
	
	for (i = 0; i < WORD; ++i)
	{
		counter += lut_table_count[*arr_p];
		++arr_p;
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
		res = res | (arr & LSB);
		arr = arr >> 1;
	}
	
	return res;
}

bitarray BArrMirrorLUT(bitarray arr)
{
	bitarray res = 0;
	int i = 0;
	
	for (i = 0; i < CHAR_BIT -1; ++i)
	{	
		res |= lut_table_mirror[arr & MASK_0xFF];
		res <<= CHAR_BIT;
		arr >>= CHAR_BIT;
	}
	res |= lut_table_mirror[arr & MASK_0xFF];
	
	return res;
}

char* BArrToString(char* buffer ,bitarray arr)
{
	size_t i = 0;
	size_t bit_mask = (size_t)LSB << (BITS_IN_ARRAY - 1);
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
	return (arr ^ ((size_t)LSB << loc));
}
