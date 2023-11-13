#include "List.h"
#include "assert.h"

int NodeCtor(Node* node, Elem_t value, int next, int prev, int ind) {
	node->data = value;
	node->next = next;
	node->prev = prev;
	node->ind  = ind;
	return 0;
}

int NodeDump(Node* node) {
	printf("------------------------\n");
	printf("|ind: %d|\n|data: %d|\n|next: %d|\n|prev: %d|\n", node->ind, node->data, node->next, node->prev);
	printf("------------------------\n");
	return 0;
}

int ListCtor(List* list, int capacity) {

	assert(list != nullptr);

	list->capacity = capacity + 1;
	list->list_arr = (Node*)calloc(list->capacity, sizeof(Node));

	assert(list->list_arr != nullptr);

	StackCtor(&list->free, capacity, "ListStackDump.txt");
	fopen_s(&list->logger, "Grapg_log.gv", "w");

	for (int i = list->capacity - 1; i > 0; i--) {
		StackPush(&list->free, i);
		Node node = {};
		if (i == list->capacity - 1) NodeCtor(&node, -1, -1, -1, i);
		else                         NodeCtor(&node, -1, i + 1, -1, i);
		
		list->list_arr[i] = node;
	}

	list->head = 0;
	list->tail = 0;
	list->size = 0;
	return 0;
}
int ListDtor(List* list) {
	free(list->list_arr);
	fclose(list->logger);
	StackDtor(&list->free);
	list->head     = 0;
	list->tail     = 0;
	list->capacity = 0;
	list->size     = 0;
	return 0;
}
int ListVerify(List* list) {
	if (list->head > list->tail) fprintf(stderr, "head > tail\n");
	if (list->size > list->capacity) fprintf(stderr, "size > capacity\n");
	if (list->list_arr == nullptr) fprintf(stderr, "list - nullptr\n");
	for (int i = 0; i < list->size; i++) {
		if (list->list_arr[i].next > list->size) fprintf(stderr, "range error\n");
	}
	return 0;
}
int ListDump(List* list) {
	printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
	printf("\nDUMP:\ncapacity: %d\nsize: %d\nhead: %d\ntail: %d\n\n", 
		list->capacity, list->size, list->head, list->tail);
	StackDump(&list->free);
	for (int i = 0; i < list->capacity; i++) {
		NodeDump(&list->list_arr[i]);
	}
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	return 0;
}

Node* FindNode(List* list, int ind) {
	int i = 0;
	Node* Node_var = &list->list_arr[list->head];
	while (i < ind) {
		Node_var = &list->list_arr[Node_var->next];
		i++;
	}
	return Node_var;
}

int ListDelete(List* list, int ind) {
	Node* delete_node = FindNode(list, ind);
	list->list_arr[delete_node->prev].next = list->list_arr[delete_node->ind].next;
	list->list_arr[delete_node->next].prev = list->list_arr[delete_node->ind].prev;
	list->size--;
	return 0;
}

int ListRealloc(List* list, int capacity) {
	if (capacity > list->capacity) {
		ListDump(list);
		list->list_arr = (Node*)realloc(list->list_arr, capacity * sizeof(Node));

		assert(list->list_arr != nullptr);

		ListDump(list);
		for (int i = capacity; i > list->capacity - 1; i--) {
			StackPush(&list->free, i);
			Node node = {};
			if (i == capacity)           NodeCtor(&node, -1, -1, -1, i);
			else                         NodeCtor(&node, -1, i + 1, -1, i);
	
			list->list_arr[i] = node;
		}
		list->capacity = capacity + 1;
		ListDump(list);
	}

	
	return 0;
}   

int ListUpdateHeadTail(List* list) {
	list->tail = list->list_arr[0].prev;
	list->head = list->list_arr[0].next;
	return 0;
}

int ListInsertNode(List* list, Node node, int free_id) {
	list->list_arr[free_id] = node;
	list->list_arr[node.prev].next = free_id;
	list->list_arr[node.next].prev = free_id;
	list->size++;

	ListUpdateHeadTail(list);
	return 0;
}

int GetFreePlace(List* list) {
	int free_id = STACK_ERROR;
	if (StackPop(&list->free, &free_id) == STACK_ERROR)
		return LIST_ERROR;

	return free_id;
}

int ListInsertStart(List* list, Elem_t value) {
	int free_id = GetFreePlace(list);
	if (free_id == LIST_ERROR) return LIST_ERROR;

	Node new_node = {};
	NodeCtor(&new_node, value, list->head, 0, free_id);

	ListInsertNode(list, new_node, free_id);
	return free_id;
}

int ListInsertEnd(List* list, Elem_t value) {
	int free_id = GetFreePlace(list);
	if (free_id == LIST_ERROR) return LIST_ERROR;

	Node new_node = {};
	NodeCtor(&new_node, value, 0, list->tail, free_id);

	ListInsertNode(list, new_node, free_id);
	return free_id;
}

int ListInsert(List* list, Elem_t value, int ind) {
	int free_id = GetFreePlace(list);
	if (free_id == LIST_ERROR) return LIST_ERROR;

	Node new_node = {};
	
	Node* old_node = FindNode(list, ind);
	NodeCtor(&new_node, value, old_node->ind, old_node->prev, free_id);

	ListInsertNode(list, new_node, free_id);
	return free_id;
}