/************************************includes************************************/
#include <stdio.h> /* printf */

#include "avl.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE "\033[4m"

#define TRUE	(1)
#define FALSE	(0)
#define SUCCESS (0)
#define FAIL	(1)

/************************************Functions Forward Decleration************************************/
void TestCreate();
void TestInsertSizeIsEmptyHeight();
void TestRemove();
void TestFind();
void TestForEach();
void TestMultiFind();
void TestMultiRemove();
void TestInsertWithoutRotations();
void HeightInsertWithRotations();
void TestRemoveWithoutRotations();
void TestRemoveWithRotations();

static void LeafRemoveTest();
static void OneChildRemoveTest();
static void TwoChildrenRemoveTest();
	
int Compare(const void* a, const void* b, void* param);
void PrintData(const void* data);
int Add(void* data, void* param);

/************************************main************************************/
int main(void)
{
	TestCreate();
	TestInsertSizeIsEmptyHeight();
	TestFind();
	TestForEach();
	TestRemove();
	TestMultiFind();
	TestInsertWithoutRotations();
	HeightInsertWithRotations();
	TestRemoveWithoutRotations();
	TestRemoveWithRotations();
	TestMultiRemove();

	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestCreate()
{
	avl_t* avl = AVLCreate(Compare, NULL);

	printf(BOLD_TITLE "\nTest: Create()\n" RESET);

	if (NULL != avl)
	{
		printf(GREEN "Create() Test 1 PASSED!\n" RESET);
	}
	
	else
	{
		printf(RED "Create() Test 1 FAILED!\n" RESET);
	}
	
	AVLDestroy(avl);
}

void TestInsertWithoutRotations()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 10, data1 = 5;
	int key2 = 5, data2 = 10;
	int status = 0;
	
	printf(BOLD_TITLE "\nTest Insert() without rotations\n" RESET);
	
	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	
	if (status == SUCCESS)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 FAILED!\n" RESET);
	}
	
	PrintTree(avl, PrintData);

	AVLDestroy(avl);
}

void HeightInsertWithRotations()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 10, data1 = 5;
	int key2 = 5, data2 = 10;
	int key3 = 0, data3 = 4;
	int key4 = 14, data4 = -2;
	int key5 = -2, data5 = 0;
	int status = 0;

	printf(BOLD_TITLE "\nTest Insert() with rotations\n" RESET);
		
	if (AVLHeight(avl) == 0)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 FAILED!\n" RESET);
	}
	
	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	status |= AVLInsert(avl, &key3, &data3);
	status |= AVLInsert(avl, &key4, &data4);
	status |= AVLInsert(avl, &key5, &data5);
	
	if (status == SUCCESS)
	{
		printf(GREEN "Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 FAILED!\n" RESET);
	}
	
	if (AVLHeight(avl) == 3)
	{
		printf(GREEN "Test 3 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 3 FAILED!\n" RESET);
	}
	
	PrintTree(avl, PrintData);

	AVLDestroy(avl);
}

void TestInsertSizeIsEmptyHeight()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int status = 0;

	int key1 = 30, data1 = 300; /* root */
	int key2 = 20, data2 = 200; /* left */
	int key3 = 40, data3 = 400; /* right */
	int key4 = 10, data4 = 100; /* LL */
	int key5 = 25, data5 = 250; /* LR */
	int key6 = 50, data6 = 500; /* RR */
	int key7 = 45, data7 = 450; /* RL */
	int key8 = 5,  data8 = 50; 
	int key9 = 35, data9 = 350;

	printf(BOLD_TITLE "\nTest: Insert All Cases\n" RESET);

	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	status |= AVLInsert(avl, &key3, &data3);
	status |= AVLInsert(avl, &key4, &data4); /* LL */
	status |= AVLInsert(avl, &key5, &data5); /* LR */
	status |= AVLInsert(avl, &key6, &data6); /* RR */
	status |= AVLInsert(avl, &key7, &data7); /* RL */
	status |= AVLInsert(avl, &key8, &data8);
	status |= AVLInsert(avl, &key9, &data9);

	if (status == SUCCESS)
	{
		printf(GREEN "Insert() Test PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Insert() Test FAILED!\n" RESET);
	}

	printf(BOLD_TITLE "\nTest: Size()\n" RESET);
	if (AVLSize(avl) == 9)
	{
		printf(GREEN "Size() test PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Size() test FAILED! Size = %lu\n" RESET, AVLSize(avl));
	}

	printf(BOLD_TITLE "\nTest: IsEmpty()\n" RESET);
	if (AVLIsEmpty(avl) == FALSE)
	{
		printf(GREEN "IsEmpty() test PASSED!\n" RESET);
	}
	else
	{
		printf(RED "IsEmpty() test FAILED!\n" RESET);
	}

	printf(BOLD_TITLE "\nTest: Height()\n" RESET);
	if (AVLHeight(avl) <= 4)
	{
		printf(GREEN "Height() test PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Height() test FAILED! Height = %lu\n" RESET, AVLHeight(avl));
	}

	printf(BOLD_TITLE "\nTree Structure:\n" RESET);
	PrintTree(avl, PrintData);

	AVLDestroy(avl);
}

