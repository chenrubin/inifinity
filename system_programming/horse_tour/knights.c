/************************************
*		Author: Chen				*	
*		Reviewer : Ran				*
*		Knight tours				*
*		29/1/2020					*
*									*
************************************/

#include <stdio.h> /* printf */
#include <unistd.h> /* ssize */
#include <time.h>
#include <alloca.h> /* alloca */
#include <stdlib.h> /* qsort */

#include "knights.h"

#define LSB (1)
#define CHESS_SIZE (/*6*/8/*5*/)
#define FULL_PATH (/*35*/63/*24*/)
#define NUM_OF_STEPS 8
#define ALL_BITS_SET /*0xFFFFFFFFF*//*33554431*/0xFFFFFFFFFFFFFFFF
#define UNUSED(x) ((void)(x))

typedef enum status
{
	SUCCESS,
	FAIL
}status_t; 

typedef struct loc_possible
{
	ssize_t location;
	size_t possibilities;
	int IsLegal;
}poss_per_loc_t;

/* finds path starting from location recursively */
static int RecHorsesTourIMP(size_t location, 
							size_t *result_path, 
							size_t current_index, 
							size_t prev_loc_bitmap);

/* checks if it is the first time locaiton is occupied  */							
static int IsFirstTimeIMP(size_t location, size_t prev_loc_bitmap);

/* set the correct locaiton bit in the bitmap  */
static size_t SetLocationIMP(size_t location, size_t prev_loc_bitmap);

/* Creates an array of strcut for each one of the eight
   possible next steps for each location  */
static void CreateNextStepsStructArrayIMP(size_t location, 
										  poss_per_loc_t *poss_arr);

/* checks if current location is last in path */
static int IsLastInPathIMP(size_t location, size_t prev_loc_bitmap);

/* Checks if one of the right steps is legal or not */
static int IsRightStepLegalIMP(ssize_t next_step, size_t location);

/* Checks if one of the left steps is legal or not */
static int IsLeftStepLegalIMP(ssize_t next_step, size_t location);

/* Checks how many possible legal steps for a location.
   This is done for huristics sorting of next possible steps */
static size_t HowManyPossibleStepsIMP(size_t location);

/* checks all steps legality for location */
static void CheckStepsLegalityIMP(poss_per_loc_t *poss_arr, size_t location);

/* compare function for qsort */
static int CompareFuncIMP(const void *data1, const void *data2);


int HorsesTour(size_t start, size_t *result_path)
{
	size_t prev_loc_bitmap = 0;
	status_t status = SUCCESS;
	
	status = RecHorsesTourIMP(start, result_path, 0, prev_loc_bitmap);
	
	return status;
}

static int RecHorsesTourIMP(size_t location, 
							size_t *result_path, 
							size_t current_index, 
							size_t prev_loc_bitmap)
{
	size_t i = 0; 
	poss_per_loc_t *poss_arr = NULL;

	if (IsLastInPathIMP(location, prev_loc_bitmap))
	{
		result_path[FULL_PATH] = location;
		
		return SUCCESS;
	}
	if (!IsFirstTimeIMP(location, prev_loc_bitmap))
	{
		return FAIL;
	}
	
	
	prev_loc_bitmap = SetLocationIMP(location, prev_loc_bitmap);
	result_path[current_index] = location;

	poss_arr = (poss_per_loc_t *)alloca(sizeof(poss_per_loc_t) * NUM_OF_STEPS);
	CreateNextStepsStructArrayIMP(location, poss_arr);
	
	qsort(poss_arr, NUM_OF_STEPS, 
		  sizeof(poss_per_loc_t),CompareFuncIMP);

	for (i = 0; i < NUM_OF_STEPS; ++i)
	{
		if (poss_arr[i].IsLegal)	
		{
			status_t status = RecHorsesTourIMP(poss_arr[i].location, 
											   result_path, 
											   current_index + 1, 
											   prev_loc_bitmap);
			if (SUCCESS == status)
			{
				return SUCCESS;
			}
		}
	}
	
	return FAIL;
}

static int IsFirstTimeIMP(size_t location, size_t prev_loc_bitmap)
{	
	return (!(((size_t)LSB << location) & prev_loc_bitmap));
}

static size_t SetLocationIMP(size_t location, size_t prev_loc_bitmap)
{	
	return (((size_t)LSB << location) | prev_loc_bitmap);
}

