#pragma once

/*----------------------includes----------------------*/
#include <stddef.h> /* size_t */

/*---------------------typedef---------------------*/
typedef struct node_t* iter_t;
typedef struct list list_t;

typedef int (*action_t)(void* data, void* param);
typedef int (*is_match_t)(const void* data1, const void* data2);

/*---------------------Functions Forward Decleration---------------------*/
list_t* ListCreate(void);
void ListDestroy(list_t* list);
iter_t ListInsert(list_t* list, iter_t iter, void* data);
iter_t ListRemove(iter_t iter);
int ListIsEmpty(const list_t* list);
size_t ListSize(const list_t* list);
void* ListGetData(iter_t iter);
void ListSetData(iter_t iter, void* data);
int IterIsEqual(iter_t iter1, iter_t iter2);
iter_t ListNext(iter_t iter);
iter_t ListBegin(const list_t* list);
iter_t ListEnd(const list_t* list);
void ListForEach(iter_t from ,iter_t to, action_t action_func, void* param);
iter_t ListFind(list_t* list, iter_t from, iter_t to, is_match_t is_match, void* data_to_find);