void TestFind()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 1, data1 = 10;
	int key2 = 2, data2 = 20;
	int key3 = 3, data3 = 30;
	int key4 = 4;
	int* found = 0;
	
	printf(BOLD_TITLE "\nTest: Find()\n" RESET);
		
	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);
	
	found = (int*)AVLFind(avl, &key1);
	if (*found == 10)
	{
		printf(GREEN "Find() Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Find() Test 1 FAILED! data = %d\n" RESET, *found);
	}
	
	found = (int*)AVLFind(avl, &key4);
	if (found == NULL)
	{
		printf(GREEN "Find() Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Find() Test 2 FAILED!\n" RESET);
	} 
	
	AVLDestroy(avl);
}

void TestForEach()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 1, data1 = 10;
	int key2 = 2, data2 = 20;
	int key3 = 3, data3 = 30;
	int foreach_status = FAIL;
	int param = 5;
	
	printf(BOLD_TITLE "\nTest: ForEach()\n" RESET);
		
	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);

	printf(TITLE "avl before ForEach():\n" RESET);
	PrintTree(avl, PrintData);
	
	foreach_status = AVLForEach(avl, Add, &param);
	
	printf(TITLE "\navl after ForEach():\n" RESET);	
	PrintTree(avl, PrintData);
	
	if (foreach_status == SUCCESS)
	{
		printf(GREEN "ForEach() Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "ForEach() Test 1 FAILED! status = %d\n" RESET, foreach_status);
	}

	AVLDestroy(avl);
}

void TestRemove()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 10, data1 = 1;
	int key2 = 20, data2 = 2;
	int key3 = 30, data3 = 3;
	int key4 = 40, data4 = 4;
	int key5 = 50, data5 = 5;
	int key6 = 60, data6 = 6;
	int key7 = 70, data7 = 7;
	int* removed_data = NULL;
	
	printf(BOLD_TITLE "\nTest: Remove()\n" RESET);
		
	AVLInsert(avl, &key5, &data5);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key6, &data6);
	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key4, &data4);
	AVLInsert(avl, &key7, &data7);	
	AVLInsert(avl, &key3, &data3);

	printf(TITLE "avl before Remove():\n" RESET);	
	PrintTree(avl, PrintData);
	
	removed_data = (int*)AVLRemove(avl, &key7);
	
	printf(TITLE "\navl after Remove():\n" RESET);
	PrintTree(avl, PrintData);
	
	if (*removed_data == data7)
	{
		printf(GREEN "Remove() Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Remove() Test 1 FAILED! removed data = %d\n" RESET, *removed_data);
	}
	
	removed_data = (int*)AVLRemove(avl, &key2);
	
	printf(TITLE "\navl after second Remove():\n" RESET);
	PrintTree(avl, PrintData);
	
	if (*removed_data == data2)
	{
		printf(GREEN "Remove() Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Remove() Test 2 FAILED! removed data = %d\n" RESET, *removed_data);
	}
	
	removed_data = AVLRemove(avl, &key2);
	
	if (removed_data == NULL)
	{
		printf(GREEN "Remove() Test 3 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Remove() Test 3 FAILED!\n" RESET);
	}

	AVLDestroy(avl);
}

void TestMultiFind()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	size_t result = 0;
	
	int key1 = 10, data1 = 10;
	int key2 = 5, data2 = 20;
	int key3 = 0, data3 = 30;
	int key4 = 14, data4 = 40;
	int key5 = 20, data5 = 50;
	
	int search_key1 = 20;
	int search_key2 = 14;
	int search_key3 = 9;
	int search_key4 = 4;
	int search_key5 = 5;

	const void* keys[5];
	void* results[5] = {0};

	keys[0] = &search_key1;
	keys[1] = &search_key2;
	keys[2] = &search_key3;
	keys[3] = &search_key4;
	keys[4] = &search_key5;

	printf(BOLD_TITLE "\nTest: MultiFind()\n" RESET);
	
	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);
	AVLInsert(avl, &key4, &data4);
	AVLInsert(avl, &key5, &data5);
	
	result = AVLMultiFind(avl, keys, 5, results);
	
	PrintTree(avl, PrintData);
		
	if (result == 3 && results[0]==&data5 && results[1]==&data4 && results[2]==NULL && results[3]==NULL && results[4]==&data2)
	{
		printf(GREEN "MultiFind() Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "MultiFind() Test 1 FAILED!\n" RESET);
	}
	
	AVLDestroy(avl);
}