static void CreateNextStepsStructArrayIMP(size_t location, 
										  poss_per_loc_t *poss_arr)
{
	poss_per_loc_t *poss_arr_runner = poss_arr;

	poss_arr_runner -> location = (ssize_t)location - 2 * CHESS_SIZE + 1;
	poss_arr_runner -> possibilities =  
					   HowManyPossibleStepsIMP((ssize_t)location - 
					   							2 * CHESS_SIZE + 1);
	++poss_arr_runner;
	poss_arr_runner -> location = (ssize_t)location + 2 - CHESS_SIZE;
	poss_arr_runner -> possibilities =  
					   HowManyPossibleStepsIMP((ssize_t)location + 
					   							2 - CHESS_SIZE);
	++poss_arr_runner;
	poss_arr_runner -> location = (ssize_t)location + 2 + CHESS_SIZE;
	poss_arr_runner -> possibilities = 
					   HowManyPossibleStepsIMP((ssize_t)location + 
					   							2 + CHESS_SIZE);
	++poss_arr_runner;
	poss_arr_runner -> location = (ssize_t)location + 2 * CHESS_SIZE + 1;
	poss_arr_runner -> possibilities = 
					   HowManyPossibleStepsIMP((ssize_t)location + 
					   							2 * CHESS_SIZE + 1);
	++poss_arr_runner;
	poss_arr_runner -> location = (ssize_t)location + 2 * CHESS_SIZE - 1;
	poss_arr_runner -> possibilities = 
					   HowManyPossibleStepsIMP((ssize_t)location + 
					   							2 * CHESS_SIZE - 1);
	++poss_arr_runner;
	poss_arr_runner -> location = (ssize_t)location - 2 + CHESS_SIZE;
	poss_arr_runner -> possibilities = 
					   HowManyPossibleStepsIMP((ssize_t)location - 
					   							2 + CHESS_SIZE);
	++poss_arr_runner;
	poss_arr_runner -> location = (ssize_t)location - 2 - CHESS_SIZE;
	poss_arr_runner -> possibilities = 
					   HowManyPossibleStepsIMP((ssize_t)location - 
					   							2 - CHESS_SIZE);
	++poss_arr_runner;
	poss_arr_runner -> location = (ssize_t)location - 2 * CHESS_SIZE - 1;
	poss_arr_runner -> possibilities = 
					   HowManyPossibleStepsIMP((ssize_t)location - 
					   							2 * CHESS_SIZE - 1);
	
	CheckStepsLegalityIMP(poss_arr, location);				   							
}

static void CheckStepsLegalityIMP(poss_per_loc_t *poss_arr, size_t location)
{
	size_t i = 0;
	
	for (i = 0; i < NUM_OF_STEPS; ++i)
	{
		if (((i >= (NUM_OF_STEPS / 2)) && 
			  IsLeftStepLegalIMP(poss_arr[i].location, location)) ||
			((i < (NUM_OF_STEPS / 2)) && 
			  IsRightStepLegalIMP(poss_arr[i].location, location)))
		{
			poss_arr[i].IsLegal = 1;
		}
		else
		{
			poss_arr[i].IsLegal = 0;
		}
	}
}

static size_t HowManyPossibleStepsIMP(size_t location)
{
	ssize_t possible_steps[NUM_OF_STEPS] = {0};
	size_t i = 0;
	size_t counter = 0;
	
	possible_steps[0] = (ssize_t)location - 2 * CHESS_SIZE + 1;
	possible_steps[1] = (ssize_t)location + 2 - CHESS_SIZE;
	possible_steps[2] = (ssize_t)location + 2 + CHESS_SIZE;
	possible_steps[3] = (ssize_t)location + 2 * CHESS_SIZE + 1;
	possible_steps[4] = (ssize_t)location + 2 * CHESS_SIZE - 1;
	possible_steps[5] = (ssize_t)location - 2 + CHESS_SIZE;
	possible_steps[6] = (ssize_t)location - 2 - CHESS_SIZE;
	possible_steps[7] = (ssize_t)location - 2 * CHESS_SIZE - 1;
	
	for (i = 0; i < NUM_OF_STEPS; ++i)
	{
		if (((i >= (NUM_OF_STEPS / 2)) && 
			  IsLeftStepLegalIMP(possible_steps[i], location)) ||
			((i < (NUM_OF_STEPS / 2)) && 
			  IsRightStepLegalIMP(possible_steps[i], location)))
		{
			++counter;
		}
	}
	
	return counter;
}

static int IsLastInPathIMP(size_t location, size_t prev_loc_bitmap)
{
	return (ALL_BITS_SET == (((size_t)LSB << location) | prev_loc_bitmap));
}

static int IsRightStepLegalIMP(ssize_t next_step, size_t location)
{
	if ((next_step < 0) ||
		(next_step > FULL_PATH) ||
		((next_step % CHESS_SIZE) < (location % CHESS_SIZE)))
	{
		return 0;
	}

	return 1;
}

static int IsLeftStepLegalIMP(ssize_t next_step, size_t location)
{
	if ((next_step < 0) ||
		(next_step > FULL_PATH) ||
		((next_step % CHESS_SIZE) > (location % CHESS_SIZE)))
	{
		return 0;
	}

	return 1;
}

static int CompareFuncIMP(const void *data1, const void *data2)
{
	return ((((poss_per_loc_t *)data1) -> possibilities) - 
			(((poss_per_loc_t *)data2) -> possibilities));
}
