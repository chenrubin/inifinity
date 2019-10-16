#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHAINSIZE 5
#define NUMOFCOMPARE 1
#define MAXBUFSIZE 256

int CmpRmv(char *str_input, char *str_to_cmp);
int CmpCnt(char *str_input, char *str_to_cmp);
int CmpExit(char *str_input, char *str_to_cmp);
int CmpFileBegin(char *str_input, char *str_to_cmp);
int CmpAll(char *str_input, char *str_to_cmp);
int RmvFile(int res, char *str, char *argv[]);
int CntFile(int res, char *str, char *argv[]);
int exitFile(int res, char *str, char *argv[]);
int BeginFile(int res, char *str, char *argv[]);
int AllFile(int res, char *str, char *argv[]);

int main(int argc, char *argv[])
{
	int i = 0;
	int res = 1;
	char buffer[MAXBUFSIZE];
/*	enum STATUS {NOT_OPERATED, OPERATED};*/
	
	struct logger_t
	{
		char *buf;
		int (*comp_func)(char *str_input, char *str_to_cmp);
		/*enum*/ int (*oper_func)(int res, char *str, char *argv[]);
	};
	
	struct logger_t chain[CHAINSIZE] = 
	{
		{"-remove", CmpRmv, RmvFile},
		{"-count", CmpCnt, CntFile},
		{"-exit", CmpExit, exitFile},
		{"<", CmpFileBegin, BeginFile},
		{"", CmpAll, AllFile}
	};

	while (1)
	{
		i = 0;
		res = 1;
		printf("res = %d and i = %d\n", res, i);
		printf("Please select string\n");
		fgets(buffer, MAXBUFSIZE, stdin);
			
		while ((i < 5) && (0 != res))
		{
			res = chain[i].comp_func(buffer, chain[i].buf);
			printf("res = %d\n", res);
			chain[i].oper_func(res, buffer, argv);
			++i;
		}
	}
	
	return 0;
}

/* Comparison functions */
int CmpRmv(char *str_input, char *str_to_cmp)
{
	printf("compare remove %s with %s\n", str_input, str_to_cmp);
	return (strncmp(str_input, str_to_cmp, strlen(str_input) - 1));
}

int CmpCnt(char *str_input, char *str_to_cmp)
{
	printf("compare count %s with %s\n", str_input, str_to_cmp);
	return (strncmp(str_input, str_to_cmp, strlen(str_input) - 1));
}

int CmpExit(char *str_input, char *str_to_cmp)
{
	printf("compare exit %s with %s\n", str_input, str_to_cmp);
	printf("input = %s, comp = %s\n", str_input, str_to_cmp);
	return (strncmp(str_input, str_to_cmp, strlen(str_input) - 1));
}

int CmpFileBegin(char *str_input, char *str_to_cmp)
{
	printf("compare begin %s with %s\n", str_input, str_to_cmp);
	
	return (strncmp(str_input, str_to_cmp, NUMOFCOMPARE));
}

int CmpAll(char *str_input, char *str_to_cmp)
{
	return 0;
}

/* operational functions */
int RmvFile(int res, char *str, char *argv[])
{	
	if (0 == res)
	{
		printf("remove file\n");
		remove(argv[1]);
		return 0;
	}
	
	return 1;
}

int CntFile(int res, char *str, char *argv[])
{
	FILE *fp; 
	int count = 0;
	char c = '\0';
	
	if (0 == res)
	{
		fp = fopen(argv[1], "r");
		
		for (c = getc(fp); c != EOF; c = getc(fp))
		{
			if ('\n' == c)
			{
				++count;
			}
		}
		
		fclose(fp);
		printf("There are %d lines in the file\n", count);
		return 0;
	}
	
	return 1;
}

int exitFile(int res, char *str, char *argv[])
{
	printf("inside exit oper --> res = %d\n", res);
	if (0 == res)
	{
		exit(0);
	}
	
	printf("exit file\n");
	
	return 0;
}

int BeginFile(int res, char *str, char *argv[])
{
	FILE *fp;
	
	if (0 == res)
	{
		fp = fopen(argv[1], "a");
		fseek (fp, 0, SEEK_SET);
		fputs(str, fp);
		fclose(fp);
		printf("begin file");
		
		return 0;
	}
	
	return 1;
}

int AllFile(int res, char *str, char *argv[])
{
	FILE *fp;
	
	if (0 == res)
	{
		fp = fopen(argv[1], "a");
		fputs(str, fp);
		fclose(fp);
		printf("all file\n");
		
		return 0;
	}
	
	return 1;
}
