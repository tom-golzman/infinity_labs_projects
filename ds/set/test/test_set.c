/************************************includes************************************/
#include <stdio.h> /* printf */

#include "set.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE "\033[4m"

enum {
	TRUE = 1,
	FALSE =	0,
	SUCCESS = 0,
	FAIL = 1
};

typedef struct {
	int key;
	int value;
} test_struct_t;

/************************************Functions Forward Decleration************************************/
void TestCreate();
void TestInsertAndSize();
void TestRemove();
void TestFind();
void TestForEach();

static int IsSameKey(const void* key1, const void* key2, const void* param);
static size_t HashFunc(const void* key, const void* param);
static int MultiplyByParam(void* data, const void* param);

/************************************main************************************/
int main(void)
{
	TestCreate();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestCreate()
{
	set_t* set = NULL;
	size_t capacity = 5;
	
	printf(BOLD_TITLE "\nTest: ()\n" RESET);
	
	set = SetCreate(capacity, IsSameKey, NULL, HashFunc, NULL);
	
	if (NULL != set)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 FAILED!\n" RESET);
	}
	
	SetDestroy(set);
}

static int IsSameKey(const void* key1, const void* key2, const void* param)
{
    return (*(int*)key1 == *(int*)key2);
	
	(void) param;
}

static size_t HashFunc(const void* key, const void* param)
{
	return (((test_struct_t*)key)->key % 7777);
	
	(void)param;
}

static int MultiplyByParam(void* data, const void* param)
{
    *(int*)data *= *(int*)param;
    
    return (SUCCESS);
}
