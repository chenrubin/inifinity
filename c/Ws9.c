#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#define WORD sizeof(size_t)
#define BYTE 8
#define STRING_SIZE 100
#define BASE 10
#define INT_MAX_BYTES 11
#define ASCII_NUM_CHARS 256

#define CHECK_RESULT(res, expres) res == expres


void *MyMemset(void *ptr, int value, size_t num);
void *MyMemcpy(void *dest, const void *src, size_t num);
void *MyMemmove(void *dest, const void *src, size_t num);
char *Itoa(int num);
int Atoi(char *str);
void PrintLetters(char *str1, char *str2, char *str3);
int IsLittleEndian();

void TestMyMemset();
void TestMyMemcpy();
void TestMyMemmove();
void TestItoa();
void TestAtoi();
void TestPrintLetters();
void TestIsLittleEndian();

int main()
{
	TestMyMemset();
	TestMyMemcpy();
	TestMyMemmove();
	TestItoa();
	TestAtoi();
	TestPrintLetters();
	TestIsLittleEndian();
	
	return 0;
}

void *MyMemset(void *ptr, int value, size_t num)
{
	size_t i = 0;
	size_t value_string = 0;
	size_t algn_adr_start = ((size_t)(size_t *)ptr / WORD) * WORD + WORD;
	size_t num_frst_byts = (algn_adr_start - (size_t)(size_t *)ptr) < (num) ?
						(algn_adr_start - (size_t)(size_t *)ptr) : num;
	size_t num_algn_words = (num - num_frst_byts)/WORD;
	size_t algn_adr_end = (size_t)((size_t *)algn_adr_start + num_algn_words);
	size_t num_last_byts = (num - num_frst_byts) % WORD;

	/* making value_string */
	for (i = 0; i < WORD; ++i)
	{
		value_string <<= BYTE;
		value_string |= value;
	}
	
	/*	set first section */
	for (i = 0; i < num_frst_byts; ++i)
	{
		*((char *)ptr + i) = value;
	}
	
	/*	set middle section section */
	for (i = 0; i < num_algn_words; ++i)
	{
		*((size_t *)algn_adr_start + i) = value_string;
	}
	
	/*	set last section */
	for (i = 0; i < num_last_byts; ++i)
	{
		*((char *)algn_adr_end + i) = value;
	}

	return ptr;
}

void *MyMemcpy(void *dest, const void *src, size_t num)
{
	size_t i = 0;
	size_t num_full_words = (num / WORD);
	size_t end_words_src = (size_t)((size_t *)src + num_full_words);
	size_t end_words_dst = (size_t)((size_t *)dest + num_full_words);
	size_t num_last_byts = num - ((num / WORD) * WORD);
	
	/*	set first section */
	for (i = 0; i < num_full_words; ++i)
	{
		*((size_t *)dest + i) = *((size_t *)src + i);
	}
	
	/*	set last section */
	for (i = 0; i < num_last_byts; ++i)
	{
		*((char *)end_words_dst + i) = *((char *)end_words_src + i);
	}

	return dest;
}

void *MyMemmove(void *dest, const void *src, size_t num)
{
	size_t i = 0;
	size_t num_full_words = (num / WORD);
	size_t end_words_src = (size_t)((size_t *)src + num_full_words);
	size_t end_words_dst = (size_t)((size_t *)dest + num_full_words);
	size_t num_last_byts = num - ((num / WORD) * WORD);
	
	if ((char *)dest > (char *)src)
	{
		/*	set last section */
		for (i = 0; i < num_last_byts; ++i)
		{
			*((char *)end_words_dst + i) = *((char *)end_words_src + i);
		}
		
		/*	set first section */
		for (i = num_full_words; i > 0; --i)
		{
			*((size_t *)dest + i - 1) = *((size_t *)src + i - 1);
		}
	}
	else
	{
		/*	set first section */
		for (i = num_full_words; i > 0; --i)
		{
			*((size_t *)dest + i - 1) = *((size_t *)src + i - 1);
		}
		
		/*	set last section */
		for (i = 0; i < num_last_byts; ++i)
		{
			*((char *)end_words_dst + i) = *((char *)end_words_src + i);
		}
	}
	

	return dest;
}

char *Itoa(int num)
{
	int i = 0;
	int buf_len = 0;
	char tmp_chr = '\0';
	char *buf = (char *)malloc((INT_MAX_BYTES + 1) * sizeof(char));

	while (num > 0)
	{
		*(buf + i) = (char)(num % BASE + '0');
		num /= BASE;
		++i;
	}
	*(buf + i) = '\0';
	buf_len = i;
	i= 0;
	
	while ((buf + i) < (buf + buf_len - 1 - i))
	{
		tmp_chr = *(buf + i);
		*(buf + i) = *(buf + buf_len - 1 - i);
		*(buf + buf_len - 1 - i) = tmp_chr;
		++i;
	}
	
	return buf;
}

int Atoi(char *str)
{
	int num = 0;
	int times = 1;
	char *str_run = str;
	
	assert(str);
	
	while (*(str_run + 1) != '\0')
	{
		++str_run;
	}
	
	while (str_run >= str)
	{
		num += (*(str_run) - '0') * times;
		times *= BASE;
		--str_run;
	}
	
	return num;
}

