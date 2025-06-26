#ifndef __ILRD_100K_THREADS_H__
#define __ILRD_100K_THREADS_H__

/************************************define************************************/
enum { NUM_THREADS = 11 };

/************************************Functions************************************/
void InitArray(int* arr, int size);
size_t WorkLoad(int* arr, size_t size);
size_t SimpleWorkLoad();

#endif /*__ILRD_100K_THREADS_H__*/
