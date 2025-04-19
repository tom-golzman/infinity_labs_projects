/**
	Written By: Tom Golzman
	Date: 16/04/2025
	Reviewed By: Sami
**/

/************************************includes************************************/
/* in header file */
/* to check that 2 iterators are of the same list */
/* also useful to get access to the default comparer function from the iterators in OListFind() */

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "oll.h"

/************************************define************************************/
#define TRUE (1)
#define FALSE (0)
#define SUCCESS (0)
#define FAIL (1)

#ifndef NDEBUG /* debug */
	#define D_TO_O_ITER_LIST(d_iter, o_list) (DllIterToOllIter(d_iter, o_list))
	#define D_TO_O_ITER_ITER(d_iter, o_iter) (DllIterToOllIter(d_iter, o_iter.list))	
#else /* release */
	#define D_TO_O_ITER_LIST(d_iter, o_list) (DllIterToOllIter(d_iter))
	#define D_TO_O_ITER_ITER(d_iter, o_iter) (DllIterToOllIter(d_iter))
#endif

#define O_TO_D_ITER(o_iter) (OllIterToDllIter(o_iter))

/************************************typedef************************************/
struct ordered_list {
    olist_comparer_t comparer; 	/* a function used to compare the values within it */
    void* comparer_param;
    dll_t* list; 				/* a dll so we would be able to use dll functions */
};

/************************************Private Functions************************************/
#ifndef NDEBUG /* debug */
	static oll_iter_t DllIterToOllIter(iter_t iter, const oll_t* list)
	{
		oll_iter_t oll_iter;

		oll_iter.iter = iter;
		oll_iter.list = list;
		
		return (oll_iter);
	}
#else /* release */
	static oll_iter_t DllIterToOllIter(iter_t iter)
	{
		oll_iter_t oll_iter;

		oll_iter.iter = iter;
		
		return (oll_iter);
	}
#endif

static iter_t OllIterToDllIter(oll_iter_t iter)
{
	return (iter.iter);
}

static int IsIterInSameList(oll_iter_t iter1, oll_iter_t iter2)
{
	return (iter1.list == iter2.list);
}

/************************************Public Functions************************************/
oll_t* OListCreate(olist_comparer_t comparer, void* param)
{
	oll_t* oll = NULL;
	dll_t* dll = NULL;
	
	assert(NULL != comparer);
	
	oll = (oll_t*)malloc(sizeof(oll_t));
	if (NULL == oll)
	{
		return (NULL);
	}
	
	dll = DListCreate();
	if (NULL == dll)
	{
		free(oll);
		oll = NULL;
		
		return (NULL);
	}
	
	oll->comparer = comparer;
	oll->comparer_param = param;
	oll->list = dll;
	
	return (oll);
}

void OListDestroy(oll_t* list)
{
	assert(NULL != list);
	
	DListDestroy(list->list);
	list->list = NULL;
	
	free(list);
	list = NULL;
}

oll_iter_t OListInsert(oll_t* list, void* data)
{
	oll_iter_t from;
    oll_iter_t to;
    oll_iter_t insert_position;
    iter_t new_node;

    assert(NULL != list);

    from = OListBegin(list);
    to = OListEnd(list);

    insert_position = OListFind(list, from, to, data);
    new_node = DListInsert(list->list, O_TO_D_ITER(insert_position), data);

    return (D_TO_O_ITER_LIST(new_node, list));
}

oll_iter_t OListRemove(oll_iter_t iter)
{
	iter_t removed = DListRemove(OllIterToDllIter(iter));
	
	return (D_TO_O_ITER_ITER(removed, iter));
}

void OListMerge(oll_t* dest, oll_t* src)
{
	oll_iter_t src_iter;
    oll_iter_t src_end;
    oll_iter_t chunk_start;
    oll_iter_t chunk_end;
    oll_iter_t insert_pos;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	src_iter = OListBegin(src);
	src_end = OListEnd(src);
	chunk_start = src_iter;
	chunk_end = src_iter;
	
	while (!OListIsSameIter(chunk_start, src_end))
    {
        insert_pos = OListFind(dest, OListBegin(dest), OListEnd(dest), OListGetData(chunk_start));

        chunk_end = chunk_start;
        
        while (!OListIsSameIter(chunk_end, src_end) && OListIsSameIter(OListFind(dest, OListBegin(dest), OListEnd(dest), OListGetData(chunk_end)), insert_pos))
        {
            chunk_end = OListNext(chunk_end);
        }
		
		DListSplice(OllIterToDllIter(insert_pos), OllIterToDllIter(chunk_start), OllIterToDllIter(chunk_end));

        chunk_start = chunk_end;
    }
}

