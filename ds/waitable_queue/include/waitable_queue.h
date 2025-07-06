#ifndef __ILRD_WAITABLE_QUEUE_H__
#define __ILRD_WAITABLE_QUEUE_H__

/************************************includes************************************/

/************************************typedef************************************/
typedef struct waitable_queue wq_t;

/************************************define************************************/

/************************************Functions************************************/
wq_t* WQCreate(size_t capacity);

int WQDestroy(wq_t*);

int WQEnqueue(wq_t*, int value);

int WQDequeue(wq_t*, int* output);

#endif /*__ILRD_WAITABLE_QUEUE_H__*/
