#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHAINSIZE 5
#define NUMOFCOMPARE 1
#define MAXBUFSIZE 256

enum error_code
{
	REMOVE_FILE		=	0,
	COUNT_LINES 	=	1,
	EXIT			=	2,
	PREPEND_FILE 	=	3,
	ADD_STRING		=	4,
	NULL_FILE		=	5
};

int CmpAll(const char *str_input, const char *str_to_cmp, size_t n);
int MyStrnCmp(const char *str_input, const char *str_to_cmp, size_t n);
int MyStrCmp(const char *str_input, const char *str_to_cmp, size_t n);
enum error_code RmvFile(char *str, char *argv[]);
enum error_code CntFile(char *str, char *argv[]);
enum error_code exitFile(char *str, char *argv[]);
enum error_code BeginFile(char *str, char *argv[]);
enum error_code AllFile(char *str, char *argv[]);

int main(int argc, char *argv[])
{
	int i = 0;
	int oper_flag = 1;
	char buffer[MAXBUFSIZE];
	char tempbuf[MAXBUFSIZE];
	int status;
/*	enum STATUS {NOT_OPERATED, OPERATED};*/
	
	struct logger_t
	{
		char *buf;
		int (*comp_func)(const char *str_input, const char *str_to_cmp, size_t n);
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

	while (1)
	{
		i = 0;
		oper_flag = 1;
		
		fgets(buffer, MAXBUFSIZE, stdin);
		strcpy(tempbuf, buffer);
		buffer[strcspn(buffer, "\n")] = '\0';
		
		while ((i < 5) && oper_flag)
		{
			if(0 == chain[i].comp_func(buffer, chain[i].buf, strlen(chain[i].buf)))
			{
				status = chain[i].oper_func(tempbuf, argv);
				printf("status = %d\n", status);
				oper_flag = 0;
			}
			
			++i;
		}
	}
	
	return 0;
}

/* Comparison function */
int CmpAll(const char *str_input, const char *str_to_cmp, size_t n)
{
	return 0;
}

int MyStrnCmp(const char *str_input, const char *str_to_cmp, size_t n)
{
	return strncmp(str_input, str_to_cmp, NUMOFCOMPARE);
}

int MyStrCmp(const char *str_input, const char *str_to_cmp, size_t n)
{
	return strcmp(str_input, str_to_cmp);
}

/* operational functions */
enum error_code RmvFile(char *str, char *argv[])
{	
	if(remove(argv[1]))
	{
		printf("Remove is impossible, check if file exists\n");
		return NULL_FILE;
	}
	
	return REMOVE_FILE;
}

enum error_code CntFile(char *str, char *argv[])
{
	FILE *fp;
	int count = 0;
	char c = '\0';
	
	if(fp = fopen(argv[1], "r"))
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
		
		return COUNT_LINES;
	}
	else
	{
		printf("opening file for counting returned NULL\n");
		return NULL_FILE;
	}
}

enum error_code exitFile(char *str, char *argv[])
{
	exit(2);
}

enum error_code BeginFile(char *str, char *argv[])
{
	FILE *fp;
	FILE *temp_file;
	int i = 0;
	char c = '\0';

	if (fp = fopen(argv[1], "r+"))
	{
		temp_file  = fopen("_argv[1]", "a");
		fputs(str + 1, temp_file);
		
		c = fgetc(fp);
		while (c != EOF)
		{
			fputc(c, temp_file);
			c = fgetc(fp);
		}
		
/*			do doesn't work I don't understand why
		{
			c = fgetc(fp);
			printf("c = %c|\n", c);
			fputc(c, temp_file);
		}
		while(c != EOF);
*/			
		rename("_argv[1]", argv[1]);
		fclose(fp);
		fclose(temp_file);
		
		return PREPEND_FILE;
	}
	else
	{
		printf("opening file for begin returned NULL\n");
		return NULL_FILE;
	}
}

enum error_code AllFile(char *str, char *argv[])
{
	FILE *fp;
	
	fp = fopen(argv[1], "a");
	
	if (fp)
	{
		fputs(str, fp);
		fclose(fp);
		
		return ADD_STRING;
	}
	else
	{
		printf("opening file for All returned NULL\n");
		return NULL_FILE;
	}
}
