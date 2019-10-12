#include <stdio.h> /* printf */
#include <assert.h> /* assert */

long Pow2(unsigned int x, unsigned int y);
int IsPowOfTwoLoop(unsigned int num);
int IsPowOfTwoNoLoop(unsigned int num);
int AddOne(int num);
void ThreeBitsOn(unsigned int *num, int size);
int BothBitsExist(unsigned char chr);
int OnlyOneBitExists(unsigned char chr);
unsigned int ClosestSmallerNum(unsigned int num);
int ByteMirrorLoop(int num);
void swap(int *num1, int *num2);
void FloatAnalysys(float num);

#define BINMASK 2147483647

long Pow2(unsigned int x, unsigned int y)
{
	return (x * (2 << (y - 1)));
}

int IsPowOfTwoLoop(unsigned int num)
{
	while (0 == (num % 2))
	{
		num = num / 2;
	}
	
	if (num == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsPowOfTwoNoLoop(unsigned int num)
{
	int ispower = 0;
	
	if (0 == num)
	{
		ispower = 0;
	}
	else
	{
		if (num & 1)
		{
			ispower = 1 + IsPowOfTwoNoLoop(num >> 1);
		}
		else
		{
			ispower = IsPowOfTwoNoLoop(num >> 1);
		}
	}
	
	if (1 == ispower)
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
			printf("%u\n", num[i]);
		}
	}
}

int ByteMirrorLoop(int num)
{
	int res = 0;
	int counter = 0;
	
	while (num != 0)
	{
		res = res << 1;
		res = res | (num & 1);
		num = (num >> 1) & BINMASK;
		
		if (counter > 10)
		{
			break;
		}
	}
	
	return res;
}

int BothBitsExist(unsigned char chr)
{
	return (68 == (68 & chr));
}

int OnlyOneBitExists(unsigned char chr)
{
	return (((68 & chr) == 64) ^ ((68 & chr) == 4));
}

unsigned int ClosestSmallerNum(unsigned int num)
{
	num = num >> 4;
	num = num << 4;
	
	return num;
}

void swap(int *num1, int *num2)
{
	*num1 = *num1 + *num2;
	*num2 = *num1 - *num2;
	*num1 = *num1 - *num2;
}
/*
unsigned char SwitchBits(unsigned char num)
{
	unsigned char bit3to5 = 0;
	unsigned char bit5to3 = 0;
	
	bit3to5 = num & 8;
	printf("bit3to5 original = %d\n", bit3to5);
	bit3to5 = bit3to5 << 2;
	printf("bit3to5 after shift = %d\n", bit3to5);
	bit5to3 = num & 32;
	printf("bit5to3 original = %d\n", bit5to3);
	bit5to3 = bit5to3 >> 2;
	printf("bit5to3 after shift = %d\n", bit5to3);
	
	(((0 == bit3to5) && (num = (~(bit3to5 & 32) & num))) || 
		   (((0 != bit3to5) && (num = ((bit3to5 & 32) | num)))));
	printf("num = %u\n", num);	   
	(((0 == bit5to3) && (num = (~(bit5to3 & 8) & num))) || 
		   (((0 != bit5to3) && (num = ((bit5to3 & 8) | num)))));
		   
	return num;	   
}
*/
int NumOfSetBits(int num)
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

void FloatAnalysis(float num)
{
	printf("%d\n", num);
	printf("\n");
}

