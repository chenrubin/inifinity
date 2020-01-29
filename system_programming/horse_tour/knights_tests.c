#include <stdio.h> /* printf */
#include <time.h> /* clock_t */

#include "knights.h"

#define CHESS_SIZE (/*6*/8/*5*/)
#define UNUSED(x) ((void)(x))

typedef enum status
{
	SUCCESS,
	FAIL
}status_t; 

clock_t start_t, end_t;
double total_t;

int main(int argc, char *argv[])
{
	size_t result_path[CHESS_SIZE * CHESS_SIZE] = {0};
	status_t status = SUCCESS;
	size_t i = 0;
	UNUSED(argc);
	UNUSED(argv);
	
/*	size_t j = 0;*/
/*	size_t XOR_res = 0;*/
/*	size_t temp_arr[CHESS_SIZE * CHESS_SIZE] = {0};*/
	
/*	for (i = 0; i < CHESS_SIZE * CHESS_SIZE; ++i)
	{
		temp_arr[i] = i;	
	}
*/	start_t = clock();
	for (i = 0; i < CHESS_SIZE * CHESS_SIZE; ++i)
	{
		status |= HorsesTour(i, result_path);
/*		if (SUCCESS == status)
		{
			XOR_res = 0;
			for (j = 0; j < (CHESS_SIZE * CHESS_SIZE); ++j)
			{
				XOR_res ^= result_path[j] ^ temp_arr[j];
			}
			printf("XOR = %ld\n", XOR_res);
			for (j = 0; j < (CHESS_SIZE * CHESS_SIZE); ++j)
			{
				printf("%ld, ", result_path[j]);
			}
			printf("\n");
		}
		else
		{
			printf("FAILED\n");
			XOR_res = result_path[0];
			for (j = 0; j < (CHESS_SIZE * CHESS_SIZE) - 1; ++j)
			{
				XOR_res ^= result_path[j + 1];
			}
			for (j = 0; j < (CHESS_SIZE * CHESS_SIZE); ++j)
			{
				XOR_res ^= temp_arr[j];
			}
			printf("XOR = %ld\n", XOR_res);
		}*/
	}
	end_t = clock();
	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("Total time taken by CPU: %f\n", total_t);
	
	printf("status = %d\n", status);
	
	return 0;
}
