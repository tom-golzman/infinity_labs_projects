#ifndef __ILRD_QUEUE__
#define __ILRD_QUEUE__

/************************************includes************************************/
#include <stddef.h>

/************************************typedef************************************/
typedef struct queue queue_t;
typedef int (*q_is_match_t)(const void* data1, const void* data2);

/************************************Functions************************************/
queue_t* QueueCreate(void);
int QueueEnqueue(queue_t* queue, void* data);
void QueueDequeue(queue_t* queue);
void* QueuePeek(const queue_t* queue);
int QueueIsEmpty(const queue_t* queue);
size_t QueueSize(const queue_t* queue);
void* QueueFind(const queue_t* queue, void* data, q_is_match_t is_match);
void QueueDestroy(queue_t* queue);

#endif
