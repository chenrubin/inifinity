#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>

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
	double avg = 0;
	struct rusage usage;
	struct timeval start, end;
	
	for (i = 0; i < 11; ++i)
	{	
		for (j = 0; j < 10; ++j)
		{
			getrusage(RUSAGE_SELF, &usage);
			start = usage.ru_stime;
			histogram = CountingLetters(num_of_threads[i]);
			getrusage(RUSAGE_SELF, &usage);
			end = usage.ru_stime;
			
			
			times[j] = (double)(end.tv_usec - start.tv_usec) / 1000000 + 
					   (double)(end.tv_sec - start.tv_sec);
				
	/*		for (i = 0; i < NUM_OF_LETTERS; ++i)
			{
				printf("%c = %ld\n",letters[i],  histogram[i]);
			}
*/			free(histogram);
		}
		
		avg = 0;
		
		for (j = 0; j < 10; ++j)
		{
			avg += times[j];
		}
		
		printf("%ld threads = %f\n",num_of_threads[i], avg / 10);	
	}	
	
	return 0;
}
