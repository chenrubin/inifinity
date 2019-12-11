#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "vsa.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define SIZE_OF_ALLOC 200
#define HEADER_NO_DEBUG 8
#define HEADER_DEBUG 16
#define MAGIC 0x12345678
#define WORD_SIZE sizeof(size_t)

void TestInit();
void TestAlloc();
void TestFree();
void TestLargestChunk();

int main()
{
	TestInit();
	TestAlloc();
	TestFree();
	TestLargestChunk();
	
	return 0;
}

void TestInit()
{
	void *memory_pool = (void *)malloc(SIZE_OF_ALLOC);
	vsa_t *new_vsa = VSAInit(memory_pool, SIZE_OF_ALLOC);
	size_t final_header_addr = 0;
	
	#ifdef NDEBUG
	final_header_addr = (size_t)((char *)new_vsa + HEADER_NO_DEBUG + SIZE_OF_ALLOC - 2 * HEADER_NO_DEBUG);
	PRINTTESTRESULTS("TestInit",1, (-1) * (SIZE_OF_ALLOC - 2 * HEADER_NO_DEBUG) == *((ssize_t *)new_vsa));
	PRINTTESTRESULTS("TestInit",2, 0 == *((ssize_t *)final_header_addr));
	#endif	

	#ifndef NDEBUG
	final_header_addr = (size_t)((char *)new_vsa + HEADER_DEBUG + SIZE_OF_ALLOC - 2 * HEADER_DEBUG);
	PRINTTESTRESULTS("TestInitDebug",1, (-1) * (SIZE_OF_ALLOC - 2 * HEADER_DEBUG) == *((ssize_t *)new_vsa));
	PRINTTESTRESULTS("TestInitDebug",2, 0 == *(size_t *)final_header_addr);
	PRINTTESTRESULTS("TestInitDebug",3, MAGIC == *((size_t *)((char *)(new_vsa) + 8)));
	PRINTTESTRESULTS("TestInitDebug",4, MAGIC == *((size_t *)((char *)(final_header_addr) + 8)));
	#endif

	free(memory_pool);
}

void TestAlloc()
{	
	void *memory_pool = (void *)malloc(SIZE_OF_ALLOC);
	vsa_t *new_vsa = VSAInit(memory_pool, SIZE_OF_ALLOC);
	int *allocated_space1 = NULL;
	int *allocated_space2 = NULL;
	int *allocated_space3 = NULL;
	int *allocated_space4 = NULL;
	size_t first_header_addr = 0;
	size_t second_header_addr = 0;
	size_t third_header_addr = 0;
	
	#ifdef NDEBUG
	size_t fourth_header_addr = 0;
	#endif
	
	size_t final_header_addr = 0;
	
	allocated_space1 = (int *)VSAAlloc(new_vsa, 30);
	allocated_space2 = (int *)VSAAlloc(new_vsa, 71);
	allocated_space3 = (int *)VSAAlloc(new_vsa, 20);
	allocated_space4 = (int *)VSAAlloc(new_vsa, 0);
	*allocated_space1 = 4;
	*allocated_space2 = 8;
	*allocated_space3 = 12;
	
	PRINTTESTRESULTS("TestAlloc",0, NULL == allocated_space4);
	#ifdef NDEBUG
	first_header_addr = (size_t)new_vsa;
	second_header_addr = (size_t)((char *)new_vsa + HEADER_NO_DEBUG + 32);
	third_header_addr = (size_t)((char *)second_header_addr + HEADER_NO_DEBUG + 72);
	fourth_header_addr = (size_t)((char *)third_header_addr + HEADER_NO_DEBUG + 24);
	final_header_addr = (size_t)((char *)fourth_header_addr + HEADER_NO_DEBUG + 32);

	PRINTTESTRESULTS("TestAlloc",1, 32 == *((ssize_t *)first_header_addr));
	PRINTTESTRESULTS("TestAlloc",2, 4 == *(int *)((ssize_t *)first_header_addr + 1));
	PRINTTESTRESULTS("TestAlloc",3, 72 == *((ssize_t *)second_header_addr));
	PRINTTESTRESULTS("TestAlloc",4, 8 == *(int *)((ssize_t *)second_header_addr + 1));
	PRINTTESTRESULTS("TestAlloc",5, 24 == *((ssize_t *)third_header_addr));
	PRINTTESTRESULTS("TestAlloc",6, 12 == *(int *)((ssize_t *)third_header_addr + 1));
	PRINTTESTRESULTS("TestAlloc",7, -32 == *((ssize_t *)fourth_header_addr));
	PRINTTESTRESULTS("TestAlloc",8, 0 == *((ssize_t *)final_header_addr));
	#endif
	
	#ifndef NDEBUG
	first_header_addr = (size_t)new_vsa;
	second_header_addr = (size_t)((char *)new_vsa + HEADER_DEBUG + 32);
	third_header_addr = (size_t)((char *)second_header_addr + HEADER_DEBUG + 72);
	final_header_addr = (size_t)((char *)third_header_addr + HEADER_DEBUG + 32);

	PRINTTESTRESULTS("TestAllocDebug",1, 32 == *((ssize_t *)first_header_addr));
	PRINTTESTRESULTS("TestAllocDebug",2, MAGIC == *(size_t *)((ssize_t *)first_header_addr + 1));
	PRINTTESTRESULTS("TestAllocDebug",3, 4 == *(int *)((ssize_t *)first_header_addr + 2));
	PRINTTESTRESULTS("TestAllocDebug",4, 72 == *((ssize_t *)second_header_addr));
	PRINTTESTRESULTS("TestAllocDebug",5, MAGIC == *(size_t *)((ssize_t *)second_header_addr + 1));
	PRINTTESTRESULTS("TestAllocDebug",6, 8 == *(int *)((ssize_t *)second_header_addr + 2));
	PRINTTESTRESULTS("TestAllocDebug",7, 32 == *((ssize_t *)third_header_addr));
	PRINTTESTRESULTS("TestAllocDebug",8, MAGIC == *(size_t *)((ssize_t *)third_header_addr + 1));
	PRINTTESTRESULTS("TestAllocDebug",9, 12 == *(int *)((ssize_t *)third_header_addr + 2));
	PRINTTESTRESULTS("TestAllocDebug",10, 0 == *((ssize_t *)final_header_addr));
	#endif

	free(memory_pool);

}

