#include <stdio.h>

/* Exercise 1 */

#ifndef SWAP_H
#define SWAP_H

void Swap (int* ptr1, int* ptr2);

#endif /* SWAP_H */

/* Exercise 2 */
#ifndef COPY_ARRAY_H
#define COPY_ARRAY_H

void* CpyArray(void* src, size_t elem_size, size_t num_elem, void* dest);

#endif /* COPY_ARRAY_H*/


/* Exercise 3 */
#ifndef PRINT_ADDRESS_H
#define PRINT_ADDRESS_H

void PrintAddress();

#endif /* PRINT_ADDRESS_H */

/* Exercise 4 */
#ifndef SWAP_SIZE_T_H
#define SWAP_SIZE_T_H

void SwapSizeT(size_t* a, size_t* b);
void SwapPointers(size_t** ptr1, size_t** ptr2);

#endif /* SWAP_SIZE_T_H */






