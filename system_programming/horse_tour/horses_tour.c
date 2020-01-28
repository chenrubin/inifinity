#include <stdio.h> /* printf */
#include <unistd.h> /* ssize */
#include <time.h>

#define LSB (1)
#define CHESS_SIZE (8)
#define FULL_PATH (63)
#define NUM_OF_STEPS 8
#define ALL_BITS_SET 0xFFFFFFFFFFFFFFFF

struct timespec {
               time_t tv_sec;        /* seconds */
               long   tv_nsec;       /* nanoseconds */
           };
typedef enum status
{
	SUCCESS,
	FAIL
}status_t;           

int RecHorsesTour(size_t location, size_t *result_path, size_t current_index, size_t prev_loc_bitmap);
int HorsesTour(size_t start, size_t *result_path);
static int IsFirstTimeIMP(size_t location, size_t prev_loc_bitmap);
static size_t SetLocationIMP(size_t location, size_t prev_loc_bitmap);
static ssize_t PossibleSteps(size_t location, size_t step_num);
int IsLastInPath(size_t location, size_t prev_loc_bitmap);
int IsRightStepLegal(ssize_t next_step, size_t location);
int IsLeftStepLegal(ssize_t next_step, size_t location);

int main(int argc, char *argv[])
{
	size_t result_path[CHESS_SIZE * CHESS_SIZE] = {0};
	int start = atoi(argv[1]);
	
	HorsesTour(start, result_path);
	
	return 0;
}

int HorsesTour(size_t start, size_t *result_path)
{
	int i = 0;
	size_t prev_loc_bitmap = 0;
	status_t status = SUCCESS;
	
	status = RecHorsesTour(start, result_path, 0, prev_loc_bitmap);
	
	for (i = 0; i < (CHESS_SIZE * CHESS_SIZE); ++i)
	{
		printf("%ld, ", result_path[i]);
	}
	printf("\n");
	
	return 0;
}

int RecHorsesTour(size_t location, size_t *result_path, size_t current_index, size_t prev_loc_bitmap)
{
	size_t i = 0; 
	size_t j = 0;
	struct timespec ts;
	static size_t counter = 0;
	
	ts.tv_sec = 0;        /* seconds */
    ts.tv_nsec = 500000000; 
	if (IsLastInPath(location, prev_loc_bitmap))
	{
/*		printf("inside IsLastInPath(location, prev_loc_bitmap) = %ld, %ld\n", location, prev_loc_bitmap);*/
		result_path[FULL_PATH] = location;
		
		return SUCCESS;
	}
	if (!IsFirstTimeIMP(location, prev_loc_bitmap))
	{
/*		printf("inside I!IsFirstTimeIMP(location, prev_loc_bitmap) = %ld, %ld\n", location, prev_loc_bitmap);*/
		return FAIL;
	}
	
	
	prev_loc_bitmap = SetLocationIMP(location, prev_loc_bitmap);
	result_path[current_index] = location;
	
/*	printf("location = %ld, current_index = %ld\n", location, current_index);
	for (j = 0; j < (CHESS_SIZE * CHESS_SIZE); ++j)
	{
		printf("%ld, ", result_path[j]);
	}
	printf("\n");
*/	for (i = 0; i < NUM_OF_STEPS; ++i)
	{
/*		printf("inside for (i = 0; i < CHESS_SIZE; ++i), i = %ld, current_index = %ld\n", i, current_index);
/*		for (j = 0; j < CHESS_SIZE * CHESS_SIZE; ++j)
		{
			printf("%ld, ", result_path[j]);
		}
		printf("\n");
*//*		nanosleep(&ts, &ts);*/
		/*sleep(1);*/
		if (((i >= (NUM_OF_STEPS / 2)) && IsLeftStepLegal(PossibleSteps(location, i), location)) ||
			((i < (NUM_OF_STEPS / 2)) && IsRightStepLegal(PossibleSteps(location, i), location)))
		{
/*			printf("next step = %ld\n", PossibleSteps(location, i));*/
/*			for (j = 0; j < CHESS_SIZE * CHESS_SIZE; ++j)
			{
				printf("%ld, ", result_path[j]);
			}
		printf("\n");
*/			status_t status = RecHorsesTour(PossibleSteps(location, i), result_path, current_index + 1, prev_loc_bitmap);
			if (SUCCESS == status)
			{
				return SUCCESS;
			}
		}
	}
/*	++counter;*/
/*	printf("went out, counter = %ld\n", counter);*/
	
	return FAIL;
}

static int IsFirstTimeIMP(size_t location, size_t prev_loc_bitmap)
{	
	if (0 != (((size_t)LSB << location) & prev_loc_bitmap))
	{
/*		printf("location %ld in %ld!!!!!!!!!!!!!!! Already exists !!!!!!!!!!!!!!!!!!\n", location, prev_loc_bitmap);*/
	}
	return (!(((size_t)LSB << location) & prev_loc_bitmap));
}

static size_t SetLocationIMP(size_t location, size_t prev_loc_bitmap)
{	
/*	printf("!!!!!!!!!!prev_loc_bitmap = 0x%lX!!!!!!!!!!!!!!!!!!!!!!!1\n", prev_loc_bitmap);*/
	
	return (((size_t)LSB << location) | prev_loc_bitmap);
}

static ssize_t PossibleSteps(size_t location, size_t step_num)
{
	ssize_t possible_steps[NUM_OF_STEPS] = {0};
	
	possible_steps[0] = (ssize_t)location - 2 * CHESS_SIZE + 1;
	possible_steps[1] = (ssize_t)location + 2 - CHESS_SIZE;
	possible_steps[2] = (ssize_t)location + 2 + CHESS_SIZE;
	possible_steps[3] = (ssize_t)location + 2 * CHESS_SIZE + 1;
	possible_steps[4] = (ssize_t)location + 2 * CHESS_SIZE - 1;
	possible_steps[5] = (ssize_t)location - 2 + CHESS_SIZE;
	possible_steps[6] = (ssize_t)location - 2 - CHESS_SIZE;
	possible_steps[7] = (ssize_t)location - 2 * CHESS_SIZE - 1;
/*	printf("step_num = %ld\n", step_num);
/*	printf("step = %ld\n", possible_steps[step_num]);
*/	return possible_steps[step_num];
}

int IsLastInPath(size_t location, size_t prev_loc_bitmap)
{
	return (ALL_BITS_SET == (((size_t)LSB << location) | prev_loc_bitmap));
}

int IsRightStepLegal(ssize_t next_step, size_t location)
{
	if ((next_step < 0) ||
		(next_step > FULL_PATH) ||
		((next_step % CHESS_SIZE) < (location % CHESS_SIZE)))
	{
		return 0;
	}
	
/*	printf("inside IsRightStepLegal, location = %ld, next_step = %ld\n",location, next_step);
*/	
	return 1;
}

int IsLeftStepLegal(ssize_t next_step, size_t location)
{
	if ((next_step < 0) ||
		(next_step > FULL_PATH) ||
		((next_step % CHESS_SIZE) > (location % CHESS_SIZE)))
	{
		return 0;
	}
	
/*	printf("inside IsLeftStepLegal, next_step = %ld\n", next_step);
*/	
	return 1;
}
