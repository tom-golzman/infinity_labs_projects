#ifndef __ILRD_KNIGHT_TOUR_H__
#define __ILRD_KNIGHT_TOUR_H__

/************************************includes************************************/

/************************************typedef************************************/
typedef bit_array_t board_t;

/************************************define************************************/
enum { BOARD_SIZE = 8 };

/************************************Functions************************************/
int IsKnightTourPathFound(int x, int y);

#endif /*__ILRD_KNIGHT_TOUR_H__*/
