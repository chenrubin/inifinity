#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "barr.h"

#define BYTE 8
#define BITS_IN_ARRAY (sizeof(size_t) * BYTE)
#define BIT_X_SET(X) ((size_t)1 << (X))

#define PRINTTESTRESULTS(func,num, res) \
(printf("Function %s: Test %d %s\n",func, num, (res) == 1 ?\
 "\tpassed" : "\tfailed"))

void TestBArrSetAll();
void TestBArrResetAll();
void TestBArrSetBit();
void TestBArrSetOn();
void TestBArrSetOff();
void TestBArrIsOn();
void TestBArrIsOff();
void TestBArrCountOn();
void TestBArrCountOnLUT();
void TestBArrCountOff();
void TestBArrRotR();
void TestBArrRotL();
void TestBArrMirror();
void TestBArrMirrorLUT();
void TestBArrToString();
void TestBArrFlip();	
	
int main()
{
	TestBArrSetAll();
	TestBArrResetAll();
	TestBArrSetBit();
	TestBArrSetOn();
	TestBArrSetOff();
	TestBArrIsOn();
	TestBArrIsOff();
	TestBArrCountOn();
	TestBArrCountOnLUT();
	TestBArrCountOff();
	TestBArrRotR();
	TestBArrRotL();
	TestBArrMirror();
	TestBArrMirrorLUT();
	TestBArrToString();
	TestBArrFlip();	
	
	return 0;
}

void TestBArrSetAll()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrSetAll", 1, ((pow(2, BITS_IN_ARRAY) - 1) == 
									 ((double)BArrSetAll(num1))));
									 
	PRINTTESTRESULTS("BArrSetAll", 2, ((pow(2, BITS_IN_ARRAY) - 1) == 
									 ((double)BArrSetAll(num2))));
								
	PRINTTESTRESULTS("BArrSetAll", 3, ((pow(2, BITS_IN_ARRAY) - 1) == 
									 ((double)BArrSetAll(num3))));			 
}

void TestBArrResetAll()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrResetAll",1, (BArrResetAll(num1) == 0));
	PRINTTESTRESULTS("BArrResetAll",2, (BArrResetAll(num2) == 0));
	PRINTTESTRESULTS("BArrResetAll",3, (BArrResetAll(num3) == 0));
}

void TestBArrSetBit()
{
	size_t num = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrSetBit",1, ((num | BIT_X_SET(6)) == 
									 BArrSetBit(num, 6, 1)));
	PRINTTESTRESULTS("BArrSetBit",2, ((num | BIT_X_SET(56)) == 
									 BArrSetBit(num, 56, 1)));
	PRINTTESTRESULTS("BArrSetBit",3, ((num & ~BIT_X_SET(62)) == 
									 BArrSetBit(num, 62, 0)));
	PRINTTESTRESULTS("BArrSetBit",4, (num & ~BIT_X_SET(5)) == 
									 BArrSetBit(num, 5, 0));									 				
}

void TestBArrSetOn()
{
	size_t num = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrSetOn",1, 
					(num | BIT_X_SET(6)) == BArrSetOn(num, 6));
	PRINTTESTRESULTS("BArrSetOn",2, 
					(num | BIT_X_SET(56)) == BArrSetOn(num, 56));
	PRINTTESTRESULTS("BArrSetOn",3, 
					(num | BIT_X_SET(62)) == BArrSetOn(num, 62));
	PRINTTESTRESULTS("BArrSetOn",4, 
					(num | BIT_X_SET(4)) == BArrSetOn(num, 4));
}

void TestBArrSetOff()
{
	size_t num = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrSetOff",1,
					(num & ~BIT_X_SET(6)) == BArrSetOff(num, 6));
	PRINTTESTRESULTS("BArrSetOff",2,
					(num & ~BIT_X_SET(56)) == BArrSetOff(num, 56));
	PRINTTESTRESULTS("BArrSetOff",3,
					(num & ~BIT_X_SET(62)) == BArrSetOff(num, 62));
	PRINTTESTRESULTS("BArrSetOff",4,
					(num & ~BIT_X_SET(4)) == BArrSetOff(num, 4));
}

void TestBArrIsOn()
{
	size_t num = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrIsOn",1, BArrIsOn(num, 0));
	PRINTTESTRESULTS("BArrIsOn",2, BArrIsOn(num, 19));
	PRINTTESTRESULTS("BArrIsOn",3, !(BArrIsOn(num, 63)));
	PRINTTESTRESULTS("BArrIsOn",4, !(BArrIsOn(num, 59)));
}

