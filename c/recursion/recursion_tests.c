#include <stdio.h> /* printf */
#include <string.h> /*RecStrcmp, RecStrlen, RecStrcpy, RecStrcat, RecStrstr */
#include <strings.h> /*RecStrcmp, RecStrlen, RecStrcpy, RecStrcat, RecStrstr */

#include "recursion.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */
#include "../../ds/stack/stack.h"
#include "../../ds/slist/slist.h"


void TestIterativeFibonachi();
void TestRecursiveFibonachi();
void TestRecStrlen();
void TestRecStrcmp();
void TestRecStrcpy();
void TestRecStrcat();
void TestRecStrstr();
void TestRecFlip();
void TestSortStack();

int MyCompareFunc(void *data1, void *data2);

int main()
{
	TestIterativeFibonachi();
	TestRecursiveFibonachi();
	TestRecStrlen();
	TestRecStrcmp();
	TestRecStrcpy();
	TestRecStrcat();
	TestRecStrstr();
	TestRecFlip();
	TestSortStack();
	
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
	PRINTTESTRESULTS("TestRecursiveFibonachi",1, 0 == RecursiveFibonacci(0));
	PRINTTESTRESULTS("TestRecursiveFibonachi",2, 1 == RecursiveFibonacci(1));
	PRINTTESTRESULTS("TestRecursiveFibonachi",3, 1 == RecursiveFibonacci(2));
	PRINTTESTRESULTS("TestRecursiveFibonachi",4, 610 == RecursiveFibonacci(15));
	PRINTTESTRESULTS("TestRecursiveFibonachi",5, 2178309  == RecursiveFibonacci(32));
	PRINTTESTRESULTS("TestRecursiveFibonachi",6, 55 == RecursiveFibonacci(10));
}

void TestRecStrlen()
{
	char *str1 = "moshe and I went shopping";
	char *str2 = "";
	
	PRINTTESTRESULTS("TestRecStrlen",1, RecStrlen(str1) == RecStrlen(str1));
	PRINTTESTRESULTS("TestRecStrlen",2, RecStrlen(str2) == RecStrlen(str2));
}

void TestRecStrcmp()
{
	char *str1 = "moshe and I went shopping";
	char *str2 = "moshe and I went shopping";
	char *str3 = "moshe and I went shopping";
	char *str4 = "moshe and I went shXpping";
	char *str5 = "x";
	char *str6 = "a";
	char *str7 = "aaaaak";
	char *str8 = "aaaaam";
	
	PRINTTESTRESULTS("TestRecStrcmp",1, RecStrcmp(str1, str2) == RecStrcmp(str1, str2));
	PRINTTESTRESULTS("TestRecStrcmp",2, RecStrcmp(str3, str4) == RecStrcmp(str3, str4));
	PRINTTESTRESULTS("TestRecStrcmp",3, RecStrcmp(str5, str6) == RecStrcmp(str5, str6));
	PRINTTESTRESULTS("TestRecStrcmp",4, RecStrcmp(str7, str8) == RecStrcmp(str7, str8));
}

void TestRecStrcpy()
{
	char str_MyRecStrcpy[50] = {0};
	char str_RecStrcpy[50] = {0};
	char *str1 = "m";
	char *str2 = "85476)(*";
	char *str3 = "   ";
	
	RecStrcpy(str_MyRecStrcpy, str1);
	RecStrcpy(str_RecStrcpy, str1);
	PRINTTESTRESULTS("TestRecStrcpy",1, 0 == RecStrcmp(str_RecStrcpy, str_MyRecStrcpy));
	RecStrcpy(str_MyRecStrcpy, str2);
	RecStrcpy(str_RecStrcpy, str2);
	PRINTTESTRESULTS("TestRecStrcpy",2, 0 == RecStrcmp(str_RecStrcpy, str_MyRecStrcpy));
	RecStrcpy(str_MyRecStrcpy, str3);
	RecStrcpy(str_RecStrcpy, str3);
	PRINTTESTRESULTS("TestRecStrcpy",3, 0 == RecStrcmp(str_RecStrcpy, str_MyRecStrcpy));
}

