#include "List.h"
#include "assert.h"
#define PREV(indx) list->list_arr[indx].prev
#define NEXT(indx) list->list_arr[indx].next
#define NODE(indx)  list->list_arr[indx]

int NodeCtor(Node* node, ListElem_t value, int next, int prev, int ind) {
	node->data = value;
	node->next = next;
	node->prev = prev;
	node->ind  = ind;
	return 0;
}

int NodeDump(Node* node) {
	printf("------------------------\n");
	printf("|ind: %d|\n|data: %d|\n|next: %d|\n|prev: %d|\n", 
		node->ind, node->data, node->next, node->prev);
	printf("------------------------\n");
	return 0;
}

int ListCtor(List* list, int capacity) {

	assert(list != nullptr);

	list->capacity = capacity + 1;
	list->list_arr = (Node*)calloc(list->capacity, sizeof(Node));

	assert(list->list_arr != nullptr);

	fopen_s(&list->logger, "Grapg_log.gv", "w");
	list->free = 1;

	for (int i = list->capacity - 1; i > 0; i--) {
		Node node = {};
		if (i == list->capacity - 1) NodeCtor(&node, LIST_POISON, LIST_POISON, LIST_POISON, i);
		else                         NodeCtor(&node, LIST_POISON, i + 1, LIST_POISON, i);
		
		NODE(i) = node;
	}

	list->head = 0;
	list->tail = 0;
	list->size = 0;
	return 0;
}

int ListDtor(List* list) {
	free(list->list_arr);
	fclose(list->logger);
	
	list->free     = LIST_POISON;
	list->head     = LIST_POISON;
	list->tail     = LIST_POISON;
	list->capacity = LIST_POISON;
	list->size     = LIST_POISON;
	return 0;
}

int ListVerify(List* list) {
	if (list->head > list->tail)     fprintf(stderr, "head > tail\n");
	if (list->size > list->capacity) fprintf(stderr, "size > capacity\n");
	if (list->list_arr == nullptr)   fprintf(stderr, "list - nullptr\n");

	for (int i = 0; i < list->size; i++) {
		if (NEXT(i) > list->size) fprintf(stderr, "range error\n");
	}
	return 0;
}
int ListDump(List* list) {
	printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
	printf("\nDUMP:\ncapacity: %d\nsize: %d\nhead: %d\ntail: %d\nfree: %d\n\n", 
		list->capacity, list->size, list->head, list->tail, list->free);
	
	for (int i = 0; i < list->capacity; i++) {
		NodeDump(&NODE(i));
	}
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	return 0;
}

Node* FindNode(List* list, int ind) {
	int i = 0;
	Node* Node_var = &NODE(list->head);
	while (i < ind) {
		Node_var = &NODE(Node_var->next);
		i++;
	}
	return Node_var;
}

int ListDeleteNode(List* list, Node delete_node) {
	NEXT(delete_node.prev) = NEXT(delete_node.ind);
	PREV(delete_node.next) = PREV(delete_node.ind);
	list->size--;
	return 0;
}

int ListDelete(List* list, int arr_ind) {
	Node delete_node = NODE(arr_ind);
	ListDeleteNode(list, delete_node);

	NEXT(arr_ind) = list->free;
	PREV(arr_ind) = LIST_POISON;
	list->free = arr_ind;

	return 0;
}

int ListReallocUp(List* list, int capacity) {
	
	list->list_arr = (Node*)realloc(list->list_arr, (capacity + 1) * sizeof(Node));

	assert(list->list_arr != nullptr);

	
	for (int i = capacity; i > list->capacity - 1; i--) {
		Node node = {};
		if (i == capacity) NodeCtor(&node, LIST_POISON, list->free, LIST_POISON, i);
		else               NodeCtor(&node, LIST_POISON, i + 1, LIST_POISON, i);
			
		NODE(i) = node;
	}
	list->free = list->capacity;
	list->capacity = capacity + 1;
	
	return 0;
}   

int ListUpdateHeadTail(List* list) {
	list->tail = PREV(0);
	list->head = NEXT(0);
	return 0;
}

int ListInsertNode(List* list, Node node, int free_id) {
	NODE(free_id) = node;
	NEXT(node.prev) = free_id;
	PREV(node.next) = free_id;
	list->size++;

	ListUpdateHeadTail(list);
	return 0;
}

int GetFreePlace(List* list) {
	int free_id = list->free;

	if (free_id == LIST_POISON) return LIST_ERROR;

	list->free = NEXT(free_id);
	
	return free_id;
}

int ListInsertStart(List* list, ListElem_t value) {
	if (list->size > list->capacity / 2) ListReallocUp(list, list->capacity * 2);

	int free_id = GetFreePlace(list);
	if (free_id == LIST_ERROR) return LIST_ERROR;
	
	Node new_node = {};
	NodeCtor(&new_node, value, list->head, 0, free_id);

	ListInsertNode(list, new_node, free_id);
	return free_id;
}

int ListInsertEnd(List* list, ListElem_t value) {
	if (list->size > list->capacity / 2) ListReallocUp(list, list->capacity * 2);

	int free_id = GetFreePlace(list);
	if (free_id == LIST_ERROR) return LIST_ERROR;

	Node new_node = {};
	NodeCtor(&new_node, value, 0, list->tail, free_id);

	ListInsertNode(list, new_node, free_id);
	return free_id;
}

int ListInsertBefore(List* list, ListElem_t value, int arr_ind) {
	if (list->size > list->capacity / 2) ListReallocUp(list, list->capacity * 2);

	int free_id = GetFreePlace(list);
	if (free_id == LIST_ERROR) return LIST_ERROR;

	Node new_node = {};
	
	Node old_node = NODE(arr_ind);
	NodeCtor(&new_node, value, old_node.ind, old_node.prev, free_id);

	ListInsertNode(list, new_node, free_id);
	return free_id;
}