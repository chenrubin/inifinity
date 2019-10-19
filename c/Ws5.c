/****************************
*	Author: ChenR
*	Reviewer: Maoz
*	Ws5 - Files
*
****************************/

#include <stdio.h> /* printf, fopen, fclose */
#include <string.h> /* strcmp, strncmp */
#include <stdlib.h> /* exit */

#define CHAINSIZE 5
#define NUMOFCOMPARE 1
#define MAXBUFSIZE 256

enum error_code
{
	OP_OK			=	0,
	NULL_FILE	 	=	1,
	FILE_NOT_FOUND	=	2
};

/* Last compare - dummy compare that catches everything */
int CmpAll(const char *str_input, const char *str_to_cmp);

/* compare for -exit, -count, -remove */
int MyStrnCmp(const char *str_input, const char *str_to_cmp);

/* Compare for '<' */ 
int MyStrCmp(const char *str_input, const char *str_to_cmp);

/* manages all process */
void Logger(char *argv[]);

/* operational function that remove file */
enum error_code RmvFile(char *str, char *argv[]);

/* operational function that counts lines in file */
enum error_code CntFile(char *str, char *argv[]);

/* operational function that exits the program */
enum error_code exitFile(char *str, char *argv[]);

/* operational function that prepends to a file */
enum error_code BeginFile(char *str, char *argv[]);

/* operational function that appends to a file */
enum error_code AllFile(char *str, char *argv[]);

int main(int argc, char *argv[])
{
	Logger(argv);
	(void)argc;
	
	return 0;
}

void Logger(char *argv[])
{
	int i = 0;
	int oper_flag = 1;
	char buffer[MAXBUFSIZE];
	char tempbuf[MAXBUFSIZE];
	int status = 0;
	
	struct logger_t
	{
		char *buf;
		int (*comp_func)(const char *str_input, const char *str_to_cmp);
		enum error_code (*oper_func)(char *str, char *argv[]);
	};
	
	struct logger_t chain[CHAINSIZE] = 
	{
		{"-remove", MyStrCmp, RmvFile},
		{"-count", MyStrCmp, CntFile},
		{"-exit", MyStrCmp, exitFile},
		{"<", MyStrnCmp, BeginFile},
		{"", CmpAll, AllFile}
	};
	
	if (NULL == argv[1])
	{
		printf("Don't forget to choose a file name\n");
		exit(0);
	}
	
	while (1)
	{
		i = 0;
		oper_flag = 1;
		
		fgets(buffer, MAXBUFSIZE, stdin);
		strcpy(tempbuf, buffer);
		buffer[strcspn(buffer, "\n")] = '\0';
		
		while ((i < CHAINSIZE) && oper_flag)
		{
			if(0 == chain[i].comp_func(buffer, chain[i].buf))
			{
				status = chain[i].oper_func(tempbuf, argv);
				
				if (status)
				{
					printf("status = %d\n", status);
				}
				
				oper_flag = 0;
			}
			
			++i;
		}
	}
}

/* comparison functions */
int CmpAll(const char *str_input, const char *str_to_cmp)
{
	(void)str_input;
	(void)str_to_cmp;
	
	return 0;
}

int MyStrnCmp(const char *str_input, const char *str_to_cmp)
{
	return strncmp(str_input, str_to_cmp, NUMOFCOMPARE);
}

int MyStrCmp(const char *str_input, const char *str_to_cmp)
{
	return strcmp(str_input, str_to_cmp);
}

/* operational functions */
enum error_code RmvFile(char *str, char *argv[])
{	
	if(remove(argv[1]))
	{
		printf("Remove is impossible, check if file exists\n");
		return FILE_NOT_FOUND;
	}
	
	(void)str;
	
	return OP_OK;
}

enum error_code CntFile(char *str, char *argv[])
{
	FILE *fp = fopen(argv[1], "r");
	int count = 0;
	char c = '\0';
	
	if(NULL != fp)
	{
		for (c = getc(fp); c != EOF; c = getc(fp))
		{
			if ('\n' == c)
			{
				++count;
			}
		}
		
		fclose(fp);
		printf("There are %d lines in file\n", count);
		
		return OP_OK;
	}
	else
	{
		printf("opening file for counting returned NULL\n");
		
		return FILE_NOT_FOUND;
	}
	
	(void)str;
}

enum error_code exitFile(char *str, char *argv[])
{
	(void)str;
	(void)argv;
	
	exit(0);
}

enum error_code BeginFile(char *str, char *argv[])
{
	FILE *fp = fopen(argv[1], "a+");
	FILE *temp_file;
	char c = '\0';

	if (NULL != fp)
	{
		temp_file  = fopen("_argv[1]", "a");
		fputs(str + 1, temp_file);
		
		c = fgetc(fp);
		while (c != EOF)
		{
			fputc(c, temp_file);
			c = fgetc(fp);
		}
			
		rename("_argv[1]", argv[1]);
		fclose(fp);
		fclose(temp_file);
		
		return OP_OK;
	}
	else
	{
		printf("opening file for prepending returned NULL\n");
		
		return FILE_NOT_FOUND;
	}
}

enum error_code AllFile(char *str, char *argv[])
{
	FILE *fp = fopen(argv[1], "a");
	
	if (NULL != fp)
	{
		fputs(str, fp);
		fclose(fp);
		
		return OP_OK;
	}
	else
	{
		printf("opening file for appending returned NULL\n");
		
		return NULL_FILE;
	}
}
