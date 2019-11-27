#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "fsa.h"
#include "MyUtils.h" /* MAX2,MIN2 */

void TestSuggestion();
void TestInit();
void TestAlloc();
void TestFree();
void TestCountFree();

static size_t calculateActualBlockSizeIMP(size_t block_size);

typedef struct block_header
{
	size_t offset;
} fsa_block_header_t;

struct fsa
{
	size_t next_free;
	size_t block_size;
} ;

int main()
{
	TestSuggestion();
	TestInit();
	TestAlloc();
	TestFree();
	TestCountFree();	

	return 0;
}

void TestSuggestion()
{
	int num_of_blocks1 = 5;
	int num_of_blocks2 = 125;
	
	typedef struct test1
	{
		int num;
		char ch;
		size_t age;
	} test1;
	
	typedef struct test2
	{
		char ch;
	} test2;
	
	PRINTTESTRESULTS("TestSuggestion",1, 
					  136 == FSASuggestedSize(num_of_blocks1, sizeof(test1)));
					  
	PRINTTESTRESULTS("TestSuggestion",2, 
					  2016 == FSASuggestedSize(num_of_blocks2, sizeof(test2)));
					  
	printf("\n\n");
}

void TestInit()
{
	int num_of_blocks = 5;
	size_t suggested_size = 0;
	void *user_buffer = NULL;
	fsa_t *new_fsa = NULL;
	int i = 0;
	
	typedef struct test	
	{
		int num;
		char ch;
		size_t age;
	} test;
	
	size_t actual_size_of_block = calculateActualBlockSizeIMP(sizeof(test));
	
	test cell = {10, 'A', 39};

	suggested_size = FSASuggestedSize(num_of_blocks, sizeof(cell));
	user_buffer = (void *)malloc(suggested_size);
	
	new_fsa = FSAInit(user_buffer, suggested_size, sizeof(cell));

	printf("%ld\n", ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t))) -> offset);
	printf("%ld\n", ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + actual_size_of_block)) -> offset);
	printf("%ld\n", ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + 2*actual_size_of_block)) -> offset);
	printf("%ld\n", ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + 3*actual_size_of_block)) -> offset);
	printf("%ld\n", ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + 4*actual_size_of_block)) -> offset);
	
	PRINTTESTRESULTS("TestInit",0, sizeof(fsa_t) == new_fsa -> next_free);
	for (i = 0; i < num_of_blocks - 1; ++i)
	{
		PRINTTESTRESULTS("TestInit",i + 1, sizeof(fsa_t) + (i + 1) * actual_size_of_block == ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + i * actual_size_of_block)) -> offset);
	}
	
	PRINTTESTRESULTS("TestInit",num_of_blocks - 1, 0 == ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + i * actual_size_of_block)) -> offset);
	
	free(user_buffer);
	printf("\n\n");
}

void TestAlloc()
{
	int num_of_blocks = 5;
	size_t suggested_size = 0;
	void *user_buffer = NULL;
	fsa_t *new_fsa = NULL;	
	
	typedef struct test	
	{
		int num;
		char ch;
		size_t age;
	} test;
	
	test *block_to_allocate1 = NULL;
	test *block_to_allocate2 = NULL;
	
	test cell = {10, 'A', 39};

	suggested_size = FSASuggestedSize(num_of_blocks, sizeof(cell));
	user_buffer = (void *)malloc(suggested_size);
	
	new_fsa = FSAInit(user_buffer, suggested_size, sizeof(cell));
	
	block_to_allocate1 = FSAAlloc(new_fsa);
	block_to_allocate1 -> num = 2;
	block_to_allocate1 -> ch = 'C';
	block_to_allocate1 -> age = 27;	

	PRINTTESTRESULTS("TestAlloc",1, ((test *)(((char *)new_fsa + sizeof(fsa_t) + sizeof(fsa_block_header_t)))) -> num == block_to_allocate1 -> num);
	PRINTTESTRESULTS("TestAlloc",2, ((test *)(((char *)new_fsa + sizeof(fsa_t) + sizeof(fsa_block_header_t)))) -> ch == block_to_allocate1 -> ch);
	PRINTTESTRESULTS("TestAlloc",3, ((test *)(((char *)new_fsa + sizeof(fsa_t) + sizeof(fsa_block_header_t)))) -> age == block_to_allocate1 -> age);
	
	block_to_allocate2 = FSAAlloc(new_fsa);
	block_to_allocate2 -> num = 2;
	block_to_allocate2 -> ch = 'C';
	block_to_allocate2 -> age = 27;	

	PRINTTESTRESULTS("TestAlloc",4, ((test *)(((char *)new_fsa + sizeof(fsa_t) + sizeof(fsa_block_header_t)))) -> num == block_to_allocate2 -> num);
	PRINTTESTRESULTS("TestAlloc",5, ((test *)(((char *)new_fsa + sizeof(fsa_t) + sizeof(fsa_block_header_t)))) -> ch == block_to_allocate2 -> ch);
	PRINTTESTRESULTS("TestAlloc",6, ((test *)(((char *)new_fsa + sizeof(fsa_t) + sizeof(fsa_block_header_t)))) -> age == block_to_allocate2 -> age);
	
	free(user_buffer);
	printf("\n\n");
}