int main()
{
	unsigned int x = 5;
	printf("%u, %u, %u\n", x, x << 2, x >> 2);	
/*	FloatAnalysis(7.5);
	
/*	printf("561 --> %d\n", NumOfSetBits(561));
	printf("32 --> %d\n", NumOfSetBits(32));
	printf("-95 --> %d\n", NumOfSetBits(-95));
	printf("-788 --> %d\n", NumOfSetBits(-788));
	printf("93556 --> %d\n", NumOfSetBits(93556));
	printf("1 --> %d\n", NumOfSetBits(1));
	printf("0 --> %d\n", NumOfSetBits(0));
	printf("Before, num = %u\n", num);
	printf("After, num = %u\n", SwitchBits(num));
*/	
/*	int x = 5;
	int y = 4;
	printf("Before swap x = %d, y = %d\n", x, y);
	swap(&x, &y);
	printf("Before swap x = %d, y = %d\n", x, y);
*/	
/*	printf("30 --> %u\n", ClosestSmallerNum(30));
	printf("0 --> %u\n", ClosestSmallerNum(0));
	printf("31 --> %u\n", ClosestSmallerNum(31));
	printf("32 --> %u\n", ClosestSmallerNum(32));
	printf("45 --> %u\n", ClosestSmallerNum(45));
	printf("257 --> %u\n", ClosestSmallerNum(257));
	printf("102 --> %u\n", ClosestSmallerNum(102));
	printf("30 --> %u\n", ClosestSmallerNum(30));
	printf("110 --> %u\n", ClosestSmallerNum(110));
*/
/*	printf("23 --> %d\n", BothBitsExist(23));
	printf("24 --> %d\n", BothBitsExist(24));
	printf("25 --> %d\n", BothBitsExist(25));
	printf("26 --> %d\n", BothBitsExist(26));
	printf("27 --> %d\n", BothBitsExist(27));
	printf("28 --> %d\n", BothBitsExist(28));
	printf("29 --> %d\n", BothBitsExist(29));
	printf("30 --> %d\n", BothBitsExist(30));
	printf("94 --> %d\n", BothBitsExist(94));
	printf("68 --> %d\n", BothBitsExist(68));
	printf("255 --> %d\n\n", BothBitsExist(255));
	
	printf("23 --> %d\n", OnlyOneBitExists(23));
	printf("24 --> %d\n", OnlyOneBitExists(24));
	printf("25 --> %d\n", OnlyOneBitExists(25));
	printf("26 --> %d\n", OnlyOneBitExists(26));
	printf("27 --> %d\n", OnlyOneBitExists(27));
	printf("28 --> %d\n", OnlyOneBitExists(28));
	printf("29 --> %d\n", OnlyOneBitExists(29));
	printf("30 --> %d\n", OnlyOneBitExists(30));
	printf("64 --> %d\n", OnlyOneBitExists(64));
	printf("68 --> %d\n", OnlyOneBitExists(68));
	printf("255 --> %d\n\n", OnlyOneBitExists(255));
*/
/*	printf("29 --> %d\n",ByteMirrorLoop(29));
	printf("28 --> %d\n",ByteMirrorLoop(28));
	printf("27 --> %d\n",ByteMirrorLoop(27));
	printf("26 --> %d\n",ByteMirrorLoop(26));
	printf("25 --> %d\n",ByteMirrorLoop(25));
	printf("24 --> %d\n",ByteMirrorLoop(24));
	printf("0 --> %d\n",ByteMirrorLoop(0));
	printf("-95 --> %d\n",ByteMirrorLoop(-95));
	printf("-102 --> %d\n",ByteMirrorLoop(-102));
	printf("236 --> %d\n",ByteMirrorLoop(236));
*/
/*	unsigned int arr[] = {5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33};
	ThreeBitsOn(arr, 29);
*/
/*	printf("AddOne(2) = %d\n", AddOne(2));
	printf("AddOne(-111) = %d\n", AddOne(-111));
	printf("AddOne(-64) = %d\n", AddOne(-64));
	printf("AddOne(256) = %d\n", AddOne(256));
	printf("AddOne(1) = %d\n", AddOne(1));
*/
/*	printf("%d power of 2 = %d\n", 2, IsPowOfTwoLoop(2));
	printf("%d power of 2 = %d\n", 32, IsPowOfTwoLoop(32));
	printf("%d power of 2 = %d\n", 65, IsPowOfTwoLoop(65));
	printf("%d power of 2 = %d\n", 1024, IsPowOfTwoLoop(1024));
	printf("%d power of 2 = %d\n", 500, IsPowOfTwoLoop(500));
*/	
/*	printf("%d power of 2 = %d\n", 2, IsPowOfTwoNoLoop(2));
	printf("%d power of 2 = %d\n", 32, IsPowOfTwoNoLoop(32));
	printf("%d power of 2 = %d\n", 65, IsPowOfTwoNoLoop(65));
	printf("%d power of 2 = %d\n", 1024, IsPowOfTwoNoLoop(1024));
	printf("%d power of 2 = %d\n", 500, IsPowOfTwoNoLoop(500));
*/
	return 0;
}
