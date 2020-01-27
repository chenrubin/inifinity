#include <stdio.h> /* printf */
#include <unistd.h> /* ssize */

#define LSB 1
#define CHESS_SIZE 8
#define FULL_PATH 63

void RecHorsesTour(size_t location, size_t *result_path, size_t current_index);
int HorsesTour(size_t start, size_t *result_path);
static int IsFirstTimeIMP(size_t location, size_t prev_loc_bitmap);
static size_t SetLocationIMP(size_t location, size_t prev_loc_bitmap);
static size_t *PossibleSteps(size_t location);
int IsLastInPath(size_t location, size_t prev_loc_bitmap);
int IsRightStepLegal(ssize_t next_step, size_t location);
int IsLeftStepLegal(ssize_t next_step, size_t location);

int main()
{
	size_t result_path[64] = {0};
	
	HorsesTour(0, result_path);
}

int HorsesTour(size_t start, size_t *result_path)
{
	int i = 0;
	
	RecHorsesTour(start, result_path, 0);
	
	for (i = 0; i < 64; ++i)
	{
		printf("%ld, ", result_path[i]);
	}
	printf("\n");
	
	return 0;
}

void RecHorsesTour(size_t location, size_t *result_path, size_t current_index)
{
	size_t prev_loc_bitmap = 0;
	size_t *possible_steps = NULL;
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
	possible_steps = PossibleSteps(location);
	
	for (i = 0; i < CHESS_SIZE; ++i)
	{
		if (((i < (CHESS_SIZE / 2)) && IsLeftStepLegal(possible_steps[i], location)) ||
			((i >= (CHESS_SIZE / 2)) && IsRightStepLegal(possible_steps[i], location)))
		{
			RecHorsesTour(possible_steps[i], result_path, current_index + 1);			
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

static size_t *PossibleSteps(size_t location)
{
	size_t possible_steps[] = {location - 2 * CHESS_SIZE + 1,
							   location + 2 - CHESS_SIZE,
							   location + 2 + CHESS_SIZE,
							   location + 2 * CHESS_SIZE + 1,
							   location + 2 * CHESS_SIZE - 1,
							   location - 2 + CHESS_SIZE,
							   location - 2 - CHESS_SIZE,
							   location - 2 * CHESS_SIZE - 1};
	
	return possible_steps;
}

int IsLastInPath(size_t location, size_t prev_loc_bitmap)
{
	LSB <<= location;
	
	return (FULL_PATH == (LSB | prev_loc_bitmap));
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
		((next_step % CHESS_SIZE) > (location / CHESS_SIZE)))
	{
		return 0;
	}
	
	return 1;
}
