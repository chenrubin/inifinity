#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */
#include <string.h> /* for all original string functions */
#include <strings.h> /* for strcasecmp */
#include <assert.h> /* for assert */
#include <ctype.h> /* for isupper and islower */

size_t Strlen(const char *s);
int Strcmp(const char *str1, const char *str2);
char *Strcpy(char *dest, const char *src);
char *Strncpy(char *dest, const char *src, size_t n);
int StrCaseCmp(const char *s1, const char *s2);
void TestStrlen();
void TestStrcmp();
void TestStrcpy();
void TestStrncpy();
void TestStrCaseCmp();
void TestStrdupe();
void TestStrcat();
void TestStrncat();
void TestMystrstr();
void TestMystrspn();
void TestMystrtok();

size_t Strlen(const char *s)
{
	size_t len = 0;
	
	assert(NULL != s);
	
	while ('\0' != *s)
	{
		++s; 
		++len;
	}
	
	return len;
}

int Strcmp(const char *str1, const char *str2) 
{
	assert(NULL != str1);
	assert(NULL != str2);
	
	while (('\0' != *str1) && (*str1 == *str2))
	{
		++str1;
	 	++str2;
	}

	return (*str1 - *str2);
}

char *Strcpy(char *dest, const char *src)
{
	char *tmp_dest = dest;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	while ('\0' != *src)
	{
		*tmp_dest++ = *src++;
	}
	
	*tmp_dest = '\0';
	
	return dest;
}

char *Strncpy(char *dest, const char *src, size_t n)
{
	unsigned int index = 0;
	char *tmp_dest = dest;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	while (('\0' != *src) && (index < n))
	{
		*tmp_dest++ = *src++;
		index++;
	}
	
	return dest;
}

int StrCaseCmp(const char *s1, const char *s2)
{
	assert(NULL != s1);
	assert(NULL != s2);
	
	while (('\0' != *s1) && ('\0' != *s2))
	{
		if (*s1 == toupper(*s2) || *s1 == tolower(*s2))
		{
			++s1;
			++s2;
		}
		else
		{
			return (*s1 - *s2);
		}
	}
	
	return (*s1 -*s2);				
}

char *StrChr(const char *s, int c)
{
	assert(NULL != s);
	
	while ('\0' != *s)
	{
		if (c == *s)
		{
			return (char *)s;
		}
		++s;
	}
	
	if ('\0' == c)
	{
		return (char *)s;
	}
	else
	{
		return NULL;
	}
}

char *StrDup(const char *s)
{
	int i,size;
	char *s_iter = (char *)s;
	char *dup, *dup_iter;
	
	assert(NULL != s);
		
	size = strlen(s_iter) + 1;
	dup = (char *)malloc(size);
	dup_iter = dup;
	
	for (i = 0; i < size ; i++)
	{
		*dup_iter = *s;
		dup_iter++;
		s++;
	}
	
	return dup;
}

char *StrCat(char *dest, const char *src)
{
	char *dest_iter = dest;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	while ('\0' != *dest_iter)
	{
		++dest_iter;
	}
	
	while ('\0' != *src)
	{
		*dest_iter = *src;
		++dest_iter;
		++src;
	}
	
	dest_iter++;
	*dest_iter = '\0';
	
	return dest;
}

char *StrnCat(char *dest, const char *src, int n)
{
	char *dest_iter = dest;
	int i = 0;

	assert(NULL != dest);
	assert(NULL != src);

	while ('\0' != *dest_iter)
	{
		dest_iter++;
	}
	
	while (('\0' != *src) && (i < n))
	{
		*dest_iter = *src;
		++dest_iter;
		++src;
		++i;
	}
	
	++dest_iter;
	*dest_iter = '\0';
	
	return dest;
}