void PrintLetters(char *str1, char *str2, char *str3)
{
	int ascii_array[2][ASCII_NUM_CHARS] = {{0}, {0}};
	
	while ('\0' != *str3)
	{
		ascii_array[1][(int)(*str3)] = 1;
		++str3;
	}
	
	while ('\0' != *str1)
	{
		ascii_array[0][(int)(*str1)] = 1;
		++str1;
	}
	
	while ('\0' != *str2)
	{
		if ((1 == ascii_array[0][(int)(*str2)]) && 
			(0 ==  ascii_array[1][(int)(*str2)]))
		{
			printf("%c, ", *str2);
			ascii_array[0][(int)(*str2)] = 0;
		}
		
		++str2;
	}
	
	printf("\n");
}

int IsLittleEndian()
{
	int num = 0x1234;
	unsigned char *ptr = (unsigned char *)&num;
	
	if (0x34 == *ptr)
	{
		return 1;
	}

	return 0;
}

void TestMyMemset()
{
	char str1[] = {"This is an experiment and I hope it will succeed"};
	char str2[] = {"This is an experiment and I hope it will succeed"};
	long l1 = 999999999999;
	long l2 = 999999999999;
	int n1 = 999812999;
	int n2 = 999812999;
	
	printf("\n/*******************************************\n");
	printf("		Testing MyMemset				    	");
	printf("\n*******************************************/\n");
	
	if (0 == strcmp(MyMemset(str1 + 5, '*', 23), 
					memset(str2 + 5, '*', 23)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	if (0 == strcmp(MyMemset(str1 + 1, '9', 3), 
					memset(str2 + 1, '9', 3)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	MyMemset(&l1, '*', 4);
	printf("l1 after MyMemse = %ld\n", l1);
	memset(&l2, '*', 4);
	printf("l2 after MyMemse = %ld\n", l2);
	MyMemset(&n1, '*', 2);
	printf("n1 after MyMemse = %d\n", n1);
	memset(&n2, '*', 2);
	printf("n2 after MyMemse = %d\n", n2);
	
/*	CHECK_RESULT(*(long *)MyMemset((long *)(&l1) + 1, '*', 5),
				 *(long *)memset((long *)(&l2) + 1, '*', 5));
	
	CHECK_RESULT(MyMemset((int *)&n1, '*', 1),
				   memset((int *)&n2, '*', 1));
*/
}

void TestMyMemcpy()
{	
	char *str_src = "Trying memcpy for the first time, cross your fingers";
	char *memcpy_str_dst = (char *)malloc(sizeof(char) * STRING_SIZE);
	char *mymemcpy_str_dst = (char *)malloc(sizeof(char) * STRING_SIZE);
	
	if (NULL == memcpy_str_dst)
	{
		printf("memcpy_str_dst returned NULL\n");
		exit(1);
	}
	if (NULL == mymemcpy_str_dst)
	{
		printf("mymemcpy_str_dst returned NULL\n");
		exit(1);
	}	
	
	printf("\n/*******************************************\n");
	printf("		Testing MyMemcpy				    	");
	printf("\n*******************************************/\n");
	
	*(mymemcpy_str_dst + 26) = '\0';
	*(memcpy_str_dst + 26) = '\0';
	
	if (0 == strcmp(MyMemcpy(mymemcpy_str_dst, str_src + 2, 25),
					memcpy(memcpy_str_dst, str_src + 2, 25)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	*(mymemcpy_str_dst + 6) = '\0';
	*(memcpy_str_dst + 6) = '\0';
	
	if (0 == strcmp(MyMemcpy(mymemcpy_str_dst, str_src, 5),
					memcpy(memcpy_str_dst, str_src, 5)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
/*	CHECK_RESULT(*(long *)MyMemset((long *)(&l1) + 1, '*', 5),
				 *(long *)memset((long *)(&l2) + 1, '*', 5));
	
	CHECK_RESULT(MyMemset((int *)&n1, '*', 1),
				   memset((int *)&n2, '*', 1));
*/
}

void TestMyMemmove()
{
	char str1[] = {"This is an experiment and I hope it will succeed"};
	char str2[] = {"This is an experiment and I hope it will succeed"};
	
	printf("\n/*******************************************\n");
	printf("		Testing MyMemmove				    	");
	printf("\n*******************************************/\n");
	
	if (0 == strcmp(MyMemmove(str1 + 8, str1, 13),
					memmove(str2 + 8, str2, 13)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
		
	if (0 == strcmp(MyMemmove(str1 + 25, str1 + 30, 10),
					memmove(str2 + 25, str2 + 30, 10)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
}

void TestItoa()
{
	int num = 12056780;
	
	printf("\n/*******************************************\n");
	printf("		Testing Itoa				    	");
	printf("\n*******************************************/\n");
	
	if (0 == strcmp(Itoa(num), "12056780"))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
}

void TestAtoi()
{
	printf("\n/*******************************************\n");
	printf("		Testing Atoi					    	");
	printf("\n*******************************************/\n");
	
	if (145678915 == Atoi("145678915"))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	if (145000915 == Atoi("145000915"))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
}

void TestPrintLetters()
{
	char *str1 = "654hgtf4k^&4@~";
	char *str2 = "654^&&&l";
	char *str3 = "1259*$%7757&";
	char expres[] = {'6', '4', '^'};
	int i = 0;
	
	printf("\n/*******************************************\n");
	printf("		Testing Atoi					    	");
	printf("\n*******************************************/\n");

	PrintLetters(str1, str2, str3);
	
	for (i = 0; i < 3; ++i)
	{
		printf("%c, ", expres[i]);
	}
	
	printf("\n");
}

void TestIsLittleEndian()
{
	printf("\n/*******************************************\n");
	printf("		Testing IsLittleEndian			    	");
	printf("\n*******************************************/\n");
	
	if (1 == IsLittleEndian())
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
}
