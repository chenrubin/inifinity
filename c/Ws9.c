#include <stdio.h>
#define WORD sizeof(size_t)
#define BYTE 8

void *MyMemset(void *ptr, int value, size_t num)
{
	size_t i = 0;
	size_t key = 0;
	size_t algn_adr_start = ((size_t)(size_t *)ptr / WORD) * WORD + WORD;
	size_t num_frst_byts = (algn_adr_start - (size_t)(size_t *)ptr) < (num) ? 
						(algn_adr_start - (size_t)(size_t *)ptr) : num;
	size_t num_algn_words = (num - num_frst_byts)/WORD;
	size_t algn_adr_end = (size_t)((size_t *)algn_adr_start + num_algn_words);
	size_t num_last_byts = (num - num_frst_byts) % WORD;

	/* making key */
	for (i = 0; i < WORD; ++i)
	{
		key <<= BYTE;
		key |= value;
	}
	
	/*	set first section */
	for (i = 0; i < num_frst_byts; ++i)
	{
		*((char *)ptr + i) = value;
	}
	
	/*	set middle section section */
	for (i = 0; i < num_algn_words; ++i)
	{
		*((size_t *)algn_adr_start + i) = key;
	}
	
	/*	set last section */
	for (i = 0; i < num_last_byts; ++i)
	{
		*((char *)algn_adr_end + i) = value;
	}

	return ptr;
}

int main()
{
	char str[] = {"This is an experiment and I hope it will succeed"};
	long l = 999999999999;

	printf("%s\n", str);
	MyMemset(str + 5, '*', 1);
	printf("%s\n", str);
	
	printf("%lX\n", l);
	MyMemset((char *)(&l) + 1, '*', 2);
	printf("%lX\n", l);
	
	

	return 0;
}
