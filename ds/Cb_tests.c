#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "cbuff.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(func,num, res) \
(printf("%-55s: Test %d %s\n" KNRM,func, num, (res) == 1 ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))

void TestsCbuffWrite();
void TestsCbuffRead();
void TestsCbuffReadWrite();
void TestsCBuffIsEmptySpaceLeft();

struct CB
{
    size_t capacity;
    char *write_ptr;
    char *read_ptr;
    char begin[1];
};

int main()
{
	TestsCbuffWrite();
	TestsCbuffRead();
	TestsCbuffReadWrite();
	TestsCBuffIsEmptySpaceLeft();		
	
	return 0;
}

void TestsCbuffWrite()
{	
	cbuff_t *new_CB = NULL;
	
	char *buf_to_read_from = "Hello moshe, my name is chen";
	
	printf("Create CB\n");
	new_CB = CBuffCreate(10);
	PRINTTESTRESULTS("TestsCbuffWrite_Write",1, 
	10 == CBuffWrite(new_CB, buf_to_read_from, 27));
	printf("Destroy CB\n");
	CBuffDestroy(new_CB);
	
	printf("Create CB\n");
	new_CB = CBuffCreate(10);
	PRINTTESTRESULTS("TestsCbuffWrite_Write",2, 
	9 == CBuffWrite(new_CB, buf_to_read_from, 9));
	printf("Destroy CB\n");
	CBuffDestroy(new_CB);
	
	printf("Create CB\n");
	new_CB = CBuffCreate(10);
	PRINTTESTRESULTS("TestsCbuffWrite_Write",3, 
	2 == CBuffWrite(new_CB, buf_to_read_from, 2));
	PRINTTESTRESULTS("TestsCbuffWrite_Write",4, 
	3 == CBuffWrite(new_CB, buf_to_read_from, 3));
	PRINTTESTRESULTS("TestsCbuffWrite_Write",5, 
	5 == CBuffWrite(new_CB, buf_to_read_from, 5));
	PRINTTESTRESULTS("TestsCbuffWrite_Write",6, 
	0 == CBuffWrite(new_CB, buf_to_read_from, 1));
	printf("Destroy CB\n");
	CBuffDestroy(new_CB);
	
	printf("\n\n");
}

void TestsCbuffRead()
{	
	cbuff_t *new_CB = CBuffCreate(25);
	
	char *buf_to_read_from = "Hello moshe, my name is chen";
	char buf_to_write_to[50];

	PRINTTESTRESULTS("TestsCbuffRead_Write",1, 
	25 == CBuffWrite(new_CB, buf_to_read_from, 27));
	
	PRINTTESTRESULTS("TestsCbuffRead_Read",2, 
	2 == CBuffRead(new_CB, buf_to_write_to, 2));
	buf_to_write_to[2] = '\0';
	PRINTTESTRESULTS("TestsCbuffRead_compareReadString",3,
	0 == strcmp(buf_to_write_to, "He"));
	
	PRINTTESTRESULTS("TestsCbuffRead_Read",4, 
	10 == CBuffRead(new_CB, buf_to_write_to, 10));
	buf_to_write_to[10] = '\0';
	PRINTTESTRESULTS("TestsCbuffRead_compareReadString",5, 
	0 == strcmp(buf_to_write_to, "llo moshe,"));
	
	PRINTTESTRESULTS("TestsCbuffRead_Read",6, 
	13 == CBuffRead(new_CB, buf_to_write_to, 20));
	buf_to_write_to[13] = '\0';
	PRINTTESTRESULTS("TestsCbuffRead_compareReadString",7,
	0 == strcmp(buf_to_write_to, " my name is c"));
	
	PRINTTESTRESULTS("TestsCbuffRead_Read",8, 
	0 == CBuffRead(new_CB, buf_to_write_to, 20));
	buf_to_write_to[0] = '\0';
	PRINTTESTRESULTS("TestsCbuffRead_compareReadString",9,
	0 == strcmp(buf_to_write_to, ""));
	

	printf("Destroy queue\n");
	CBuffDestroy(new_CB);
	
	printf("\n\n");
}

