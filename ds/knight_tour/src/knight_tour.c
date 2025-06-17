/**
	Written By: Tom Golzman
	Date: 17/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */

#include "bit_array.h"
#include "knight_tour.h"

/************************************define************************************/
enum { NUM_MOVES = 8 };

/************************************typedef************************************/

typedef struct position {
	int x;
	int y;
} position_t;

/**************************Private Functions Decleration**************************/
static int KnigtTourStep(board_t board, position_t position);
static int PositionToIndex(position_t position);
static int IsOutOfBorad(position_t position);
static position_t GetNextMove(position_t curr_position, int direction);
static int IsVisited(board_t board, position_t position);

/************************************Functions************************************/
int IsKnightTourPathFound(int x, int y)
{
	board_t board;
	
	/* assert */
	assert(x >= 0);
	assert(x < BORAD_SIZE);
	assert(y >= 0);
	assert(y < BORAD_SIZE);
	
	/* create borad */
	
	/* ?? mark start position as visited ?? */
	
	/* call the recursive function */
}

/********************************Private Functions********************************/
static int KnigtTourStep(board_t board, position_t position)
{
	/* assert */
	
	/* trivial case - out of the borad */
		/* return false */
	/* trivial case - already visited position */
		/* return false */

	/* mark position as visited */
	
	/* trivial case - borad is full */
		/* return success */
		

	/* foreach direction (8 directions) */
		/* calculate next position from current position */
		/* call the recursive function with next position */
		/* if returned success */
			/* return success */
			
	/* return failure */ 
}

static int PositionToIndex(position_t position)
{
	
}

static int IsOutOfBorad(position_t position)
{}

static position_t GetNextMove(position_t curr_position, int direction)
{}

static int IsVisited(board_t board, position_t position)
{}
