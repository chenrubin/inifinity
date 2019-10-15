/********************************************
*	Author : ChenR							*
*	Reviewer : Tamir						*
*	WS6	- 									*
*	including bitwise tests					*
*											*				
********************************************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <string.h>

long Pow2(unsigned int x, unsigned int y);
int IsPowOfTwoLoop(unsigned int num);
int IsPowOfTwoNoLoop(unsigned int num);
int AddOne(int num);
void ThreeBitsOn(unsigned int *num, int size); /* exactly 3 bits */
int ByteMirrorLoop(int num); /* for 8 bits Byte only */
int ByteMirrorNoLoop(int num); /* for 8 bits Byte only */
int BothBitsExist(unsigned char chr);
int OnlyOneBitExists(unsigned char chr);
unsigned char SwitchBits(unsigned char num);
unsigned int ClosestSmallerNum(unsigned int num); /* that devides by 16 */
void SwapVeriables(int *num1, int *num2);
int NumOfSetBitsLoop(int num);
int NumOfSetBitsNoLoop(unsigned int x);
void FloatAnalysis(float num);
void PrintBits(int num);
void TestPow2();
void TestIsPowOfTwoLoop();
void TestIsPowOfTwoNoLoop();
void TestAddOne();
void TestThreeBitsOn();
void TestByteMirrorLoop();
void TestByteMirrorNoLoop();
void TestBothBitsExist();
void TestOnlyOneBitExists();
void TestSwitchBits();
void TestClosestSmallerNum();
void TestSwapVeriables();
void TestNumOfSetBitsLoop();
void TestNumOfSetBitsNoLoop();
void TestFloatAnalysis();

#define BINMASK 32767 
#define m1 0x5555555555555555
#define m2 0x3333333333333333
#define m4 0x0f0f0f0f0f0f0f0f
#define m8 0x00ff00ff00ff00ff
#define m16 0x0000ffff0000ffff
#define EVEN_ONES 85 /* bin 01010101 */
#define ODD_ONES 170 /* bin 10101010 */
#define EVEN_PAIRS 51 /* bin 00110011 */
#define ODD_PAIRS 204 /* bin 11001100 */
#define EVEN_QUARTET 15 /* bin 00001111 */
#define ODD_QUARTET 240 /* bin 11110000 */
#define BYTE 8
#define checkResultOneNum(num, res, expres)\
		{\
			if (res == expres) \
			{ \
				printf("PASSED for %d\n", num); \
			} \
			else \
			{ \
				printf("FAILED for %d\n", num); \
				printf("res = %d, expres = %d\n", res, expres);\
			}\
		}							
#define checkResultTwoNum(num1, num2, res, expres)\
		{\
			if (res == expres) \
			{ \
				printf("PASSED for %d, %d\n", num1, num2); \
			} \
			else \
			{ \
				printf("FAILED for %d, %d\n", num1, num2); \
				printf("res = %ld, expres = %ld\n", res, expres); \
			}\
		}									

long Pow2(unsigned int x, unsigned int y)
{
	long res = (long)x;
	
	return (res <<= y);
}

int IsPowOfTwoLoop(unsigned int num)
{	
	while (0 == (num % 2))
	{
		num = num >> 1;
	}
	
	return (1 == num);
}

