#ifndef __ILRD_BIT_ARRAY__
#define __ILRD_BIT_ARRAY__

/************************************includes************************************/
#include <limits.h> /* CHAR_BIT */
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef enum {
    ON  = 1,
    OFF = 0
} state_t;

typedef size_t bit_array_t;

/************************************define************************************/
/*
# NOTES: index must be lower than WORD_SIZE_BITS.
# EXAMPLE:
	if(index < WORD_SIZE_BITS)
	{
   		BitArrSetVal(arr, index, value)
	}
*/
#define WORD_SIZE_BITS (sizeof(size_t)*CHAR_BIT)
#define STRING_BUFFER_SIZE (sizeof(size_t)*CHAR_BIT + 1)

/************************************Functions************************************/
/*
# DESCRIPTION: initialize a bit array with all bits set to val
# RETURNS: bit_array_t - bit array set with all bits set to val
# ARGUMENTS:
    @arg1 - int val (state_t enum - OFF (0) or ON (any other value))
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrInit(state_t val);

/*
# DESCRIPTION: updates value of given index with given value
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
    @arg2 - size_t index
    @arg3 - int val (state_t enum - OFF (0) or ON (any other value))
# LIMITATIONS: index must be lower than WORD_SIZE_BITS.
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrSetVal(bit_array_t arr, size_t index, state_t val);

/*
# DESCRIPTION: get the bit value at specified index
# RETURNS: int - either ON or OFF
# ARGUMENTS:
    @arg1 - bit_array_t arr
    @arg2 - size_t index
# LIMITATIONS: index must be lower than WORD_SIZE_BITS
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
state_t BitArrGetVal(bit_array_t arr, size_t index);

/*
# DESCRIPTION: counts the number of turned ON bits (bits set to 1)
# RETURNS: int - the number of bits set to 1
# ARGUMENTS:
    @arg1 - bit_array_t arr
# LIMITATIONS: no limitations
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
size_t BitArrCountOn(bit_array_t arr);

/*
# DESCRIPTION: counts the number of turned OFF bits (bits set to 0)
# RETURNS: int - the number of bits set to 0
# ARGUMENTS:
    @arg1 - bit_array_t arr
# LIMITATIONS: no limitations
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
size_t BitArrCountOff(bit_array_t arr);

/*
# DESCRIPTION: set the the bit at the specified index to ON (1)
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
    @arg2 - size_t index
# LIMITATIONS: index must be lower than WORD_SIZE_BITS
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/

bit_array_t BitArrSetOn(bit_array_t arr, size_t index);

/*
# DESCRIPTION: set the the bit at the specified index to OFF (0)
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
    @arg2 - size_t index
# LIMITATIONS: index must be lower than WORD_SIZE_BITS
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrSetOff(bit_array_t arr, size_t index);

/*
# DESCRIPTION: toggle bit at index
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
    @arg2 - size_t index
# LIMITATIONS: no limitations
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrToggle(bit_array_t arr, size_t index);

/*
# DESCRIPTION: sets all bits to 0
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
# LIMITATIONS: no limitations
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrSetAllOn(bit_array_t arr);

/*
# DESCRIPTION: sets all bits to 1
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
# LIMITATIONS: no limitations
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrSetAllOff(bit_array_t arr);

/*
# DESCRIPTION: swap order of bits in reverse
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
# LIMITATIONS: no limitations
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrMirror(bit_array_t arr);

/*
# DESCRIPTION: shifts the array to the left specific amount of times. the bits removed from left are added back to the right side.
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
    @arg2 - unsigned int amount
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrRotateLeft(bit_array_t arr, unsigned int amount);

/*
# DESCRIPTION: shifts the array to the right specific amount of times. the bits removed from right are added back to the left side.
# RETURNS: bit_array_t - newly changed bit-array
# ARGUMENTS:
    @arg1 - bit_array_t arr
    @arg2 - unsigned int amount
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
bit_array_t BitArrRotateRight(bit_array_t arr, unsigned int amount);

/*
# DESCRIPTION: fill the specified buffer with ones and zeroes in order represented by the bit array.
# RETURNS: char* - string representing the bit-array
# ARGUMENTS:
    @arg1 - char buffer[BUFFER_SIZE]
    @arg2 - bit_array_t arr
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
    Space: O(1)
*/
char* BitArrToString(bit_array_t arr, char buffer[STRING_BUFFER_SIZE]);
#endif