void TestRecStrcat()
{
	char str_MyRecStrcpy[50] = "Moshe";
	char str_RecStrcpy[50] = "Moshe";
	char *str1 = "85476)(*";
	char *str2 = "Maoz Can't write CV";
	
	RecStrcat(str_MyRecStrcpy, str1);
	RecStrcat(str_RecStrcpy, str1);
	PRINTTESTRESULTS("TestRecStrcat",1, 0 == RecStrcmp(str_RecStrcpy, str_MyRecStrcpy));
	RecStrcat(str_MyRecStrcpy, str2);
	RecStrcat(str_RecStrcpy, str2);
	PRINTTESTRESULTS("TestRecStrcat",2, 0 == RecStrcmp(str_RecStrcpy, str_MyRecStrcpy));
}

void TestRecStrstr()
{
	char *str1 = "Maoz dum is dumb";
	char *str2 = "dumb";
	char *str3 = "abcdabcd";
	char *str4 = "cd";
	char *res1 = NULL;
	char *res2 = NULL;
	
	res1 = RecStrstr(str1, str2);
	res2 = RecStrstr(str1, str2);
	PRINTTESTRESULTS("TestRecStrstr",1, res1 == res2);
	res1 = RecStrstr(str3, str4);
	res2 = RecStrstr(str3, str4);
	PRINTTESTRESULTS("TestRecStrstr",2, res1 == res2);
}

void TestRecFlip()
{
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int data4 = 4;
	int data5 = 5;
	sl_node_t *fifth_node = SListCreateNode(&data5, NULL);
	sl_node_t *fourth_node = SListCreateNode(&data4, fifth_node);
	sl_node_t *third_node = SListCreateNode(&data3, fourth_node);
	sl_node_t *second_node = SListCreateNode(&data2, third_node);
	sl_node_t *first_node = SListCreateNode(&data1, second_node);
	
	sl_node_t *head_after_RecFlip = RecFlip(first_node);

	PRINTTESTRESULTS("TestRecFlip",1, 5 == *(int *)head_after_RecFlip -> data);
	PRINTTESTRESULTS("TestRecFlip",2, head_after_RecFlip == fifth_node);
	PRINTTESTRESULTS("TestRecFlip",3, 4 == *(int *)head_after_RecFlip -> next -> data);
	PRINTTESTRESULTS("TestRecFlip",4, 3 == *(int *)head_after_RecFlip -> next -> next -> data);
	PRINTTESTRESULTS("TestRecFlip",5, 2 == *(int *)head_after_RecFlip -> next -> next -> next -> data);
	PRINTTESTRESULTS("TestRecFlip",6, 1 == *(int *)head_after_RecFlip -> next -> next -> next -> next -> data);
	
	SListFreeAll(head_after_RecFlip);
}

void TestSortStack()
{
	int data1 = 1;
	int data2 = 10;
	int data3 = -8;
	int data4 = 5;
	int data5 = 0;
	stack_t *new_stack = StackCreate(5, sizeof(int));
	
	StackPush(new_stack, &data1);
	StackPush(new_stack, &data2);
	StackPush(new_stack, &data3);
	StackPush(new_stack, &data4);
	StackPush(new_stack, &data5);
	
	new_stack = RecSortStack(new_stack, 4, MyCompareFunc);
	
	PRINTTESTRESULTS("TestSortStack",1, 10 == *(int *)StackPeek(new_stack));
	StackPop(new_stack);
	PRINTTESTRESULTS("TestSortStack",2, 5 == *(int *)StackPeek(new_stack));
	StackPop(new_stack);
	PRINTTESTRESULTS("TestSortStack",3, 1 == *(int *)StackPeek(new_stack));
	StackPop(new_stack);
	PRINTTESTRESULTS("TestSortStack",4, 0 == *(int *)StackPeek(new_stack));
	StackPop(new_stack);
	PRINTTESTRESULTS("TestSortStack",5, -8 == *(int *)StackPeek(new_stack));
	StackPop(new_stack);
	
	StackDestroy(new_stack);
}

int MyCompareFunc(void *data1, void *data2)
{
	if (*(int *)data1 < *(int *)data2)
	{
		return 1;
	}
	else if (*(int *)data1 == *(int *)data2)
	{
		return 0;
	}
	
	return -1;
}
