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
static int KnightTourStep(board_t board, position_t position);
static size_t PositionToIndex(position_t position);
static int IsOutOfBoard(position_t position);
static position_t GetNextPosition(position_t curr_position, int direction);
static int IsVisitedPosition(board_t board, position_t position);

/************************************Functions************************************/
int IsKnightTourPathFound(int x, int y)
{
	/* create board */
	board_t board = BitArrInit(OFF);
	
	/* create position struct */
	position_t position;	
		
	/* assert */
	assert(x >= 0);
	assert(x < BOARD_SIZE);
	assert(y >= 0);
	assert(y < BOARD_SIZE);
	
	/* intialize position with given x and y */
	position.x = x;
	position.y = y;
	
	/* ?? mark start position as visited ?? */
	
	/* call the recursive function */
	return KnightTourStep(board, position);
}

/********************************Private Functions********************************/
static int KnightTourStep(board_t board, position_t position)
{
	position_t new_position;
	size_t position_idx = 0;
	int status = FALSE;
	int i = 0;

	/* trivial case - out of the borad */
	if (IsOutOfBoard(position))
	{
		/* return false */
		return FALSE;		
	}
	/* trivial case - already visited position */
	if (IsVisitedPosition(board, position))
	{
		/* return false */
		return FALSE;
	}
	
	/* convert position to index */
	position_idx = PositionToIndex(position);
	
	/* mark position as visited */
	board = BitArrSetVal(board, position_idx, ON);
	
	/* trivial case - borad is full */
	if (0 == BitArrCountOff(board))
	{
		/* return true */
		return TRUE;
	}	

	/* foreach direction (8 directions) */
	for (i = 0; i < NUM_MOVES; ++i)
	{
		/* calculate next position from current position */
		new_position = GetNextPosition(position, i);
		
		/* call the recursive function with next position */
		status = KnightTourStep(board, new_position);
		
		/* if returned true - return it */
		if (TRUE == status)
		{
			return TRUE;
		}
	}
	
	/* return false */
	return FALSE; 
}

static size_t PositionToIndex(position_t position)
{
	return (size_t)(position.x * BOARD_SIZE + position.y);
}

static int IsOutOfBoard(position_t position)
{
	return (position.x < 0) || (position.x >= BOARD_SIZE) ||
		   (position.y < 0) || (position.y >= BOARD_SIZE);
}

static position_t GetNextPosition(position_t curr_position, int direction)
{
	position_t next_position;
	
	/* intialize moves LUT (static - intialize once, const - can't be modified */
	static const int moves[NUM_MOVES][2] = { { 1,2} , { 1,-2} , { 2,1} , { 2,-1} ,
											 {-1,2} , {-1,-2} , {-2,1} , {-2,-1} };
	
	/* assert */
	assert(direction >= 0);
	assert(direction < NUM_MOVES);
	
	/* initialize next position */
	next_position.x = curr_position.x + moves[direction][0];
	next_position.y = curr_position.y + moves[direction][1];
	
	/* return next poisition */
	return next_position;
}

static int IsVisitedPosition(board_t board, position_t position)
{
	size_t position_idx = PositionToIndex(position);
	
	if (ON == BitArrGetVal(board, position_idx))
	{
		return TRUE;
	}
	
	return FALSE;
}
