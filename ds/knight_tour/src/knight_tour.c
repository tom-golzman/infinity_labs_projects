/**
	Written By: Tom Golzman
	Date: 17/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> 	/* assert() */
#include <time.h>		/* clock_t, clock(), CLOCKS_PER_SEC */
#include <stdlib.h>		/* qsort */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */

#include "bit_array.h"	/* bit_array_t , bit_array functions*/
#include "knight_tour.h"

/************************************define************************************/
enum { NUM_MOVES = 8 };

#define TIMEOUT_SECONDS 300

/************************************typedef************************************/
typedef bit_array_t board_t;

typedef struct position {
	int x;
	int y;
} position_t;

typedef struct {
	position_t position;
	int next_moves;
} move_option_t;

/**************************Private Functions Decleration**************************/
static int KnightTourStep(board_t board, position_t position, clock_t start_time);
static size_t PositionToIndex(position_t position);
static int IsOutOfBoard(position_t position);
static position_t GetNextPosition(position_t curr_position, int direction);
static int IsVisitedPosition(board_t board, position_t position);
static board_t SetVisited(board_t board, size_t position_idx);
static int IsBoardFull(board_t board);
static int CountNextMoves(board_t board, position_t position);
static int CompareMoves(const void *a, const void *b);

/************************************Functions************************************/
int IsKnightTourPathFound(int x, int y)
{
	clock_t start_time = clock();
	
	/* create board */
	board_t board = BitArrInit(OFF);
	
	/* create position struct */
	position_t position = {-1,-1};	
		
	/* assert */
	assert(x >= 0);
	assert(x < BOARD_SIZE);
	assert(y >= 0);
	assert(y < BOARD_SIZE);
	
	/* intialize position with given x and y */
	position.x = x;
	position.y = y;
	
	/* call the recursive function */
	return KnightTourStep(board, position, start_time);
}

/********************************Private Functions********************************/
static int KnightTourStep(board_t board, position_t position, clock_t start_time)
{
	position_t next_position = {-1,-1};
	size_t position_idx = 0;
	int num_valid_moves = 0;	
	int is_path_found = FALSE;
	int i = 0;
	move_option_t options[NUM_MOVES];
	clock_t curr_time = clock();

	/* timeout check */
	if ((double)(curr_time - start_time) / CLOCKS_PER_SEC > TIMEOUT_SECONDS)
	{
		return FALSE;
	}

	/* trivial cases - out of the borad or already visited position */
	if (IsOutOfBoard(position) || IsVisitedPosition(board, position))
	{
		return FALSE;		
	}
	
	/* convert position to index */
	position_idx = PositionToIndex(position);
	
	/* mark position as visited */
	board = SetVisited(board, position_idx);
	
	/* trivial case - borad is full */
	if (IsBoardFull(board))
	{
		return TRUE;
	}	

	/* fill move options with next moves count */
	for (i = 0; i < NUM_MOVES; ++i)
	{
		next_position = GetNextPosition(position, i);
		if (!IsOutOfBoard(next_position) && !IsVisitedPosition(board, next_position))
		{
			options[num_valid_moves].position = next_position;
			options[num_valid_moves].next_moves = CountNextMoves(board, next_position);
			++num_valid_moves;
		}
	}
	
	/* sort options by Warnsdorff's rule */
	qsort(options, num_valid_moves, sizeof(move_option_t), CompareMoves);

	/* foreach direction (8 directions) */
	for (i = 0; i < num_valid_moves; ++i)
	{
		/* call the recursive function with next position */
		next_position = options[i].position;
		is_path_found = KnightTourStep(board, next_position, start_time);
		if (FALSE != is_path_found)
		{
			return TRUE;
		}
	}
	
	/* backtracking */	
	board = BitArrSetVal(board, position_idx, OFF);
	return FALSE;
}

static int CountNextMoves(board_t board, position_t position)
{
	int i = 0, count = 0;
	position_t next;

	for (i = 0; i < NUM_MOVES; ++i)
	{
		next = GetNextPosition(position, i);
		if (!IsOutOfBoard(next) && !IsVisitedPosition(board, next))
		{
			++count;
		}
	}

	return count;
}

static int CompareMoves(const void *a, const void *b)
{
	return ((move_option_t *)a)->next_moves - ((move_option_t *)b)->next_moves;
}

static int IsBoardFull(board_t board)
{
	return 0 == BitArrCountOff(board);
}

static board_t SetVisited(board_t board, size_t position_idx)
{
	return BitArrSetVal(board, position_idx, ON);
}

static size_t PositionToIndex(position_t position)
{
	return (size_t)((position.x * BOARD_SIZE) + position.y);
}

static int IsOutOfBoard(position_t position)
{
	return (position.x < 0) || (position.x >= BOARD_SIZE) ||
		   (position.y < 0) || (position.y >= BOARD_SIZE);
}

static position_t GetNextPosition(position_t curr_position, int direction)
{
	position_t next_position = {-1,-1};
	
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
	
	return ON == BitArrGetVal(board, position_idx);
}