char *Mystrstr(const char *haystack, const char *needle)
{
	char *run_n;
	char *run_h;
	size_t len = strlen(needle);
	
	assert(NULL != haystack);
	assert(NULL != needle);
	
	while ('\0' != *(haystack + len))
	{
		if (*haystack != *needle)
		{
			++haystack;
		}
		else
		{
			run_n = (char *)needle;
			run_h = (char *)haystack;
			
			while (('\0' != *run_n) && (*run_n == *run_h))
			{
				++run_n;
				++run_h;
			}
			
			if ('\0' == *run_n)
			{
				return (char *)haystack;
			}
			else
			{
				++haystack;
			}
		}
	}
	
	return NULL;
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

/* TEST functions */
void TestStrlen()
{
	char str1[20] = "";
	char str2[20] = "a";
	char str3[20] = "njiewpugerwanhgiuer";
	int res = 0;
	int expres = 0;
	
	printf("/************************************************************\n");
	printf("	Testing Strlen						 \n");
	printf("************************************************************/\n\n");
	res = Strlen(str1);
	expres = strlen(str1);
	if (res == expres)
	{
		printf("PASSED on test with %s\n\n", str1);
	}
	else
	{
		printf("FAILED on test with %s\n", str1);
		printf("expres = %d and res = %d\n\n", expres, res);
	}
	
	res = Strlen(str2);
	expres = strlen(str2);
	if (res == expres)
	{
		printf("PASSED on test with %s\n\n", str2);
	}
	else
	{
		printf("FAILED on test with %s\n", str2);
		printf("expres = %d and res = %d\n\n", expres, res);
	}
	
	res = Strlen(str3);
	expres = strlen(str3);
	if (res == expres)
	{
		printf("PASSED on test with %s\n\n", str3);
	}
	else
	{
		printf("FAILED on test with %s\n", str3);
		printf("expres = %d andvoid TestStrncpy() res = %d\n\n", expres, res);
	}
}

void TestStrcmp()
{
	char str01[20] = "compare this string";
	char str11[20] = "compare this string";
	char str02[20] = "compare this string";
	char str12[20] = "compare this string";
	char str03[20] = "compare this string";
	char str13[20] = "compare this string";
	int res = 0;
	int expres = 0;
	
	printf("/************************************************************\n");	
	printf("	Testing Strcmp						 \n");
	printf("************************************************************/\n\n");
	
	res = Strcmp(str01, str11);
	expres = strcmp(str01, str11);	
	if (res == expres)
	{
		printf("PASSED on test with %s and %s\n\n", str01, str11);
	}
	else
	{
		printf("FAILED on test with %s and %s\n", str01, str11);
		printf("expres = %d and res = %d\n\n", expres, res);
	}
	
	res = Strcmp(str02, str12);
	expres = strcmp(str02, str12);	
	if (res == expres)
	{
		printf("PASSED on test with %s and %s\n\n", str02, str12);
	}
	else
	{
		printf("FAILED on test with %s and %s\n", str02, str12);
		printf("strlen = %d and Strlen = %d\n\n", expres, res);
	}
	
	res = Strcmp(str03, str13);
	expres = strcmp(str03, str13);	
	if (res == expres)
	{
		printf("PASSED on test with %s and %s\n\n", str03, str13);
	}
	else
	{
		printf("FAILED on test with %s and %s\n", str03, str13);
		printf("strlen = %d and Strlen = %d\n\n", expres, res);
	}
}

void TestStrcpy()
{
	char *res;
	char *expres;
	char dest[] = "This is the destination for copying  !";
	const char src1[] = "something to copy  !";
	const char src2[] = "";
	
	printf("/************************************************************\n");	
	printf("	Testing Strcpy						 \n");
	printf("************************************************************/\n\n");
	
	res = Strcpy(dest, src1);
	expres = strcpy(dest, src1);
	if (strcmp(res, expres) == 0)
	{
		printf("PASSED on test with dest = %s and src = %s\n\n", dest, src1);
	}
	else
	{
		printf("FAILED on test with dest = %s and src = %s\n", dest, src1);
		printf("expres = %s and res = %s\n\n", expres, res);
	}
	
	res = Strcpy(dest, src2);
	expres = strcpy(dest, src2);
	if (strcmp(res, expres) == 0)
	{
		printf("PASSED on test with dest = %s and src = %s\n\n", dest, src2);
	}
	else
	{
		printf("FAILED on test with dest = %s and src = %s\n", dest, src2);
		printf("expres = %s and res = %s\n\n", expres, res);
	}
}

void TestStrncpy()
{
	char *res;
	char *expres;
	char dest[] = "This is the destination for copying  !";
	const char src1[] = "something to copy  !";
	const char src2[] = "";
	
	
	res = Strncpy(dest, src1, 5);
	expres = strncpy(dest, src1, 5);
	if (strcmp(res, expres) == 0)
	{
		printf("PASSED on test with dest = %s and src = %s\n\n", dest, src1);
	}
	else
	{
		printf("FAILED on test with dest = %s and src = %s\n", dest, src1);
		printf("expres = %s and res = %s\n\n", expres, res);
	}
	
	res = Strncpy(dest, src2, 5);
	expres = strncpy(dest, src2, 5);
	if (strcmp(res, expres) == 0)
	{
		printf("PASSED on test with dest = %s and src = %s\n\n", dest, src2);
	}
	else
	{
		printf("FAILED on test with dest = %s and src = %s\n", dest, src2);
		printf("expres = %s and res = %s\n\n", expres, res);
	}
}

void TestStrCaseCmp()
{
	char str1[] = "This SHOU*/Ld bE compa?rEd  ";
	char str2[] = "this shou*/lD BE COmpa?rED  ";
	int res = 0;
	int expres = 0;
	
	printf("/************************************************************\n");	
	printf("	Testing TestStrCaseCmp				 \n");
	printf("************************************************************/\n\n");
	
	res = StrCaseCmp(str1, str2);
	expres = strcasecmp(str1, str2);
	
	if (res == expres)
	{
		printf("PASSED on test with dest = %s and src = %s\n\n", str1, str2);
	}
	else
	{
		printf("FAILED on test with dest = %s and src = %s\n", str1, str2);
		printf("expres = %d and res = %d\n\n", expres, res);
	}
}

void TestStrChr()
{
	char str[] = "Something to look into";
	char chr1 = 'o';
	char chr2 =  ' ';
	char chr3 = '!';
	char chr4 = '\0';
	char *res;
	char *expres;
	
	printf("/************************************************************\n");	
	printf("	Testing TestStrChr				 \n");
	printf("************************************************************/\n\n");
	
	res = StrChr(str, chr1);
	expres = strchr(str, chr1);
	if (strcmp(res, expres) == 0)
	{
		printf("PASSED on test with str = %s and chr = %c\n\n", str, chr1);
	}
	else
	{
		printf("FAILED on test with str = %s and chr = %c\n", str, chr1);
		printf("expres = %s and res = %s\n\n", expres, res);
	}	
	
	res = StrChr(str, chr2);
	expres = strchr(str, chr2);
	if (strcmp(res, expres) == 0)
	{
		printf("PASSED on test with str = %s and chr = %c\n\n", str, chr2);
	}
	else
	{
		printf("FAILED on test with str = %s and chr = %c\n", str, chr2);
		printf("expres = %s and res = %s\n\n", expres, res);
	}	
	
	res = StrChr(str, chr3);
	expres = strchr(str, chr3);
	if (strcmp(res, expres) == 0)
	{
		printf("PASSED on test with str = %s and chr = %c\n\n", str, chr3);
	}
	else
	{
		printf("FAILED on test with str = %s and chr = %c\n", str, chr3);
		printf("expres = %s and res = %s\n\n", expres, res);
	}
	
	res = StrChr(str, chr4);
	expres = strchr(str, chr4);
	if (strcmp(res, expres) == 0)
	{
		printf("PASSED on test with str = %s and chr = %c\n\n", str, chr4);
	}
	else
	{
		printf("FAILED on test with str = %s and chr = %c\n", str, chr4);
		printf("expres = %s and res = %s\n\n", expres, res);
	}	
}

void TestStrdupe()
{
	char str[] = "something is *here";
	char *str_dup = StrDup(str);
	
	printf("/************************************************************\n");	
	printf("	Testing TestStrChr				 \n");
	printf("************************************************************/\n\n");
	
	if ((strcmp(str, str_dup) == 0) && str != str_dup)
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
		printf("Original = %s and duplicate = %s\n", str, str_dup);
		printf("original address = %p and duplicate address = %p\n\n", str, str_dup);
	}
	
	free(str_dup);
}

