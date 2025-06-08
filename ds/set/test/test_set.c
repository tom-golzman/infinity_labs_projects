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
void TestCreateAndDestroy();
void TestInsertAndSizeAndRemove();
void TestFind();
void TestForEach();

static int IsSameKey(const void* key1, const void* key2, const void* param);
static size_t HashFunc(const void* key, const void* param);
static int AddByParam(void* key, const void* param);

/************************************main************************************/
int main(void)
{
	TestCreateAndDestroy();
	TestInsertAndSizeAndRemove();
	TestFind();
	TestForEach();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestCreateAndDestroy()
{
	set_t* set = NULL;
	size_t capacity = 5;
	
	printf(BOLD_TITLE "\nTest: Create() & Destroy()\n" RESET);
	
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

void TestInsertAndSizeAndRemove()
{
	set_t* set = NULL;
	size_t capacity = 5;
	test_struct_t s1 = {1, 10};
	test_struct_t s2 = {2, 20};
	test_struct_t s3 = {3, 30};
	test_struct_t s4 = {1, 40};
	int insert_status = SUCCESS;
	
	printf(BOLD_TITLE "\nTest: Insert() & Size() & Remove()\n" RESET);
	
	set = SetCreate(capacity, IsSameKey, NULL, HashFunc, NULL);
	
	if (0 == SetSize(set))
	{
		printf(GREEN "Test 1 Size() PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Size() FAILED! size = %lu\n" RESET, SetSize(set));
	}
	
	insert_status |= SetInsert(set, &s1);
	insert_status |= SetInsert(set, &s2);
	insert_status |= SetInsert(set, &s3);
	insert_status |= SetInsert(set, &s4);
	
	if (SUCCESS == insert_status)
	{
		printf(GREEN "Test 2 Insert() PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Insert() FAILED! status = %d\n" RESET, insert_status);
	}
	
	if (4 == SetSize(set))
	{
		printf(GREEN "Test 3 Size() PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 3 Size() FAILED! size = %lu\n" RESET, SetSize(set));
	}
	
	SetRemove(set, &s1.key);
	if (3 == SetSize(set))
	{
		printf(GREEN "Test 4 Remove() PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 4 Remove() FAILED! size = %lu\n" RESET, SetSize(set));
	}
	
	SetDestroy(set);	
}

void TestFind()
{
	set_t* set = NULL;
	size_t capacity = 5;
	test_struct_t s1 = {1, 10};
	test_struct_t s2 = {2, 20};
	test_struct_t s3 = {3, 30};
	test_struct_t s4 = {4, 40};
	int insert_status = SUCCESS;
	test_struct_t* found = NULL;
	
	printf(BOLD_TITLE "\nTest: Find()\n" RESET);
	
	set = SetCreate(capacity, IsSameKey, NULL, HashFunc, NULL);
	
	insert_status |= SetInsert(set, &s1);
	insert_status |= SetInsert(set, &s2);
	insert_status |= SetInsert(set, &s3);
	insert_status |= SetInsert(set, &s4);
	
	if (SUCCESS != insert_status)
	{
		printf(RED "INSERT FAILED!\n" RESET);
	}
	
	found = (test_struct_t*)SetFind(set, &s3.key);
	
	if (found->key == 3)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 FAILED!\n" RESET);
	}
	
	SetRemove(set, &s3.key);
	found = (test_struct_t*)SetFind(set, &s3.key);
	
	if (found == NULL)
	{
		printf(GREEN "Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 FAILED!\n" RESET);
	}
	
	SetDestroy(set);
}

void TestForEach()
{
	set_t* set = NULL;
	size_t capacity = 5;
	test_struct_t s1 = {1, 10};
	test_struct_t s2 = {2, 20};
	test_struct_t s3 = {3, 30};
	test_struct_t s4 = {4, 40};
	int insert_status = SUCCESS;
	int foreach_status = SUCCESS;
	int param = 5;
	
	printf(BOLD_TITLE "\nTest: ForEach()\n" RESET);
	
	set = SetCreate(capacity, IsSameKey, NULL, HashFunc, NULL);
	
	insert_status |= SetInsert(set, &s1);
	insert_status |= SetInsert(set, &s2);
	insert_status |= SetInsert(set, &s3);
	insert_status |= SetInsert(set, &s4);
	
	if (SUCCESS != insert_status)
	{
		printf(RED "INSERT FAILED!\n" RESET);
	}
	
	foreach_status = SetForEach(set, AddByParam, &param);
	
	if (foreach_status == SUCCESS && s1.value == 15 && s2.value == 25 && s3.value == 35 && s4.value == 45)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 FAILED! s1=%d , s2=%d , s3=%d , s4=%d\n" RESET, s1.value, s2.value, s3.value, s4.value);
	}
	
	SetDestroy(set);
}

/************************************Private Functions************************************/
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

static int AddByParam(void* key, const void* param)
{
	((test_struct_t*)key)->value += *(int*)param;
	
    return (SUCCESS);
}