void TestsCbuffReadWrite()
{	
	cbuff_t *new_CB = CBuffCreate(25);
	
	char *buf_to_read_from = "Hello moshe, my name is chen";
	char buf_to_write_to[50];

	PRINTTESTRESULTS("TestsCbuffReadWrite_Write",1, 
	10 == CBuffWrite(new_CB, buf_to_read_from, 10));
	
	PRINTTESTRESULTS("TestsCbuffReadWrite_Read",2, 
	2 == CBuffRead(new_CB, buf_to_write_to, 2));
	buf_to_write_to[2] = '\0';
	PRINTTESTRESULTS("TestsCbuffReadWrite_compareReadString",3,
	0 == strcmp(buf_to_write_to, "He"));
	
	PRINTTESTRESULTS("TestsCbuffReadWrite_Read",4, 
	5 == CBuffRead(new_CB, buf_to_write_to, 5));
	buf_to_write_to[5] = '\0';
	PRINTTESTRESULTS("TestsCbuffReadWrite_compareReadString",5,
	0 == strcmp(buf_to_write_to, "llo m"));
	
	PRINTTESTRESULTS("TestsCbuffReadWrite_Write",6, 
	22 == CBuffWrite(new_CB, buf_to_read_from, 22));

	PRINTTESTRESULTS("TestsCbuffReadWrite_Read",7, 
	25 == CBuffRead(new_CB, buf_to_write_to, 30));
	buf_to_write_to[25] = '\0';
	PRINTTESTRESULTS("TestsCbuffReadWrite_compareReadString",8,
	0 == strcmp(buf_to_write_to, "oshHello moshe, my name i"));
	
	PRINTTESTRESULTS("TestsCbuffReadWrite_Write",9, 
	10 == CBuffWrite(new_CB, buf_to_read_from, 10));
	PRINTTESTRESULTS("TestsCbuffReadWrite_Write",10, 
	10 == CBuffWrite(new_CB, buf_to_read_from, 10));
	PRINTTESTRESULTS("TestsCbuffReadWrite_Read",11, 
	20 == CBuffRead(new_CB, buf_to_write_to, 30));
	buf_to_write_to[20] = '\0';
	PRINTTESTRESULTS("TestsCbuffReadWrite_compareReadString",12,
	0 == strcmp(buf_to_write_to, "Hello moshHello mosh"));

	printf("Destroy queue\n");
	CBuffDestroy(new_CB);
	
	printf("\n\n");
}

void TestsCBuffIsEmptySpaceLeft()
{
	cbuff_t *new_CB = CBuffCreate(25);
	
	char *buf_to_read_from = "Hello moshe, my name is chen";
	char buf_to_write_to[50];
	
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_IsEmpty",1,
	1 == CBuffIsEmpty(new_CB));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_SpaceLeft",2,
	25 == CBuffSpaceLeft(new_CB));

	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_Write",3,
	10 == CBuffWrite(new_CB, buf_to_read_from, 10));
	
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_IsEmpty",4,
	0 == CBuffIsEmpty(new_CB));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_SpaceLeft",5,
	15 == CBuffSpaceLeft(new_CB));
	
	PRINTTESTRESULTS("TestsCbuffRead_Read",6, 
	7 == CBuffRead(new_CB, buf_to_write_to, 7));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_IsEmpty",7,
	0 == CBuffIsEmpty(new_CB));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_SpaceLeft",8,
	22 == CBuffSpaceLeft(new_CB));
	
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_Write",9,
	10 == CBuffWrite(new_CB, buf_to_read_from, 10));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_IsEmpty",10,
	0 == CBuffIsEmpty(new_CB));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_SpaceLeft",11,
	12 == CBuffSpaceLeft(new_CB));
	
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_Write",12,
	7 == CBuffWrite(new_CB, buf_to_read_from, 7));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_IsEmpty",13,
	0 == CBuffIsEmpty(new_CB));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_SpaceLeft",14,
	5 == CBuffSpaceLeft(new_CB));
	
	PRINTTESTRESULTS("TestsCbuffRead_Read",15, 
	11 == CBuffRead(new_CB, buf_to_write_to, 11));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_IsEmpty",16,
	0 == CBuffIsEmpty(new_CB));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_SpaceLeft",17,
	16 == CBuffSpaceLeft(new_CB));
	
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_Write",18,
	16 == CBuffWrite(new_CB, buf_to_read_from, 30));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_IsEmpty",19,
	0 == CBuffIsEmpty(new_CB));
	PRINTTESTRESULTS("TestsCBuffIsEmptySpaceLeft_SpaceLeft",20,
	0 == CBuffSpaceLeft(new_CB));
	
	printf("Destroy queue\n");
	CBuffDestroy(new_CB);	
}