void TestStrcat()
{
	char str1[100] = "something is *here";
	char *str2 = "adding something else";
	char *str_res = "something is *hereadding something else";
	StrCat(str1, str2);
	
	printf("/************************************************************\n");	
	printf("	Testing TestStrcat				 \n");
	printf("************************************************************/\n\n");
	
	if (strcmp(str1, str_res) == 0)
	{
		printf("PASSED on test with str = %s and str = %s\n\n", str1, str2);
	}
	else
	{
		printf("FAILED on test with str = %s and str = %s\n", str1, str2);
		printf("expected result = %s and actual result = %s\n\n", str_res, str1);
	}
}	

void TestStrncat()
{
	char str1[40] = "something is *here";
	char str2[40] = "adding something else";
	char str_res[] = "something is *hereadding som";
	char *new_str = StrnCat(str1, str2, 10);
	
	printf("/************************************************************\n");	
	printf("	Testing TestStrnCat				 \n");
	printf("************************************************************/\n\n");
	
	if (strcmp(str_res, new_str) == 0)
	{
		printf("PASSED on test with str = %s and str = %s\n\n", str1, str2);
	}
	else
	{
		printf("FAILED on test with str = %s and str = %s\n", str1, str2);
		printf("expected result = %s and actaul result = %s\n\n", str_res, new_str);
	}
}

void TestMystrstr()
{
	char *needle = "ho";
	char *haystack= "good things happen to those who wait";
	char *res = Mystrstr(haystack, needle);
	char *expres = strstr(haystack, needle);
	
	if (0 == strcmp(res, expres))
	{
		printf("PASSED\n\n");
	}
	else
	{
		printf("FAILED with haystack = %s and needle = %s\n", haystack, needle);
		printf("expres = %s, res = %s\n\n", expres, res);
	}
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
	TestStrlen();
	TestStrcmp();
	TestStrcpy();
	TestStrncpy();
	TestStrCaseCmp();
	TestStrdupe();
	TestStrcat();
	TestStrncat();
	TestMystrstr();
	TestMystrspn();
	TestMystrtok();
	
	return 0;
}


