#include <stdio.h> /* printf */
#include <string.h> /*strcmp, strlen, strcpy, strcat, strstr */

#include "recursion.h"
#include "MyUtils.h" /* MAX2,MIN2 */
#include "../../ds/stack/stack.h"
#include "../../ds/slist/slist.h"


void TestIterativeFibonachi();
void TestRecursiveFibonachi();
void TestStrlen();
void TestStrcmp();
void TestStrcpy();
void TestStrcat();
void TestStrstr();
void TestFlip();

int main()
{
	TestIterativeFibonachi();
	TestRecursiveFibonachi();
	TestStrlen();
	TestStrcmp();
	TestStrcpy();
	TestStrcat();
	TestStrstr();
	TestFlip();
	
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

void TestStrcpy()
{
	char str_MyStrcpy[50] = {0};
	char str_strcpy[50] = {0};
	char *str1 = "m";
	char *str2 = "85476)(*";
	char *str3 = "   ";
	
	Strcpy(str_MyStrcpy, str1);
	Strcpy(str_strcpy, str1);
	PRINTTESTRESULTS("TestStrcpy",1, 0 == strcmp(str_strcpy, str_MyStrcpy));
	Strcpy(str_MyStrcpy, str2);
	Strcpy(str_strcpy, str2);
	PRINTTESTRESULTS("TestStrcpy",2, 0 == strcmp(str_strcpy, str_MyStrcpy));
	Strcpy(str_MyStrcpy, str3);
	Strcpy(str_strcpy, str3);
	PRINTTESTRESULTS("TestStrcpy",3, 0 == strcmp(str_strcpy, str_MyStrcpy));
}

void TestStrcat()
{
	char str_MyStrcpy[50] = "Moshe";
	char str_strcpy[50] = "Moshe";
	char *str1 = "85476)(*";
	char *str2 = "Maoz Can't write CV";
	
	Strcat(str_MyStrcpy, str1);
	strcat(str_strcpy, str1);
	PRINTTESTRESULTS("TestStrcat",1, 0 == strcmp(str_strcpy, str_MyStrcpy));
	Strcat(str_MyStrcpy, str2);
	strcat(str_strcpy, str2);
	PRINTTESTRESULTS("TestStrcat",2, 0 == strcmp(str_strcpy, str_MyStrcpy));
}

void TestStrstr()
{
	char *str1 = "Maoz dum is dumb";
	char *str2 = "dumb";
	char *str3 = "abcdabcd";
	char *str4 = "cd";
	char *res1 = NULL;
	char *res2 = NULL;
	
	res1 = Strstr(str1, str2);
	res2 = strstr(str1, str2);
	PRINTTESTRESULTS("TestStrstr",1, res1 == res2);
	res1 = Strstr(str3, str4);
	res2 = strstr(str3, str4);
	PRINTTESTRESULTS("TestStrstr",2, res1 == res2);
}

void TestFlip()
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
	
	sl_node_t *head_after_flip = Flip(first_node);

	PRINTTESTRESULTS("TestFlip",1, 5 == *(int *)head_after_flip -> data);
	PRINTTESTRESULTS("TestFlip",2, head_after_flip == fifth_node);
	PRINTTESTRESULTS("TestFlip",3, 4 == *(int *)head_after_flip -> next -> data);
	PRINTTESTRESULTS("TestFlip",4, 3 == *(int *)head_after_flip -> next -> next -> data);
	PRINTTESTRESULTS("TestFlip",5, 2 == *(int *)head_after_flip -> next -> next -> next -> data);
	PRINTTESTRESULTS("TestFlip",6, 1 == *(int *)head_after_flip -> next -> next -> next -> next -> data);
	
	SListFreeAll(head_after_flip);
}