int IsPowOfTwoNoLoop(unsigned int num)
{
	int num_two_comp = (~num + 1);
		
	if ((num_two_comp & num) == num && (num != 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int AddOne(int num)
{
	int counter = 0;
	
	if (0 == (num & 1)) /* even number */
	{
		return (num | 1); 
	}
	else
	{
		while (!(0 == (num & 1)))
		{
			num = num >> 1;
			++counter;
		}
		
		num = ((num | 1) << counter);
	}
	
	return num;
}

void ThreeBitsOn(unsigned int *num, int size)
{
	int i = 0;
	int counter = 0;
	unsigned int tmp_num = 0;
	
	for (i = 0; i < size; ++i)
	{
		tmp_num = num[i];
		counter = 0;
		while (tmp_num > 0)
		{
			if (tmp_num & 1)
			{
				++counter;
			}	
			if (counter > 3)
			{
				break;
			}
			
			tmp_num = tmp_num >> 1;
		}
		
		if (3 == counter)
		{
			printf("%u, ", num[i]);
		}
	}
	
	printf("\n");
}

int ByteMirrorLoop(int num)
{
	int res = 0;
	int i = 0;
	
	for (i = 0; i < BYTE; ++i)
	{
		res = res << 1;
		res = res | (num & 1);
		num = (num >> 1) & BINMASK;
	}
	
	return res;
}

int ByteMirrorNoLoop(int num)
{
	int shift_num1 = 0;
	int shift_num2 = 0;
	
	shift_num1 = (num & EVEN_ONES) << 1;
	shift_num2 = (num & ODD_ONES) >> 1;
	num = shift_num1 | shift_num2;
	shift_num1 = (num & EVEN_PAIRS) << 2;
	shift_num2 = (num & ODD_PAIRS) >> 2;
	num = shift_num1 | shift_num2;
	shift_num1 = (num & EVEN_QUARTET) << 4;
	shift_num2 = (num & ODD_QUARTET) >> 4;
	num = shift_num1 | shift_num2;
	
	return num;
}

int BothBitsExist(unsigned char chr)
{	
	return (68 == (68 & chr));
}

int OnlyOneBitExists(unsigned char chr)
{	
	return (((68 & chr) == 64) ^ ((68 & chr) == 4));
}

unsigned char SwitchBits(unsigned char num)
{
	unsigned char bit3to5 = (num & 8) << 2;
	unsigned char bit5to3 = (num & 32) >> 2;
	
	num = (~32) & num; /* put zero instead of bit 3 */
	num = (~8) & num; /* put zero instead of bit 5 */
	num = ((num | bit3to5) | bit5to3);
	
	return num;	   
}

unsigned int ClosestSmallerNum(unsigned int num)
{	
	num = num >> 4;
	num = num << 4;
	
	return num;
}

void SwapVeriables(int *num1, int *num2)
{	
	*num1 = *num1 ^ *num2;
	*num2 = *num1 ^ *num2;
	*num1 = *num1 ^ *num2;
}

int NumOfSetBitsLoop(int num)
{
	int counter = 0;
	
	while (num != 0)
	{
		if (num & 1)
		{
			++counter;
		}
		
		num = (num >> 1) & BINMASK;
	}
	
	return counter;
}

int NumOfSetBitsNoLoop(unsigned int x)
{	
    x -= (x >> 1) & m1;				/* put count of each 2 bits into those 2 bits */
    x = (x & m2) + ((x >> 2) & m2); /* put count of each 4 bits into those 4 bits */
    x = (x + (x >> 4)) & m4;        /* put count of each 8 bits into those 8 bits */
    x += x >>  8;  					/* put count of each 16 bits into their lowest 8 bits */
    x += x >> 16;  					/* put count of each 32 bits into their lowest 8 bits */
    return x & 0x7f;
}

void FloatAnalysis(float num)
{
	int *ptr = (int *)&num;
	
	PrintBits(*ptr);
	printf(" <-- Actual\n");
}

void PrintBits(int num)
{
	int size = 8 * sizeof(int);
	unsigned int mask = 2147483648; /* 1000...0 */
	int i = 0;
	
	for (i = 0; i < size; ++i)
	{
		if ((num & mask) == mask)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
		
		mask = mask >> 1;
	}
}

void TestPow2()
{	
	int x_input[] = {2,3,7};
	int y_input[] = {5,2,3};
	long expres[] = {64, 12, 56};
	int len = sizeof(x_input) / sizeof(int);
	int i = 0;
	
	printf("\n\n/********************************\n");
	printf("			POW2				 \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultTwoNum(x_input[i], y_input[i], 
						  Pow2(x_input[i], y_input[i]), expres[i]);
	} 
}

void TestIsPowOfTwoLoop()
{
	int input[] = {2, 32, 65, 1024, 500};
	int expres[] = {1, 1, 0 , 1, 0};
	int len = sizeof(input) / sizeof(int);
	int i = 0;
	
	printf("\n\n/********************************\n");
	printf("  Is power of two with loop 	 \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  IsPowOfTwoLoop(input[i]), expres[i]);
	}
}

void TestIsPowOfTwoNoLoop()
{
	int input[] = {2, 32, 65, 1024, 500};
	int expres[] = {1, 1, 0 , 1, 0};
	int len = sizeof(input) / sizeof(int);
	int i = 0;
	
	printf("\n\n/********************************\n");
	printf("  Is power of two without loop 	 \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  IsPowOfTwoNoLoop(input[i]), expres[i]);
	}
}

void TestAddOne()
{
	int input[] = {2, -111, -64, 256, 1};
	int expres[] = {3, -110, -63, 257, 2};
	int len = sizeof(input) / sizeof(int);
	int i = 0;
	
	printf("\n\n/******************************************\n");
	printf("  Add one without arithmetic operations 	 \n");		
	printf("********************************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  AddOne(input[i]), expres[i]);
	}
}

void TestThreeBitsOn()
{
	unsigned int arr[] = {5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
						  21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
	int expres[] = {7, 11, 13, 14, 19, 21, 22, 25, 26, 28, 35};
	int len = sizeof(expres) / sizeof(int);
	int i = 0;

	printf("\n\n/********************************\n");
	printf("  	Exactly 3 bits on		 	 \n");	
	printf("********************************/\n\n");
	printf("This test is done manually\n");
	printf("Actual result\n");
	ThreeBitsOn(arr, 31);
	printf("Expected result\n");
	
	for (i = 0; i < len; ++i)
	{
		printf("%d, ", expres[i]);
	}
	
	printf("\n");
}

void TestByteMirrorLoop()
{
	int input[] = {29, 24, 0, 236};
	int expres[] = {184, 24, 0, 55};
	int len = sizeof(input) / sizeof(int);
	int i = 0;
	
	printf("\n\n/********************************\n");
	printf("	Byte mirror with loop		 \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  ByteMirrorLoop(input[i]), expres[i]);
	}
}

void TestByteMirrorNoLoop()
{
	int input[] = {29, 24, 0, 236};
	int expres[] = {184, 24, 0, 55};
	int len = sizeof(input) / sizeof(int);
	int i = 0;
	
	printf("\n\n/********************************\n");
	printf("	Byte mirror without loop	   \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  ByteMirrorNoLoop(input[i]), expres[i]);
	}
}

void TestBothBitsExist()
{
	int input[] = {23, 24, 25, 26, 27, 94, 68, 255};
	int expres[] = {0, 0, 0, 0, 0, 1, 1, 1};
	int i = 0;
	int len = sizeof(input) / sizeof(int);
	
	printf("\n\n/********************************\n");
	printf("	Both bits 2 and 6 on		 \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  BothBitsExist(input[i]), expres[i]);
	}
}

void TestOnlyOneBitExists()
{
	int input[] = {23, 24, 25, 26, 27, 64, 68, 94, 255};
	int expres[] = {1, 0, 0, 0, 0, 1, 0, 0, 0};
	int i = 0;
	int len = sizeof(input) / sizeof(int);
	
	printf("\n\n/********************************\n");
	printf("	Only one bit 2 or 6 exist	 \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  OnlyOneBitExists(input[i]), expres[i]);
	}
}

void TestSwitchBits()
{	
	int input[] = {16, 24, 112, 255};
	int expres[] = {16, 48, 88, 255};
	int i = 0;
	int len = sizeof(input) / sizeof(int);
	
	printf("\n\n/********************************\n");
	printf("	Swap between bits 3 and 5	 \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  SwitchBits(input[i]), expres[i]);
	}
}

void TestClosestSmallerNum()
{
	unsigned int input[] = {30, 0, 31, 32, 45, 257, 102, 110};
	unsigned int expres[] = {16, 0, 16, 32, 32, 256, 96, 96};
	int i = 0;
	int len = sizeof(input) / sizeof(int);
	
	printf("\n\n/********************************************\n");
	printf("	Closest smaller number devisable by 16	 \n");	
	printf("********************************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  ClosestSmallerNum(input[i]), expres[i]);
	}	
}

void TestSwapVeriables()
{
	int x = 1;
	int y = 49;
	int x_after_swap = x;
	int y_after_swap = y;
	
	printf("\n\n/*************************************\n");
	printf("  Swap without using a third veriable \n");	
	printf("*************************************/\n\n");
	
	SwapVeriables(&x, &y);
	
	if ((x  == y_after_swap) &&
		(y == x_after_swap))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED to swap between x = %d and y = %d\n",x, y);
	}
}

void TestNumOfSetBitsLoop()
{
	int input[] = {561, 32, -95, -788, 30000, 1, 0};
	int expres[] = {4, 1, 11, 11, 7, 1, 0};
	int i = 0;
	int len = sizeof(input) / sizeof(int);
	
	printf("\n\n/********************************\n");
	printf("	Num of set bits with loop	 \n");
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  NumOfSetBitsLoop(input[i]), expres[i]);
	}
}