void TestFree()
{	
	void *memory_pool = (void *)malloc(SIZE_OF_ALLOC);
	vsa_t *new_vsa = VSAInit(memory_pool, SIZE_OF_ALLOC);
	int *allocated_space1 = NULL;
	int *allocated_space2 = NULL;
	int *allocated_space3 = NULL;
	size_t first_header_addr = 0;
	size_t second_header_addr = 0;
	size_t third_header_addr = 0;
	
	#ifdef NDEBUG
	size_t fourth_header_addr = 0;
	#endif
	
	size_t final_header_addr = 0;
	
	allocated_space1 = (int *)VSAAlloc(new_vsa, 30);
	allocated_space2 = (int *)VSAAlloc(new_vsa, 71);
	allocated_space3 = (int *)VSAAlloc(new_vsa, 20);
	*allocated_space1 = 4;
	*allocated_space2 = 8;
	*allocated_space3 = 12;
	
	VSAFree(allocated_space2);
	
	#ifdef NDEBUG
	first_header_addr = (size_t)new_vsa;
	second_header_addr = (size_t)((char *)new_vsa + HEADER_NO_DEBUG + 32);
	third_header_addr = (size_t)((char *)second_header_addr + HEADER_NO_DEBUG + 72);
	fourth_header_addr = (size_t)((char *)third_header_addr + HEADER_NO_DEBUG + 24);
	final_header_addr = (size_t)((char *)fourth_header_addr + HEADER_NO_DEBUG + 32);

	PRINTTESTRESULTS("TestFree",1, 32 == *((ssize_t *)first_header_addr));
	PRINTTESTRESULTS("TestFree",2, 4 == *(int *)((ssize_t *)first_header_addr + 1));
	PRINTTESTRESULTS("TestFree",3, -72 == *((ssize_t *)second_header_addr));
	PRINTTESTRESULTS("TestFree",5, 24 == *((ssize_t *)third_header_addr));
	PRINTTESTRESULTS("TestFree",6, 12 == *(int *)((ssize_t *)third_header_addr + 1));
	PRINTTESTRESULTS("TestFree",7, -32 == *((ssize_t *)fourth_header_addr));
	PRINTTESTRESULTS("TestFree",8, 0 == *((ssize_t *)final_header_addr));
	#endif
	
	#ifndef NDEBUG
	first_header_addr = (size_t)new_vsa;
	second_header_addr = (size_t)((char *)new_vsa + HEADER_DEBUG + 32);
	third_header_addr = (size_t)((char *)second_header_addr + HEADER_DEBUG + 72);
	final_header_addr = (size_t)((char *)third_header_addr + HEADER_DEBUG + 32);

	PRINTTESTRESULTS("TestFreeDebug",1, 32 == *((ssize_t *)first_header_addr));
	PRINTTESTRESULTS("TestFreeDebug",2, MAGIC == *(size_t *)((ssize_t *)first_header_addr + 1));
	PRINTTESTRESULTS("TestFreeDebug",3, 4 == *(int *)((ssize_t *)first_header_addr + 2));
	PRINTTESTRESULTS("TestFreeDebug",4, -72 == *((ssize_t *)second_header_addr));
	PRINTTESTRESULTS("TestFreeDebug",5, MAGIC == *(size_t *)((ssize_t *)second_header_addr + 1));
	PRINTTESTRESULTS("TestFreeDebug",7, 32 == *((ssize_t *)third_header_addr));
	PRINTTESTRESULTS("TestFreeDebug",8, MAGIC == *(size_t *)((ssize_t *)third_header_addr + 1));
	PRINTTESTRESULTS("TestFreeDebug",9, 12 == *(int *)((ssize_t *)third_header_addr + 2));
	PRINTTESTRESULTS("TestFreeDebug",10, 0 == *((ssize_t *)final_header_addr));
	#endif

	free(memory_pool);

}

