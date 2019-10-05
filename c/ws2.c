#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */
#include <string.h> /* for all original string functions */
#include <strings.h> /* for strcasecmp */
#include <assert.h> /* for assert */
#include <ctype.h> /* for isupper and islower */

int Ispalindrom(const char *s);
void Sevenboom(int from, int to);
void SwapPtr(int **p1, int **p2);
void RmSpaces(char *str);
char *SumNum(char *str1, char *str2);
size_t Mystrspn(const char *str1, const char *str2);
void TestIspalindrom();
void TestSevenboom();
void TestSwapptr();
void TestRmspaces();
void TestSumNum();
void TestMystrspn();
void TestMystrtok();


int Ispalindrom(const char *s)
{
	char *end = (char *)s;
	
	assert(NULL != s);
	
	while ('\0' != *end) /* finding the end of the string */
	{
		++end;
	}
	
	--end; /* start form last char of string not including '\0' */
	
	while (s <= end) /* going over the string one char at a time */
	{
		/* 
		* Do not account for certain chars.
		*/
		while ((' ' == *s) || (',' == *s) || ('.' == *s) ||
			   (' ' == *end) || (',' == *end) || ('.' == *end))
		{
			if ((' ' == *s) || (',' == *s) || ('.' == *s))
			{
				s++;
			}	
			if ((' ' == *end) || (',' == *end) || ('.' == *end))
			{
				--end;
			}
		} /* end of while */
		
		/* 
		* When dealing with letters ignore case related
		*/
		if ((('A' <= *s && 'Z' >= *s) || ('a' <= *s && 'z' >= *s)) &&
			(('A' <= *end && 'Z' >= *end) || ('a' <= *end && 'z' >= *end)))
			{
				/* Same letter ,one capital and one not, or both the same */
				if ((32 == *s - *end) || (32 == *end - *s) || (*s == *end))
				{
					++s;
					--end;
				}
				else
				{
					return 1;
				}
			}
		else /* when at least one of them is not a letter */
		{
			if (*s != *end)
			{
				return 1;
			}
			else
			{
				s++;
				--end;
			}
		}	
	} /* end of while */
	
	return 0;
}

void Sevenboom(int from, int to)
{
	int i;
	int tmpindex;
	int boomflag;
	
	for (i = from ; i <= to ; ++i)
	{
		tmpindex = i;
		boomflag = 0;
		
		if (0 == (tmpindex % 7)) /* number is divided by 7 without remainder*/
		{
			printf("BOOM, ");
		}
		else
		{
			while (0 != tmpindex) /* checking for appearance of number 7*/
			{
				if ((7 == (tmpindex % 10)) || (-7 == (tmpindex % 10)))
				{
					printf("BOOM, ");
					boomflag = 1; /* BOOM was printed */
					tmpindex = 0; /* in order to stop the loop */
				}
				else
				{
					tmpindex = tmpindex / 10;
				}
			} /* end of while */
			
			if (boomflag == 0) /* in case no 7 was found */
			{
				printf("%d, ", i);
			}
		} /* end of else */
	} /* end of for */
}

void Swapptr(int **p1, int **p2)
{
	int *p;
	int **tmp_addr = &p;
	
	assert(NULL != p1);
	assert(NULL != p2);
	
	*tmp_addr = *p1;
	*p1 = *p2;
	*p2 = *tmp_addr;
}

void Rmspaces(char *str)
{
	char *read = str;
	char *write = str;
	
	while (isspace(*read) != 0) /* there are spaces */
	{
		++read;
	}
	
	while ('\0' != *read)
	{
		if ((isspace(*read) == 0) || /* no spaces */
			((isspace(*read) != 0) && (isspace(*(write-1)) == 0))) /* there is a space but it is the first */
		{
			*write = *read;
			++read;
			++write;
		}
		else
		{
			++read;
		}
	}
	
	if (isspace(*(write - 1)) == 0)
	{
		*write = '\0';
	}
	else
	{
		*(write - 1) = '\0';
	}
}