void TestFree()
{
	int num_of_blocks = 5;
	size_t suggested_size = 0;
	void *user_buffer = NULL;
	fsa_t *new_fsa = NULL;	
	
	typedef struct test	
	{
		int num;
		char ch;
		size_t age;
	} test;
	
	test *block_to_allocate1 = NULL;
	test *block_to_allocate2 = NULL;
	
	size_t actual_size_of_block = calculateActualBlockSizeIMP(sizeof(test));
	
	test cell = {10, 'A', 39};

	suggested_size = FSASuggestedSize(num_of_blocks, sizeof(cell));
	user_buffer = (void *)malloc(suggested_size);
	
	new_fsa = FSAInit(user_buffer, suggested_size, sizeof(cell));
	
	block_to_allocate1 = FSAAlloc(new_fsa);
	block_to_allocate1 -> num = 2;
	block_to_allocate1 -> ch = 'C';
	block_to_allocate1 -> age = 27;	
	
	block_to_allocate2 = FSAAlloc(new_fsa);
	block_to_allocate2 -> num = 3;
	block_to_allocate2 -> ch = 'A';
	block_to_allocate2 -> age = 30;
	
	printf("free allocation\n");
	FSAFree(block_to_allocate1);

	PRINTTESTRESULTS("TestFree",1, 16 == (new_fsa -> next_free));
	PRINTTESTRESULTS("TestFree",2, 64 == ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t))) -> offset);
	PRINTTESTRESULTS("TestFree",2, 40 == ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + actual_size_of_block)) -> offset);
	PRINTTESTRESULTS("TestFree",3, 88 == ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + 2 * actual_size_of_block)) -> offset);
	PRINTTESTRESULTS("TestFree",4, 112 == ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + 3 * actual_size_of_block)) -> offset);
	PRINTTESTRESULTS("TestFree",5, 0 == ((fsa_block_header_t *)((char *)new_fsa + sizeof(fsa_t) + 4 * actual_size_of_block)) -> offset);
	
	FSAFree(NULL);
	
	free(user_buffer);
	printf("\n\n");
}

void TestCountFree()
{
	int num_of_blocks = 5;
	size_t suggested_size = 0;
	void *user_buffer = NULL;
	fsa_t *new_fsa = NULL;	
	
	typedef struct test	
	{
		int num;
		char ch;
		size_t age;
	} test;
	
	test *block_to_allocate1 = NULL;
	test *block_to_allocate2 = NULL;
	
	test cell = {10, 'A', 39};

	suggested_size = FSASuggestedSize(num_of_blocks, sizeof(cell));
	user_buffer = (void *)malloc(suggested_size);
	
	new_fsa = FSAInit(user_buffer, suggested_size, sizeof(cell));
	
	PRINTTESTRESULTS("TestCountFree",1, 5 == FSACountFree(new_fsa));

	
	printf("Adding Block\n");
	block_to_allocate1 = FSAAlloc(new_fsa);
	block_to_allocate1 -> num = 2;
	block_to_allocate1 -> ch = 'C';
	block_to_allocate1 -> age = 27;
	
	PRINTTESTRESULTS("TestCountFree",1, 4 == FSACountFree(new_fsa));

	
	printf("Adding Block\n");
	block_to_allocate2 = FSAAlloc(new_fsa);
	block_to_allocate2 -> num = 3;
	block_to_allocate2 -> ch = 'A';
	block_to_allocate2 -> age = 30;
	
	PRINTTESTRESULTS("TestCountFree",2, 3 == FSACountFree(new_fsa));

	printf("free allocation\n");
	FSAFree(block_to_allocate1);
	
	PRINTTESTRESULTS("TestCountFree",3, 4 == FSACountFree(new_fsa));

	block_to_allocate2 = FSAAlloc(new_fsa);
	PRINTTESTRESULTS("TestCountFree",4, 3 == FSACountFree(new_fsa));

	block_to_allocate2 = FSAAlloc(new_fsa);
	PRINTTESTRESULTS("TestCountFree",5, 2 == FSACountFree(new_fsa));

	block_to_allocate2 = FSAAlloc(new_fsa);

	PRINTTESTRESULTS("TestCountFree",6, 1 == FSACountFree(new_fsa));

	block_to_allocate2 = FSAAlloc(new_fsa);
	PRINTTESTRESULTS("TestCountFree",7, 0 == FSACountFree(new_fsa));
	
	free(user_buffer);
	printf("\n\n");
}

static size_t calculateActualBlockSizeIMP(size_t block_size)
{
	size_t actual_block_size = sizeof(fsa_block_header_t) + block_size;
	
	if (block_size % 8)
	{
		actual_block_size = ((actual_block_size / 8) * 8) + 8;
	}
	
	return actual_block_size;
}
