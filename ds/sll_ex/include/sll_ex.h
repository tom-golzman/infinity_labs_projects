#ifndef __ILRD_SLL_EX__
#define __ILRD_SLL_EX__

/************************************typedef************************************/
typedef struct node {
	void* data;
	struct node* next;
} node_t;

/************************************Functions************************************/
node_t* Flip(node_t* head);
int HasLoop(const node_t* head);
node_t* FindIntersection(node_t* head_1, node_t* head_2);

#endif
