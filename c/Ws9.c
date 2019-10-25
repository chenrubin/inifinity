#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <stdlib.h> /* malloc */
#define WORD sizeof(size_t)
#define BYTE 8
#define STRING_SIZE 100

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

int main()
{
	char *str_src = (char *)malloc(sizeof(char) * STRING_SIZE);
	char *str_dst = (char *)malloc(sizeof(char) * STRING_SIZE);
	str_src = "Trying memcpy for the first time, cross your fingers";
	
	
	printf("%s\n", str_src);
	MyMemcpy(str_dst, str_src + 2, 25);
	*(str_dst + 33) = '\0';
	printf("%s\n", str_dst);
	
/*	char str[] = {"This is an experiment and I hope it will succeed"};
	char str1[] = {"This is an experiment and I hope it will succeed"};
	long l = 999999999999;

	printf("%s\n", str);
	MyMemset(str + 5, '*', 1);
	printf("%s\n", str);
	
	printf("%s\n", str1);
	memset(str1 + 5, '*', 1);
	printf("%s\n", str1);
	
	printf("%lX\n", l);
	MyMemset((char *)(&l) + 1, '*', 2);
	printf("%lX\n", l);
*/	
	return 0;
}
