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
	
	return 0;
}


