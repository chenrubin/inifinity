#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHAINSIZE 5
#define NUMOFCOMPARE 1
#define MAXBUFSIZE 256

int CmpAll(const char *str_input, const char *str_to_cmp, size_t n);
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
		int (*comp_func)(const char *str_input, const char *str_to_cmp, size_t n);
		/*enum*/ int (*oper_func)(int res, char *str, char *argv[]);
	};
	
	struct logger_t chain[CHAINSIZE] = 
	{
		{"-remove", strncmp, RmvFile},
		{"-count", strncmp, CntFile},
		{"-exit", strncmp, exitFile},
		{"<", strncmp, BeginFile},
		{"", CmpAll, AllFile}
	};

	while (1)
	{
		i = 0;
		res = 1;
		
		fgets(buffer, MAXBUFSIZE, stdin);
			
		while ((i < 5) && (0 != res))
		{
			res = chain[i].comp_func(buffer, chain[i].buf, strlen(chain[i].buf) - 1);
			chain[i].oper_func(res, buffer, argv);
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

/* operational functions */
int RmvFile(int res, char *str, char *argv[])
{	
	if (0 == res)
	{
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
		printf("There are %d lines in file\n", count);
		
		return 0;
	}
	
	return 1;
}

int exitFile(int res, char *str, char *argv[])
{
	if (0 == res)
	{
		exit(0);
	}
		
	return 1;
}

int BeginFile(int res, char *str, char *argv[])
{
	FILE *fp;

	if (0 == res)
	{
		fp = fopen(argv[1], "r+");
		fseek (fp, 0, SEEK_SET);
		fputs(str + 1, fp);
		fclose(fp);
		
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
		
		return 0;
	}
	
	return 1;
}
