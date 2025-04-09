#ifndef __ILRD_CIRCULAR_BUFFER__
#define __ILRD_CIRCULAR_BUFFER__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct circular_buffer cb_t;

/************************************Functions************************************/
cb_t* CBCreate(size_t capacity);
void CBDestroy(cb_t* cb);
size_t CBRead(cb_t* src, void* dest, size_t n);
size_t CBWrite(cb_t* dest, const void* src, size_t n);
size_t CBGetSize(const cb_t* cb);
size_t CBGetCapacity(const cb_t* cb);

void PrintCharBuffer(cb_t* cb, size_t size);

#endif