void TestLargestChunk()
{	
	void *memory_pool = (void *)malloc(SIZE_OF_ALLOC);
	vsa_t *new_vsa = VSAInit(memory_pool, SIZE_OF_ALLOC);
	int *allocated_space1 = NULL;
	int *allocated_space2 = NULL;
	int *allocated_space3 = NULL;
	
	#ifdef NDEBUG
	PRINTTESTRESULTS("TestLargestChunk",1, 184 == VSALargestChunk(new_vsa));
	allocated_space1 = (int *)VSAAlloc(new_vsa, 30);
	PRINTTESTRESULTS("TestLargestChunk",2, 144 == VSALargestChunk(new_vsa));
	allocated_space2 = (int *)VSAAlloc(new_vsa, 71);
	PRINTTESTRESULTS("TestLargestChunk",3, 64 == VSALargestChunk(new_vsa));
	allocated_space3 = (int *)VSAAlloc(new_vsa, 20);
	PRINTTESTRESULTS("TestLargestChunk",4, 32 == VSALargestChunk(new_vsa));	
	printf("Free allocated space\n");
	VSAFree(allocated_space2);	
	PRINTTESTRESULTS("TestLargestChunk",5, 72 == VSALargestChunk(new_vsa));
	printf("Free allocated space\n");
	VSAFree(allocated_space1);
	PRINTTESTRESULTS("TestLargestChunk",6, 112 == VSALargestChunk(new_vsa));
	allocated_space1 = (int *)VSAAlloc(new_vsa, 115);
	PRINTTESTRESULTS("TestLargestChunk",7, 112 == VSALargestChunk(new_vsa));
	allocated_space1 = (int *)VSAAlloc(new_vsa, 108);
	PRINTTESTRESULTS("TestLargestChunk",8, 32 == VSALargestChunk(new_vsa));
	#endif
	
	#ifndef NDEBUG
	PRINTTESTRESULTS("TestLargestChunkDebug",1, 168 == VSALargestChunk(new_vsa));
	allocated_space1 = (int *)VSAAlloc(new_vsa, 30);
	PRINTTESTRESULTS("TestLargestChunkDebug",2, 120 == VSALargestChunk(new_vsa));
	allocated_space2 = (int *)VSAAlloc(new_vsa, 71);
	PRINTTESTRESULTS("TestLargestChunkDebug",3, 32 == VSALargestChunk(new_vsa));
	allocated_space3 = (int *)VSAAlloc(new_vsa, 20);
	PRINTTESTRESULTS("TestLargestChunkDebug",4, 0 == VSALargestChunk(new_vsa));
	printf("Free allocated space\n");
	VSAFree(allocated_space2);
	PRINTTESTRESULTS("TestLargestChunkDebug",5, 72 == VSALargestChunk(new_vsa));
	printf("Free allocated space\n");
	VSAFree(allocated_space1);
	PRINTTESTRESULTS("TestLargestChunkDebug",6, 120 == VSALargestChunk(new_vsa));
	allocated_space1 = (int *)VSAAlloc(new_vsa, 124);
	PRINTTESTRESULTS("TestLargestChunkDebug",7, 120 == VSALargestChunk(new_vsa));
	allocated_space1 = (int *)VSAAlloc(new_vsa, 110);
	PRINTTESTRESULTS("TestLargestChunkDebug",8, 0 == VSALargestChunk(new_vsa));
	#endif
	
	free(memory_pool);
}