char *SumNum(char *str1, char *str2)
{
	int tmp = 0; /* calculating before putting in result string */
	int mem = 0; /* when calculation exceeds 9 need to remeber 1 */
	int max = 0; /* calculate max size between strings */
	char *str1end = str1;
	char *str2end = str2;
	char *res_str = NULL;
	char *res_str_final = NULL;
	char *res_str_end = NULL;
	int is_str1_finished = 0;
	int is_str2_finished = 0;
	char buf[2];
	int counter = 0;
	
	assert(NULL != str1);
	assert(NULL != str2);
	
	while ('\0' != *str1end) /* moving str1end to the end */
	{
		++str1end;
	}
	
	while ('\0' != *str2end) /* moving str2end to the end */
	{
		++str2end;
	}
	
	/* creating a new string with length of max(len(str1),len(str2)) + 2 */
	if ((str1end - str1) > (str2end - str2))
	{
		max = str1end - str1;
	}
	else
	{
		max = str2end - str2;
	}

	res_str = (char *)calloc(max + 2, sizeof(char));
	res_str_end = res_str + max + 1;
	*res_str_end = '\0';
	--res_str_end;
	
	while ((str1end != str1) || (str2end != str2))
	{				
		++counter;
		
		if (is_str1_finished)
		{
			--str2end;
		}
		else if (is_str2_finished)
		{
			--str1end;
		}
		else
		{
			--str2end;
			--str1end;
		}
		
		if (is_str2_finished) /* str1 is now pointing to cell str1[1] */
		{
			tmp = *str1end- 48 + mem;
		}
		else if (is_str1_finished) /* str2 is now ointing to cell strr2[1] */
		{
			tmp = *str2end - 48 + mem;
		}
		else /* both are left */
		{
			tmp = *str1end + *str2end - 96 + mem;
		}
		
		if (10 > tmp)
		{
			sprintf(buf, "%d", tmp);
			*res_str_end = *buf;
			mem = 0;
		}
		else
		{
			sprintf(buf, "%d", tmp % 10);
			*res_str_end = *buf;
			mem = tmp / 10;
		}
		--res_str_end;
		
		if (str1end == str1) /* finished or not */
		{
			is_str1_finished = 1;
		}
		if (str2end == str2)
		{
			is_str2_finished = 1;
		}
	}
	
	if (mem > 0)
	{
		sprintf(buf, "%d", mem);
		*res_str_end = *buf;
		return res_str_end;
	}
	else
	{
		++res_str_end;
		res_str_final = (char *)calloc(strlen(res_str_end) + 1, sizeof(char));
		res_str_final = strcpy(res_str_final, res_str_end);
		--res_str_end;
		free(res_str_end);
		return res_str_final;
	}
}

size_t Mystrspn(const char *str1, const char *str2)
{
	int counter = 0;
	const char *str2_run = str2;
	
	assert(NULL != str1);
	assert(NULL != str2);
	
	while ('\0' != *str1) /* runs over str1 */
	{
		while ((*str1 != *str2_run) && ('\0' != *str2_run)) /* runs over str2 */
		{
			++str2_run;
		}
		
		if ('\0' == *str2_run)
		{
			return counter;
		}
		else
		{
			++counter;
			++str1;
			str2_run = str2;			
		}
	}
	
	return counter;
}

char *Mystrtok(char *str, const char *delim)
{
	static char *pointer = NULL;
	char const *delim_run = delim;
	char *res = NULL;
	char *run = NULL;
	size_t counter = 0;
	
	assert(delim != NULL);
	
	if (NULL != str) /* first time of calling this function */
	{
		pointer = str;
	}
	
	if ('\0' == *pointer)
	{
		return NULL;
	}
	else
	{
		run = pointer;
	}
	
	counter = strspn(run, delim);
	run = run + counter;
	
	if ('\0' == *run)
	{
		res = NULL;
	}
	else
	{
		res = run;
	}
	
	while ('\0' != *run)
	{	
		while ((*delim_run != *run) && ('\0' != *delim_run)) /* find delimiter */
		{
			++delim_run;
		}
		
		if (*delim_run == *run) /* delim was found */
		{
			*run = '\0';
			pointer = run + 1;
			return res;
		}
		else /* delim was not found */
		{
			++run;
		}
		
		delim_run = delim;
	}
	
	pointer = run;	
	return res; 
}

