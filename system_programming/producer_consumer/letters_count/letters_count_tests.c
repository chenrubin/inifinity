#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_OF_LETTERS 26
#define NUM_OF_THREADS 100
size_t *CountingLetters(size_t num_of_threads);
char *letters = "abcdefghijklmnopqrstuvwxyz";

int main()
{
	size_t num_of_threads[] = {1,2,4,8,15,16,40,100,101,1000,5000};
	double times[10] = {0};
	size_t i = 0;
	size_t j = 0;
	size_t *histogram = NULL;
	clock_t begin = 0;
	clock_t end = 0;
	double avg = 0;
	
	for (i = 0; i < 11; ++i)
	{	
		for (j = 0; j < 10; ++j)
		{
			begin = clock();
			histogram = CountingLetters(num_of_threads[i]);
			end = clock();
			
			times[j] = (double)(end - begin) / CLOCKS_PER_SEC;
				
		/*	for (i = 0; i < NUM_OF_LETTERS; ++i)
			{
				printf("%c = %ld\n",letters[i],  histogram[i]);
			}*/
		}
		
		avg = 0;
		
		for (j = 0; j < 10; ++j)
		{
			avg += times[j];
		}
		
		printf("%ld threads = %f\n",num_of_threads[i], avg / 10);	
	}
	
	free(histogram);	
	
	return 0;
}
