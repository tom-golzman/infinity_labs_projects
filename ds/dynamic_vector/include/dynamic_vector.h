#ifndef __ILRD_DYNAMIC_VECTOR__
#define __ILRD_DYNAMIC_VECTOR__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct dvector dvector_t;

/************************************define************************************/
#define DEFAULT_CAPACITY 10

/************************************Functions************************************/
dvector_t* VectorCreate(size_t capacity, size_t elem_size);
void VectorDestroy(dvector_t* vector);
void* VectorGetAccessToElem(const dvector_t* vector, size_t index);
int VectorPushBack(dvector_t* vector, void* data);
void VectorPopBack(dvector_t* vector);
void VectorSetElem(dvector_t* vector, size_t index, void* data);
int VectorResize(dvector_t* vector, size_t new_capacity);
size_t VectorSize(const dvector_t* vector);
size_t VectorCapacity(const dvector_t* vector);
size_t VectorGetElemSize(const dvector_t* vector);
int VectorShrinkToFit(dvector_t* vector);

#endif
