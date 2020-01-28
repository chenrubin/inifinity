#include <stdio.h> /* printf */
#include <unistd.h> /* ssize */

#define LSB 1
#define CHESS_SIZE 3
#define FULL_PATH 9

void RecHorsesTour(size_t location, size_t *result_path, size_t current_index, size_t prev_loc_bitmap);
int HorsesTour(size_t start, size_t *result_path);
static int IsFirstTimeIMP(size_t location, size_t prev_loc_bitmap);
static size_t SetLocationIMP(size_t location, size_t prev_loc_bitmap);
static ssize_t PossibleSteps(size_t location, size_t step_num);
int IsLastInPath(size_t location, size_t prev_loc_bitmap);
int IsRightStepLegal(ssize_t next_step, size_t location);
int IsLeftStepLegal(ssize_t next_step, size_t location);

int main()
{
	size_t result_path[CHESS_SIZE * CHESS_SIZE] = {0};
	
	HorsesTour(0, result_path);
	
	return 0;
}

int HorsesTour(size_t start, size_t *result_path)
{
	int i = 0;
	size_t prev_loc_bitmap = 0;
	
	RecHorsesTour(start, result_path, 0, prev_loc_bitmap);
	
	for (i = 0; i < (CHESS_SIZE * CHESS_SIZE); ++i)
	{
		printf("%ld, ", result_path[i]);
	}
	printf("\n");
	
	return 0;
}

void RecHorsesTour(size_t location, size_t *result_path, size_t current_index, size_t prev_loc_bitmap)
{
	size_t i = 0; 
	
	if (!IsFirstTimeIMP(location, prev_loc_bitmap))
	{
		return;
	}
	if (IsLastInPath(location, prev_loc_bitmap))
	{
		result_path[FULL_PATH] = location;
		
		return;
	}
	
	prev_loc_bitmap |= (LSB << location);
	result_path[current_index] = location;
	SetLocationIMP(location, prev_loc_bitmap);
	
	for (i = 0; i < CHESS_SIZE; ++i)
	{
		if (((i < (CHESS_SIZE / 2)) && IsLeftStepLegal(PossibleSteps(location, i), location)) ||
			((i >= (CHESS_SIZE / 2)) && IsRightStepLegal(PossibleSteps(location, i), location)))
		{
			RecHorsesTour(PossibleSteps(location, i), result_path, current_index + 1, prev_loc_bitmap);			
		}
	}
}

static int IsFirstTimeIMP(size_t location, size_t prev_loc_bitmap)
{	
	return (!((LSB << location) & prev_loc_bitmap));
}

static size_t SetLocationIMP(size_t location, size_t prev_loc_bitmap)
{	
	return ((LSB << location) | prev_loc_bitmap);
}

static ssize_t PossibleSteps(size_t location, size_t step_num)
{
	ssize_t possible_steps[CHESS_SIZE] = {0};
	
	possible_steps[0] = (ssize_t)location - 2 * CHESS_SIZE + 1;
	possible_steps[1] = (ssize_t)location + 2 - CHESS_SIZE;
	possible_steps[2] = (ssize_t)location + 2 + CHESS_SIZE;
	possible_steps[3] = (ssize_t)location + 2 * CHESS_SIZE + 1;
	possible_steps[4] = (ssize_t)location + 2 * CHESS_SIZE - 1;
	possible_steps[5] = (ssize_t)location - 2 + CHESS_SIZE;
	possible_steps[6] = (ssize_t)location - 2 - CHESS_SIZE;
	possible_steps[7] = (ssize_t)location - 2 * CHESS_SIZE - 1;
	
	return possible_steps[step_num];
}

int IsLastInPath(size_t location, size_t prev_loc_bitmap)
{
	return (FULL_PATH == ((LSB << location) | prev_loc_bitmap));
}

int IsRightStepLegal(ssize_t next_step, size_t location)
{
	if ((next_step < 0) ||
		(next_step > FULL_PATH) ||
		((next_step % CHESS_SIZE) < (location / CHESS_SIZE)))
	{
		return 0;
	}
	
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
	
	return 1;
}
