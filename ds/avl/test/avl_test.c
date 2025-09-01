#include <stdio.h>

#include "avl.h"
#include "test_utils.h"

/* Function declarations */
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

/* Main function */
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
	return 0;
}

void TestCreate()
{
	avl_t* avl = AVLCreate(Compare, NULL);

	TITLE("Test: Create()");
	RUN_TEST("AVLCreate returns non-NULL", NULL != avl);

	AVLDestroy(avl);
}

void TestInsertWithoutRotations()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 10, data1 = 5;
	int key2 = 5, data2 = 10;
	int status = 0;

	TITLE("Test Insert() without rotations");

	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);

	RUN_TEST("Insert without rotations returns SUCCESS", status == SUCCESS);

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

	TITLE("Test Insert() with rotations");

	RUN_TEST("Initial height is 0", AVLHeight(avl) == 0);

	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	status |= AVLInsert(avl, &key3, &data3);
	status |= AVLInsert(avl, &key4, &data4);
	status |= AVLInsert(avl, &key5, &data5);

	RUN_TEST("All inserts return SUCCESS", status == SUCCESS);
	RUN_TEST("Final height is 3", AVLHeight(avl) == 3);

	PrintTree(avl, PrintData);
	AVLDestroy(avl);
}

void TestInsertSizeIsEmptyHeight()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int status = 0;

	int key1 = 30, data1 = 300;
	int key2 = 20, data2 = 200;
	int key3 = 40, data3 = 400;
	int key4 = 10, data4 = 100;
	int key5 = 25, data5 = 250;
	int key6 = 50, data6 = 500;
	int key7 = 45, data7 = 450;
	int key8 = 5,  data8 = 50;
	int key9 = 35, data9 = 350;

	TITLE("Test: Insert All Cases");

	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	status |= AVLInsert(avl, &key3, &data3);
	status |= AVLInsert(avl, &key4, &data4);
	status |= AVLInsert(avl, &key5, &data5);
	status |= AVLInsert(avl, &key6, &data6);
	status |= AVLInsert(avl, &key7, &data7);
	status |= AVLInsert(avl, &key8, &data8);
	status |= AVLInsert(avl, &key9, &data9);

	RUN_TEST("Insert returned SUCCESS", status == SUCCESS);
	RUN_TEST("Size() is 9", AVLSize(avl) == 9);
	RUN_TEST("IsEmpty() returns FALSE", AVLIsEmpty(avl) == FALSE);
	RUN_TEST("Height() <= 4", AVLHeight(avl) <= 4);

	TITLE("Tree Structure:");
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
	int* found = NULL;

	TITLE("Test: Find()");

	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);

	found = (int*)AVLFind(avl, &key1);
	RUN_TEST("Find existing key returns correct data", *found == 10);

	found = (int*)AVLFind(avl, &key4);
	RUN_TEST("Find non-existent key returns NULL", found == NULL);

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

	TITLE("Test: ForEach()");

	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);

	TITLE("AVL before ForEach():");
	PrintTree(avl, PrintData);

	foreach_status = AVLForEach(avl, Add, &param);

	TITLE("AVL after ForEach():");
	PrintTree(avl, PrintData);

	RUN_TEST("ForEach returned SUCCESS", foreach_status == SUCCESS);

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

	TITLE("Test: Remove()");

	AVLInsert(avl, &key5, &data5);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key6, &data6);
	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key4, &data4);
	AVLInsert(avl, &key7, &data7);
	AVLInsert(avl, &key3, &data3);

	TITLE("AVL before Remove():");
	PrintTree(avl, PrintData);

	removed_data = (int*)AVLRemove(avl, &key7);
	TITLE("AVL after Remove key 70:");
	PrintTree(avl, PrintData);
	RUN_TEST("Remove key 70 returned correct data", *removed_data == data7);

	removed_data = (int*)AVLRemove(avl, &key2);
	TITLE("AVL after Remove key 20:");
	PrintTree(avl, PrintData);
	RUN_TEST("Remove key 20 returned correct data", *removed_data == data2);

	removed_data = AVLRemove(avl, &key2);
	RUN_TEST("Removing nonexistent key returns NULL", removed_data == NULL);

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

	TITLE("Test: MultiFind()");

	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);
	AVLInsert(avl, &key4, &data4);
	AVLInsert(avl, &key5, &data5);

	result = AVLMultiFind(avl, keys, 5, results);

	PrintTree(avl, PrintData);

	RUN_TEST("MultiFind found correct values",
		result == 3 &&
		results[0] == &data5 &&
		results[1] == &data4 &&
		results[2] == NULL &&
		results[3] == NULL &&
		results[4] == &data2);

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

	TITLE("Test: MultiRemove()");

	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);
	AVLInsert(avl, &key4, &data4);
	AVLInsert(avl, &key5, &data5);

	result = AVLMultiRemove(avl, keys, 5, results);

	RUN_TEST("MultiRemove removed correct elements",
		result == 3 &&
		results[0] == &data5 &&
		results[1] == &data4 &&
		results[2] == NULL &&
		results[3] == NULL &&
		results[4] == &data2);

	AVLDestroy(avl);
}

void TestRemoveWithoutRotations()
{
	TITLE("Test Remove() without rotations:");

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

	TITLE("Test Remove() with rotations:");

	status = AVLInsert(avl, &key1, &data1);
	status |= AVLInsert(avl, &key2, &data2);
	status |= AVLInsert(avl, &key3, &data3);
	status |= AVLInsert(avl, &key4, &data4);
	status |= AVLInsert(avl, &key5, &data5);
	status |= AVLInsert(avl, &key6, &data6);
	status |= AVLInsert(avl, &key7, &data7);

	RUN_TEST("Insert succeeded", status == SUCCESS);

	RUN_TEST("Remove key 70 and check size",
		*(int*)AVLRemove(avl, &key7) == 70 &&
		AVLSize(avl) == 6);

	AVLDestroy(avl);
}

static void LeafRemoveTest()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 10, data1 = 11;
	int key2 = 5, data2 = 6;

	TITLE("Test Remove() leaf");

	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);

	RUN_TEST("Remove leaf (5)",
		*(int*)AVLRemove(avl, &key2) == 6 &&
		AVLSize(avl) == 1);

	RUN_TEST("Remove root (10)",
		*(int*)AVLRemove(avl, &key1) == 11 &&
		AVLSize(avl) == 0);

	AVLDestroy(avl);
}

static void OneChildRemoveTest()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 10, data1 = 11;
	int key2 = 5, data2 = 6;

	TITLE("Test Remove() one child");

	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);

	RUN_TEST("Remove node with one child (10)",
		*(int*)AVLRemove(avl, &key1) == 11 &&
		AVLSize(avl) == 1);

	AVLDestroy(avl);
}

static void TwoChildrenRemoveTest()
{
	avl_t* avl = AVLCreate(Compare, NULL);
	int key1 = 10, data1 = 10;
	int key2 = 5, data2 = 5;
	int key3 = 0, data3 = 0;
	int key4 = 14, data4 = 14;
	int key5 = 20, data5 = 20;

	TITLE("Test Remove() two children");

	AVLInsert(avl, &key1, &data1);
	AVLInsert(avl, &key2, &data2);
	AVLInsert(avl, &key3, &data3);
	AVLInsert(avl, &key4, &data4);
	AVLInsert(avl, &key5, &data5);

	RUN_TEST("Remove node with two children (14)",
		*(int*)AVLRemove(avl, &key4) == 14 &&
		AVLSize(avl) == 4);

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
	return SUCCESS;
}
