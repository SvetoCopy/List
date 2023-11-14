#include "GraphVizDump.h"

int PrintInfoNode(List* list) {
	fprintf(list->logger, "info [shape=record, style=filled, fillcolor=\"darkseagreen4\","\
		"label = \"SIZE : %d | CAPACITY : %d\","\
		"fontcolor = \"black\", fontsize = 25];\n", list->size, list->capacity);
	return 0;
}

int DefineListNode(List* list) {
	Node node = list->list_arr[0];
	fprintf(list->logger, "\"node%d\" [shape=Mrecord, style=filled, fillcolor=darkseagreen4,"\
		"label = \"ind: %d | data: %d | next: %d | prev: %d\"];\n",
		node.ind, node.ind, node.data, node.next, node.prev);

	for (int i = 1; i < list->capacity; i++) {
		node = list->list_arr[i];
		fprintf(list->logger, "\"node%d\" [shape=Mrecord, style=filled, fillcolor=darkgreen,"\
			"label = \"ind: %d | data: %d | next: %d | prev: %d\"];\n",
			node.ind, node.ind, node.data, node.next, node.prev);
	}

	return 0;
}

int ConnectListNodes(List* list) {
	Node node1 = list->list_arr[list->head];

	while (node1.next != list->tail) {
		int next = node1.next;
		Node node2 = list->list_arr[next];
		fprintf(list->logger, "node%d->node%d;\n", node1.ind, node2.ind);
		node1 = node2;
	}
	int next = node1.next;
	Node node2 = list->list_arr[next];
	fprintf(list->logger, "node%d->node%d;\n", node1.ind, node2.ind);

	for (int i = list->free.size - 1; i > 0; i--) {
		printf("%d->%d\n", list->free.data[i], list->free.data[i - 1]);
		fprintf(list->logger, "node%d->node%d;\n", list->free.data[i], list->free.data[i - 1]);
	}
	return 0;
}

int AlignListNodes(List* list) {
	Node node = {};
	for (int i = 0; i < list->capacity - 1; i++) {
		node = list->list_arr[i];
		fprintf(list->logger, "node%d -> ", node.ind);
	}
	node = list->list_arr[list->capacity - 1];
	fprintf(list->logger, "node%d [arrowsize = 0.0, weight = 10000, color = \"gray37\"];\n", node.ind);
	return 0;
}

int PrintHeadTail(List* list) {
	//head[shape = rect, label = "HEAD", color = yellow, fillcolor = "#7293ba", style = filled];
	//tail[shape = rect, label = "TAIL", color = yellow, fillcolor = "#7293ba", style = filled];
	//free_head[shape = rect, label = "FREE_HEAD", color = yellow, fillcolor = "#7293ba", style = filled];
	//head->node1[color = yellow];
	//tail->node3[color = yellow];
	//free_head->node2[color = yellow];
	fprintf(list->logger, "head[shape = rect, label = \"HEAD\", "\
		"color = black, fillcolor = \"#7293ba\", style = filled];\n");
	fprintf(list->logger, "tail[shape = rect, label = \"TAIL\", "\
		"color = black, fillcolor = \"#7293ba\", style = filled];\n");
	fprintf(list->logger, "free_head[shape = rect, label = \"FREE_HEAD\", "\
		"color = black, fillcolor = \"#7293ba\", style = filled]; \n");
	fprintf(list->logger, "head->node%d[color = yellow];\n", list->head);
	fprintf(list->logger, "tail->node%d[color = yellow];\n", list->tail);
	int free_head = 0;
	StackPop(&list->free, &free_head);
	StackPush(&list->free, free_head);
	fprintf(list->logger, "free_head->node%d[color = yellow];\n", free_head);
	return 0;
}

int PrintList(List* list) {
	fprintf(list->logger, "digraph List \n{ bgcolor=\"gray37\"; node[margin = \"0.01\"];\n rankdir = \"LR\";\n");

	PrintInfoNode(list);
	AlignListNodes(list);
	DefineListNode(list);
	ConnectListNodes(list);
	PrintHeadTail(list);

	fprintf(list->logger, "}");
	return 0;
}
