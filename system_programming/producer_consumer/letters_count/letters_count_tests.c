#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_LETTERS 26
#define NUM_OF_THREADS 1

size_t *CountingLetters(size_t num_of_threads);

int main()
{
	size_t *histogram = (size_t *)malloc(NUM_OF_LETTERS * sizeof(size_t));
	int i = 0;
	
	histogram = CountingLetters(NUM_OF_THREADS);
	
	for (i = 0; i < NUM_OF_LETTERS; ++i)
	{
		printf("%ld\n", histogram[i]);
	}
	printf("\n");
	
	return 0;
}
