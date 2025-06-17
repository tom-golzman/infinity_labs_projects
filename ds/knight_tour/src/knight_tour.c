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

/************************************typedef************************************/
typedef bit_array_t board_t;

/************************************Functions************************************/
int IsKnightTourPathFound(int x, int y)
{
	/* assert */
	
	/* create borad */
	/* mark start position as visited */
	
	/* call the recursive function */
}

static int KnigtTourStep(board_t board, int x, int y)
{
	/* assert */
	
	/* trivial case - out of the borad */
		/* return false */
	/* trivial case - already visited position */
		/* return false */

	/* mark position as visited */
	
	/* trivial case - borad is full */
		/* return success */
		

	/* for loop 8 times */
		/* call the recursive function with different possible knight's moves */
	
	/* return the returned status of the recursive calls */ 
}