size_t OListSize(const oll_t* list)
{
	dll_t* dll = NULL;

	assert(NULL != list);
	
	dll = list->list;
	
	return (DListSize(dll));
}

int OListIsEmpty(const oll_t* list)
{
	assert(NULL != list);
	
	return (DListIsEmpty(list->list));
}

oll_iter_t OListBegin(const oll_t* list)
{
	assert(NULL != list);
	
	return (D_TO_O_ITER_LIST(DListBegin(list->list), list));
}

oll_iter_t OListEnd(const oll_t* list)
{
	dll_t* dll = NULL;

	assert(NULL != list);
	
	dll = list->list;
	
	return (D_TO_O_ITER_LIST(DListEnd(dll), list));
}

oll_iter_t OListNext(oll_iter_t iter)
{
	iter_t dll_iter = O_TO_D_ITER(iter);
	
	return (D_TO_O_ITER_ITER(DListNext(dll_iter), iter));
}

oll_iter_t OListPrev(oll_iter_t iter)
{
	iter_t dll_iter = O_TO_D_ITER(iter);

	return (D_TO_O_ITER_ITER(DListPrev(dll_iter), iter));
}

void OListForEach(oll_iter_t from ,oll_iter_t to, olist_action_t action, void* param)
{
	iter_t dll_iter_from = O_TO_D_ITER(from);
	iter_t dll_iter_to = O_TO_D_ITER(to);
	
	assert(NULL != action);
	assert(IsIterInSameList(from, to));
	
	DListForEach(dll_iter_from, dll_iter_to, action, param);
}

oll_iter_t OListFind(const oll_t* list, oll_iter_t from, oll_iter_t to, const void* data_to_find)
{
	iter_t dll_iter_from = O_TO_D_ITER(from);
	iter_t dll_iter_to = O_TO_D_ITER(to);
	iter_t curr = dll_iter_from;
	
	assert(NULL != list);	
	assert(IsIterInSameList(from, to));

	while (!DListIsSameIter(curr, dll_iter_to))
	{
		if (0 <= list->comparer(DListGetData(curr), data_to_find, list->comparer_param))
		{
			return (D_TO_O_ITER_LIST(curr, list));
		}
		curr = DListNext(curr);
	}
	
	return (to);
}

oll_iter_t OListFindIf(oll_iter_t from, oll_iter_t to, olist_is_match_t is_match, const void* data_to_find)
{
	iter_t dll_iter_from = O_TO_D_ITER(from);
	iter_t dll_iter_to = O_TO_D_ITER(to);
	iter_t curr = dll_iter_from;

	assert(NULL != is_match);
	
	while (!DListIsSameIter(curr, dll_iter_to))
	{
		if (is_match(DListGetData(curr), data_to_find))
		{
			return (D_TO_O_ITER_LIST(curr, from.list));
		}
		curr = DListNext(curr);
	}
	
	return (to);
}

void* OListGetData(oll_iter_t iter)
{
	iter_t dll_iter = O_TO_D_ITER(iter);

	return (DListGetData(dll_iter));
}

int OListIsSameIter(oll_iter_t iter1, oll_iter_t iter2)
{
	iter_t dll_iter1 = O_TO_D_ITER(iter1);
	iter_t dll_iter2 = O_TO_D_ITER(iter2);

	assert(IsIterInSameList(iter1, iter2));

	return (DListIsSameIter(dll_iter1, dll_iter2));
}

void OListPopFront(oll_t* list)
{
	dll_t* dll = NULL;

	assert(NULL != list);
	
	dll = list->list;
	
	DListPopFront(dll);
}

void OListPopBack(oll_t* list)
{
	dll_t* dll = NULL;

	assert(NULL != list);
	
	dll = list->list;
	
	DListPopBack(dll);
}