void TestMultiRemove()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	size_t result = 0;
	
	int key1 = 10, data1 = 10;
	int key2 = 5, data2 = 20;
	int key3 = 0, data3 = 30;
	int key4 = 14, data4 = 40;
	int key5 = 20, data5 = 50;
	
	int remove_key1 = 20;
	int remove_key2 = 14;
	int remove_key3 = 9;
	int remove_key4 = 4;
	int remove_key5 = 5;

	const void* keys[5];
	void* results[5] = {0};

	keys[0] = &remove_key1;
	keys[1] = &remove_key2;
	keys[2] = &remove_key3;
	keys[3] = &remove_key4;
	keys[4] = &remove_key5;

	printf(BOLD_TITLE "\nTest: MultiRemove()\n" RESET);
	
	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);
	AVLInsert(avl, &key4, &data4);
	AVLInsert(avl, &key5, &data5);
	
	result = AVLMultiRemove(avl, keys, 5, results);
	
	if (result == 3 && results[0]==&data5 && results[1]==&data4 && results[2]==NULL && results[3]==NULL && results[4]==&data2)
	{
		printf(GREEN "MultiRemove() Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "MultiRemove() Test 1 FAILED!\n" RESET);
	}
	
	AVLDestroy(avl);
}

void TestRemoveWithoutRotations()
{
	printf(BOLD_TITLE "\nTest Remove() without rotations:\n" RESET);

	LeafRemoveTest();
	OneChildRemoveTest();
	TwoChildrenRemoveTest();
}

void TestRemoveWithRotations()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int status = 0;
	
	int key1 = 10, data1 = 10;
	int key2 = 20, data2 = 20;
	int key3 = 30, data3 = 30;
	int key4 = 40, data4 = 40;
	int key5 = 50, data5 = 50;
	int key6 = 60, data6 = 60;
	int key7 = 70, data7 = 70;		

	printf(BOLD_TITLE "\nTest Remove() with rotations:\n" RESET);
		
	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	status |= AVLInsert(avl, &key3, &data3);
	status |= AVLInsert(avl, &key4, &data4);
	status |= AVLInsert(avl, &key5, &data5);
	status |= AVLInsert(avl, &key6, &data6);
	status |= AVLInsert(avl, &key7, &data7);
	
	if (FAIL == status)
	{
		printf(RED "Insert Failed!\n" RESET);
	}
	
	if (*(int*)AVLRemove(avl, &key7) == 70 && AVLSize(avl) == 6)
	{
		printf(GREEN "Remove() Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Remove() Test 1 FAILED!\n" RESET);
	}
	
	AVLDestroy(avl);
}

static void LeafRemoveTest()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int status = 0;
	
	int key1 = 10, data1 = 11;
	int key2 = 5, data2 = 6;
	
	printf(TITLE "\nTest Remove() leaf\n" RESET);
	
	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	
	if (FAIL == status)
	{
		printf(RED "Insert Failed!\n" RESET);
	}
	
	if (*(int*)AVLRemove(avl, &key2) == 6 && AVLSize(avl) == 1)
	{
		printf(GREEN "Remove() leaf Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Remove() leaf Test 1 FAILED!\n" RESET);
	}
	
	if (*(int*)AVLRemove(avl, &key1) == 11 && AVLSize(avl) == 0)
	{
		printf(GREEN "Remove() leaf Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Remove() leaf Test 2 FAILED!\n" RESET);
	}
	
	AVLDestroy(avl);
}

static void OneChildRemoveTest()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int status = 0;
	
	int key1 = 10, data1 = 11;
	int key2 = 5, data2 = 6;
	
	printf(TITLE "\nTest Remove() one child\n" RESET);
	
	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	
	if (FAIL == status)
	{
		printf(RED "Insert Failed!\n" RESET);
	}
	
	if (*(int*)AVLRemove(avl, &key1) == 11 && AVLSize(avl) == 1)
	{
		printf(GREEN "Remove() one child Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Remove() one child Test 1 FAILED!\n" RESET);
	}
	
	AVLDestroy(avl);
}

static void TwoChildrenRemoveTest()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int status = 0;
	
	int key1 = 10, data1 = 10;
	int key2 = 5, data2 = 5;
	int key3 = 0, data3 = 0;
	int key4 = 14, data4 = 14;
	int key5 = 20, data5 = 20;
	
	printf(TITLE "\nTest Remove() two children\n" RESET);
	
	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	status |= AVLInsert(avl, &key3, &data3);
	status |= AVLInsert(avl, &key4, &data4);
	status |= AVLInsert(avl, &key5, &data5);
				
	if (FAIL == status)
	{
		printf(RED "Insert Failed!\n" RESET);
	}
	
	if (*(int*)AVLRemove(avl, &key4) == 14 && AVLSize(avl) == 4)
	{
		printf(GREEN "Remove() two children Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Remove() two children Test 1 FAILED!\n" RESET);
	}
	
	AVLDestroy(avl);
}

int Compare(const void* key1, const void* key2, void* param)
{
    (void)param;
    return (*(int*)key1 - *(int*)key2);
}

void PrintData(const void* data)
{
	printf("%d\n", *(const int*)data);
}

int Add(void* data, void* param)
{
	*(int*)data += *(int*)param;
	
	return (SUCCESS);
}

