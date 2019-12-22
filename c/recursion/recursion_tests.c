#include <stdio.h> /* printf */
#include <string.h> /*strcmp, strlen, strcpy, strcat, strstr */

#include "recursion.h"
#include "MyUtils.h" /* MAX2,MIN2 */


void TestIterativeFibonachi();
void TestRecursiveFibonachi();
void TestStrlen();
void TestStrcmp();

int main()
{
	TestIterativeFibonachi();
	TestRecursiveFibonachi();
	TestStrlen();
	TestStrcmp();	
	
	return 0;
}

void TestIterativeFibonachi()	
{
	PRINTTESTRESULTS("TestFibonachi",1, 0 == IterativeFibonacci(0));
	PRINTTESTRESULTS("TestFibonachi",2, 1 == IterativeFibonacci(1));
	PRINTTESTRESULTS("TestFibonachi",3, 1 == IterativeFibonacci(2));
	PRINTTESTRESULTS("TestFibonachi",4, 610 == IterativeFibonacci(15));
	PRINTTESTRESULTS("TestFibonchi",5, 2178309  == IterativeFibonacci(32));
	PRINTTESTRESULTS("TestFibonachi",6, 55 == IterativeFibonacci(10));
}

void TestRecursiveFibonachi()
{
	PRINTTESTRESULTS("RecursiveFibonacci",1, 0 == RecursiveFibonacci(0));
	PRINTTESTRESULTS("RecursiveFibonacci",2, 1 == RecursiveFibonacci(1));
	PRINTTESTRESULTS("RecursiveFibonacci",3, 1 == RecursiveFibonacci(2));
	PRINTTESTRESULTS("RecursiveFibonacci",4, 610 == RecursiveFibonacci(15));
	PRINTTESTRESULTS("RecursiveFibonacci",5, 2178309  == RecursiveFibonacci(32));
	PRINTTESTRESULTS("RecursiveFibonacci",6, 55 == RecursiveFibonacci(10));
}

void TestStrlen()
{
	char *str1 = "moshe and I went shopping";
	char *str2 = "";
	
	PRINTTESTRESULTS("TestStrlen",1, strlen(str1) == Strlen(str1));
	PRINTTESTRESULTS("TestStrlen",2, strlen(str2) == Strlen(str2));
}

void TestStrcmp()
{
	char *str1 = "moshe and I went shopping";
	char *str2 = "moshe and I went shopping";
	char *str3 = "moshe and I went shopping";
	char *str4 = "moshe and I went shXpping";
	char *str5 = "x";
	char *str6 = "a";
	char *str7 = "aaaaak";
	char *str8 = "aaaaam";
	
	PRINTTESTRESULTS("TestStrcmp",1, strcmp(str1, str2) == Strcmp(str1, str2));
	PRINTTESTRESULTS("TestStrcmp",2, strcmp(str3, str4) == Strcmp(str3, str4));
	PRINTTESTRESULTS("TestStrcmp",3, strcmp(str5, str6) == Strcmp(str5, str6));
	PRINTTESTRESULTS("TestStrcmp",4, strcmp(str7, str8) == Strcmp(str7, str8));
}