void TestNumOfSetBitsNoLoop()
{
	int input[] = {561, 32, 95, 200, 255, 1, 0};
	int expres[] = {4, 1, 6, 3, 8, 1, 0};
	int i = 0;
	int len = sizeof(input) / sizeof(int);
	
	printf("\n\n/********************************\n");
	printf("  Num of set bits without loop	 \n");	
	printf("********************************/\n\n");
	
	for (i = 0; i < len; ++i)
	{
		checkResultOneNum(input[i], 
						  NumOfSetBitsNoLoop(input[i]), expres[i]);
	}
}

void TestFloatAnalysis()
{
	float input[] = {100.093, 98.654, 20.5, 1.5};
	char *bin[] = {"01000010110010000010111110011110", 
				   "01000010110001010100111011011001",
				   "01000001101001000000000000000000", 
				   "00111111110000000000000000000000"};
	int i = 0;
	int len = sizeof(input) / sizeof(float);
	
	printf("\n\n/********************************\n");
	printf("  Float analysis					 \n");
	printf("********************************/\n\n");
	
	printf("This test is done manually\n");
	
	for (i = 0; i < len; ++i)
	{
		printf("%s <-- Expected\n", bin[i]);
		FloatAnalysis(input[i]);
		printf("\n");
	}
}

int main()
{
	TestPow2();
	TestIsPowOfTwoLoop();
	TestIsPowOfTwoNoLoop();
	TestAddOne();
	TestThreeBitsOn();
	TestByteMirrorLoop();
	TestByteMirrorNoLoop();
	TestBothBitsExist();
	TestOnlyOneBitExists();
	TestSwitchBits();
	TestClosestSmallerNum();
	TestSwapVeriables();
	TestNumOfSetBitsLoop();
	TestNumOfSetBitsNoLoop();
	TestFloatAnalysis();

	return 0;
}
