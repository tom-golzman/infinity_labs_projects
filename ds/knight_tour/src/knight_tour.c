/**
	Written By: Tom Golzman
	Date: 17/06/2025
	Reviewed By: Amir Granot
**/

/************************************includes************************************/
#include <assert.h> 	/* assert() */
#include <time.h>		/* clock_t, clock(), CLOCKS_PER_SEC */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */

#include "bit_array.h"	/* bit_array_t , bit_array functions*/
#include "sorts.h"		/* QuickSort() */
#include "knight_tour.h"

/************************************define************************************/
enum { NUM_DIRECTIONS = 8 };

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
static int CountMoveOptions(board_t board, position_t curr_pos, move_option_t* options_out);
static int CountValidMoves(board_t board, position_t position);
static int CompareMoves(const void *a, const void *b);
static int IsTimeoutReached(clock_t start_time);

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
	int num_move_options = 0;	
	int is_path_found = FALSE;
	int i = 0;
	move_option_t move_options[NUM_DIRECTIONS];
	
	/* timeout check */
	if (IsTimeoutReached(start_time))
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

	/* count move options */
	num_move_options = CountMoveOptions(board, position, move_options);
	
	/* sort options by Warnsdorff's rule */
	QuickSort(move_options, num_move_options, sizeof(move_option_t), CompareMoves);

	/* for each valid moves */
	for (i = 0; i < num_move_options; ++i)
	{
		/* call the recursive function with next position */
		next_position = move_options[i].position;
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

static int CountMoveOptions(board_t board, position_t curr_pos, move_option_t *options_out)
{
	position_t next_pos = {-1, -1};
	int i = 0;
	int num_valid = 0;
	int valid_moves = 0;

	assert(NULL != options_out);
	
	/* for each possible directions */
	for (i = 0; i < NUM_DIRECTIONS; ++i)
	{
		/* get next position and check if its valid */
		next_pos = GetNextPosition(curr_pos, i);
		if (!IsOutOfBoard(next_pos) && !IsVisitedPosition(board, next_pos))
		{
			/* count valid moves from next position */
			valid_moves = CountValidMoves(board, next_pos);
			
			/* add the next position with its valid moves, to the options array */	
			options_out[num_valid].position = next_pos;
			options_out[num_valid].next_moves = valid_moves;
			++num_valid;
		}
	}

	return num_valid;
}

static int CountValidMoves(board_t board, position_t position)
{
	int i = 0, count = 0;
	position_t next;

	/* for each possible directions */
	for (i = 0; i < NUM_DIRECTIONS; ++i)
	{
		/* get next position and check if its valid */
		next = GetNextPosition(position, i);
		if (!IsOutOfBoard(next) && !IsVisitedPosition(board, next))
		{
			++count;
		}
	}

	return count;
}

static int IsTimeoutReached(clock_t start_time)
{
	clock_t curr_time = clock();
	
	return (((double)(curr_time - start_time) / CLOCKS_PER_SEC) > TIMEOUT_SECONDS) ? TRUE : FALSE;
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
	static const int moves[NUM_DIRECTIONS][2] = { { 1,2} , { 1,-2} , { 2,1} , { 2,-1} ,
											 {-1,2} , {-1,-2} , {-2,1} , {-2,-1} };
	
	/* assert */
	assert(direction >= 0);
	assert(direction < NUM_DIRECTIONS);
	
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
