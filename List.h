#ifndef DED_LIST
#define DED_LIST
#include "RuzalLib/stack.h"

typedef int ListElem_t;
const int LIST_ERROR = -1;

struct Node {
	int ind;
	ListElem_t data;
	int next;
	int prev;
};

struct List {
	Node* list_arr;
	Stack free;
	int size;
	int capacity;
	int head;
	int tail;
	FILE* logger;
};
int NodeCtor(Node* node, ListElem_t value, int next, int prev, int ind);

int ListCtor(List* list, int capacity);
int ListDtor(List* list);
int ListVerify(List* list);
int ListDump(List* list);

int ListInsertStart(List* list, ListElem_t value);
int ListInsertEnd(List* list, ListElem_t value);
int ListInsertBefore(List* list, ListElem_t value, int arr_ind);
int ListDelete(List* list, int arr_ind);
int ListReallocUp(List* list, int capacity);
#endif