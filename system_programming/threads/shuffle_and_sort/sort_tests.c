#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>

#define NUM_OF_LETTERS 26

typedef int (*compare)(const void *, const void *);
FILE *Sort(size_t num_of_threads, char *func_name, compare func);
int func(const void *str1, const void *str2);

int main(int argc, char *argv[])
{
/*	size_t num_of_threads[] = {30,1,4,8,15,16,40,100,101,1000,5000};
	double times[10] = {0};
	size_t i = 0;
	size_t j = 0;
	double avg = 0;
	struct rusage usage;
	struct timeval start, end;
*/	FILE *new_fp = NULL;
	int num_of_threads = atoi(argv[1]);
	
/*	for (i = 0; i < 1; ++i)
	{	
		for (j = 0; j < 1; ++j)
		{
/*			getrusage(RUSAGE_SELF, &usage);
			start = usage.ru_stime;
*/			new_fp = Sort(num_of_threads, argv[2], func);
/*			getrusage(RUSAGE_SELF, &usage);
			end = usage.ru_stime;
*/			
			
/*			times[j] = (double)(end.tv_usec - start.tv_usec) / 1000000 + 
					   (double)(end.tv_sec - start.tv_sec);
*/				
	/*		for (i = 0; i < NUM_OF_LETTERS; ++i)
			{
				printf("%c = %ld\n",letters[i],  histogram[i]);
			}
*/			
/*		}
	}	
*/	
	return 0;
}

int func (const void *str1, const void *str2) 
{
   return (strcmp(*(char **)str1, *(char **)str2));
}