void TestBArrIsOff()
{
	size_t num = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrIsOff",1, !BArrIsOff(num, 0));
	PRINTTESTRESULTS("BArrIsOff",2, !BArrIsOff(num, 19));
	PRINTTESTRESULTS("BArrIsOff",3, BArrIsOff(num, 63));
	PRINTTESTRESULTS("BArrIsOff",4, BArrIsOff(num, 59));
}

void TestBArrCountOn()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrCountOn",1, 64 == BArrCountOn(num1));
	PRINTTESTRESULTS("BArrCountOn",2, 0 == BArrCountOn(num2));
	PRINTTESTRESULTS("BArrCountOn",3, 32 == BArrCountOn(num3));
}

void TestBArrCountOnLUT()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrCountOnLUT",1, 64 == BArrCountOnLUT(num1));
	PRINTTESTRESULTS("BArrCountOnLUT",2, 0 == BArrCountOnLUT(num2));
	PRINTTESTRESULTS("BArrCountOnLUT",3, 32 == BArrCountOnLUT(num3));
}

void TestBArrCountOff()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0x0123456709ABCDEF;
	
	PRINTTESTRESULTS("BArrCountOff",1, 0 == BArrCountOff(num1));
	PRINTTESTRESULTS("BArrCountOff",2, 64 == BArrCountOff(num2));
	PRINTTESTRESULTS("BArrCountOff",3, 33 == BArrCountOff(num3));
}

void TestBArrRotR()
{
	size_t num = 0x8123456789ABCDEF;
	size_t num_rot1 = 0x7BE048D159E26AF3;
	size_t num_rot2 = 0xBCDEF8123456789A;
	size_t num_rot3 = 0x37BE048D159E26AF;
						
	PRINTTESTRESULTS("BArrRotR",1, num_rot1 == BArrRotR(num, 10));
	PRINTTESTRESULTS("BArrRotR",2, num_rot2 == BArrRotR(num, 20));
	PRINTTESTRESULTS("BArrRotR",3, num_rot3 == BArrRotR(num, 78));
}

void TestBArrRotL()
{
	size_t num = 0x8123456789ABCDEF;
	size_t num_rot1 = 0x8D159E26AF37BE04;
	size_t num_rot2 = 0x56789ABCDEF81234;
	size_t num_rot3 = 0xD159E26AF37BE048;
	
	PRINTTESTRESULTS("BArrRotL",1, num_rot1 == BArrRotL(num, 10));
	PRINTTESTRESULTS("BArrRotL",2, num_rot2 == BArrRotL(num, 20));
	PRINTTESTRESULTS("BArrRotL",3, num_rot3 == BArrRotL(num, 78));
}

void TestBArrMirror()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0xF123456789ABCDEF;
	size_t num3_mirror = 0xF7B3D591E6A2C48F;
	
	PRINTTESTRESULTS("BArrMirror",1, num1 == BArrMirror(num1));
	PRINTTESTRESULTS("BArrMirror",2, num2 == BArrMirror(num2));
	PRINTTESTRESULTS("BArrMirror",3, num3_mirror == BArrMirror(num3));
}

void TestBArrMirrorLUT()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0xF123456789ABCDEF;
	size_t num3_mirror = 0xF7B3D591E6A2C48F;
	
	PRINTTESTRESULTS("BArrMirrorLUT",1, num1 == BArrMirrorLUT(num1));
	PRINTTESTRESULTS("BArrMirrorLUT",2, num2 == BArrMirrorLUT(num2));
	PRINTTESTRESULTS("BArrMirrorLUT",3, num3_mirror == BArrMirrorLUT(num3));
}

void TestBArrToString()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0xF123456789ABCDEF;
	char *buf = NULL;
	
	buf = (char *)malloc(BITS_IN_ARRAY + 1);
	if (NULL == buf)
	{
		printf("buffer allocaiton returned NULL\n");
		exit(0);
	}
	
	PRINTTESTRESULTS("BArrToString",1,
	!(strcmp("1111111111111111111111111111111111111111111111111111111111111111", 
	  BArrToString(buf, num1))));
	PRINTTESTRESULTS("BArrToString",2,
	!(strcmp("0000000000000000000000000000000000000000000000000000000000000000",
	  BArrToString(buf, num2)))); 
	PRINTTESTRESULTS("BArrToString",3,
	!(strcmp("1111000100100011010001010110011110001001101010111100110111101111",
	  BArrToString(buf, num3))));
		 
	free(buf);
}

void TestBArrFlip()
{
	size_t num = 0xF123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrFlip",1 ,0xF123456789AACDEF == BArrFlip(num, 16));
	PRINTTESTRESULTS("BArrFlip",2 ,0xF127456789ABCDEF == BArrFlip(num, 50));
	PRINTTESTRESULTS("BArrFlip",3 ,0xF123456F89ABCDEF == BArrFlip(num, 35));
}