void TestIspalindrom()
{
	int res = 0;
	char str1[60] = "This is a Polin*drommord*niloP  AsiSIht";
	char str2[60] = "This is not a polindrom";
	
	printf("/************************************************************\n");
	printf("	Testing Ispalindrom						 \n");
	printf("************************************************************/\n\n");
	
	
	res = Ispalindrom(str1);
	
	if (res == 0)
	{
		printf("PASS - str %s is a palindrom\n", str1);
	}
	else
	{
		printf("FAILED - str %s is a palindrom\n", str1);
	}
	
	res = Ispalindrom(str2);
	
	if (res == 1)
	{
		printf("PASS - str %s is not a palindrom\n", str2);
	}
	else
	{
		printf("FAILED - str %s is not a palindrom\n", str2);
	}
}

void TestSevenboom()
{
	int start = -90;
	int end = 49;
	
	printf("/************************************************************\n");
	printf("	Testing Sevenboom						 \n");
	printf("************************************************************/\n\n");
	
	Sevenboom(start, end);
	printf("Need to manually look at the print out and verifyn\n\n");
}

void TestSwapptr()
{
	int x = 5;
	int y = 10;
	int *p1 = &x;
	int *p2 = &y;
	char buf1[20];
	char buf2[20];	
	
	printf("/************************************************************\n");
	printf("	Testing Swapptr						 \n");
	printf("************************************************************/\n\n");
	
	Swapptr(&p1, &p2);
	
	sprintf(buf1, "%p", (void *)p1);
	sprintf(buf2, "%p", (void *)p2);
	printf("*p1 = %d\n", *p1);
	printf("y = %d\n", y);
	printf("*p2 = %d\n", *p2);
	printf("x = %d\n", x);
	printf("*p1 == y --> %d\n", *p1 == y);
	printf("*p2 == x --> %d\n", *p2 == x);
	printf("strcmp(buf1, buf2) != 0--> %d\n", strcmp(buf1, buf2) != 0);
		
	if ((*p1 == 10) && (*p2 == 5) && (strcmp(buf1, buf2) != 0))
	{
		printf("PASS - pointers were swapped\n\n");
	}
	else
	{
		printf("FAIL - pointers were not swapped\n");
		printf("p1 = %d, p2 = %d\n", *p1, *p2);
	}
	printf("x = %d\ny = %d\n", *p1, *p2);
}

void TestRmspaces()
{
	char str[100] = "	  123@!Something with   	 many 		   	spaces in between 		 ";
	char expres[100] = "123@!Something with many spaces in between";
	
	printf("/************************************************************\n");
	printf("	Testing RmSpaces						 \n");
	printf("************************************************************/\n\n");
	
	Rmspaces(str);
	
	if (strcmp(str, expres) == 0)
	{
		printf("PASS - spaces were removed as should\n\n");
	}
	else
	{
		printf("FAILED - spaces were not removed as should\n");
		printf("%ld %ld\n", strlen(str), strlen(expres));
		printf("%s\n%s\n\n", str, expres);
		printf("res = %s and expres = %s\n\n", str, expres);
	}
}

void TestSumNum()
{
	char str01[] = "9";
	char str11[] = "9";
	char str02[] = "999";
	char str12[] = "999";
	char str03[] = "1234";
	char str13[] = "156789";
	char *expres1 = "18";
	char *expres2 = "1998";	
	char *expres3 = "158023";
	char *res1 = NULL;
	char *res2 = NULL;
	char *res3 = NULL;	
	
	printf("/************************************************************\n");
	printf("	Testing SumNum						 \n");
	printf("************************************************************/\n\n");
	
	res1 = SumNum(str01, str11);
	if (strcmp(res1, expres1) == 0)
	{
		printf("PASS - adding two strings worked\n");
	}
	else
	{	
		printf("FAILED - with adding two strings\n");
		printf("res = %s and expres = %s\n\n", res1, expres1);
	}
	free(res1);
	
	res2 = SumNum(str02, str12);
	if (strcmp(res2, expres2) == 0)
	{
		printf("PASS - adding two strings worked\n");
	}
	else
	{	
		printf("FAILED - with adding two strings\n");
		printf("res = %s and expres = %s\n\n", res2, expres2);
	}
	free(res2);
	
	res3 = SumNum(str03, str13);
	if (strcmp(res3, expres3) == 0)
	{
		printf("PASS - adding two strings worked\n");
	}
	else
	{	
		printf("FAILED - with adding two strings\n");
		printf("res = %s and expres = %s\n\n", res3, expres3);
	}
	free(res3);
}

void TestMystrspn()
{
	char *str01 = "adbaaklaad";
	char *str02 = "abcd";
	char *str11 = "adbaaklaad";
	char *str12 = "ahcd";
	char *str21 = "adbjjklaad";
	char *str22 = "abcd";
	char *str31 = "12 3&%$4bjifqef  nfqoe";
	char *str32 = " &3&abcd21";
	size_t res = 0;
	size_t expres = 0;
	
	printf("/************************************************************\n");
	printf("	Testing Mystrspn						 \n");
	printf("************************************************************/\n\n");
	
	res = Mystrspn(str01, str02);
	expres = strspn(str01, str02);
	 
	if (res == expres)
	{
		printf("PASS with str1 = %s and str2 = %s\n\n", str01, str02);
	}
	else
	{
		printf("FAILED with str1 = %s and str2 = %s\n", str01, str02);
		printf("res = %ld, expres = %ld\n\n", res, expres);
	}
	
	res = Mystrspn(str11, str12);
	expres = strspn(str11, str12);
	 
	if (res == expres)
	{
		printf("PASS with str1 = %s and str2 = %s\n\n", str11, str12);
	}
	else
	{
		printf("FAILED with str1 = %s and str2 = %s\n", str11, str12);
		printf("res = %ld, expres = %ld\n\n", res, expres);
	}
	
	res = Mystrspn(str21, str22);
	expres = strspn(str21, str22);
	 
	if (res == expres)
	{
		printf("PASS with str1 = %s and str2 = %s\n\n", str21, str22);
	}
	else
	{
		printf("FAILED with str1 = %s and str2 = %s\n", str21, str22);
		printf("res = %ld, expres = %ld\n\n", res, expres);
	}
	
	res = Mystrspn(str31, str32);
	expres = strspn(str31, str32);
	 
	if (res == expres)
	{
		printf("PASS with str1 = %s and str2 = %s\n\n", str31, str32);
	}
	else
	{
		printf("FAILED with str1 = %s and str2 = %s\n", str31, str32);
		printf("res = %ld, expres = %ld\n\n", res, expres);
	}
}

void TestMystrtok()
{
	char str1[] = "^!my-^name-is mos^he-and i am-here!!!";
	char str2[] = "my-^name-is mos^he-and i am-here";
	char *token1 = NULL;
	char *token2 = NULL;
	int status = 0;
	
	printf("/************************************************************\n");
	printf("	Testing Mystrtok						 \n");
	printf("************************************************************/\n\n");
	
	token1 = Mystrtok(str1, "-^!");
	token2 = strtok(str1, "-^!");
	
	while ((NULL != token1) && (NULL != token2))
	{
		if (0 != strcmp(token1, token2))
		{
			printf("FAILED with str = %s\n", str1);
			printf("expres = %s, res = %s\n\n", token2, token1);
			++status;
		}
		
		token1 = Mystrtok(NULL, "-^!");
		token2 = strtok(NULL, "-^!");
	}
	
	if (status == 0)
	{
		printf("PASS\n\n");
	}
	
	
	token1 = Mystrtok(str2, "-^!");
	token2 = strtok(str2, "-^!");
	
	while ((NULL != token1) && (NULL != token2))
	{
		if (0 != strcmp(token1, token2))
		{
			printf("FAILED with str = %s\n", str2);
			printf("expres = %s, res = %s\n\n", token2, token1);
			++status;
		}
		
		token1 = Mystrtok(NULL, "-^!");
		token2 = strtok(NULL, "-^!");
	}
	
	if (status == 0)
	{
		printf("PASS\n\n");
	}
}

int main()
{
	TestIspalindrom();
	TestSevenboom();
	TestSwapptr();
	TestRmspaces();
	TestSumNum();
	TestMystrspn();
	TestMystrtok();
	
	return 0;
}
