#include "GraphVizDump.h"
#define BG_COLOR              "cornsilk1"
#define HEADNTAIL_ARROW_COLOR "darksalmon"
#define HEADNTAIL_COLOR		  "darksalmon"
#define NODE_ARROW_COLOR	  "black"
#define FONT_COLOR			  "black"
#define INFONODE_COLOR		  "darksalmon"
#define LISTNODE_COLOR		  "bisque"
#define FREENODE_COLOR		  "darkolivegreen1"

int PrintInfoNode(List* list) {
	fprintf(list->logger, "info [shape=record, style=filled, fillcolor=\"" INFONODE_COLOR "\", "
		"label = \"SIZE : %d | CAPACITY : %d\","
		"fontcolor = \"" FONT_COLOR "\", fontsize = 25];\n", list->size, list->capacity);
	return 0;
}

int DefineListNode(List* list) {
	Node node = list->list_arr[0];
	fprintf(list->logger, "\"node%d\" [shape=Mrecord, style=filled, fillcolor=" INFONODE_COLOR ", "
		"label = \"ind: %d | data: %d | next: %d | prev: %d\"];\n",
		node.ind, node.ind, node.data, node.next, node.prev);

	for (int i = 1; i < list->capacity; i++) {
		node = list->list_arr[i];
		if (node.prev != LIST_POISON) {
			fprintf(list->logger, "\"node%d\" [shape=Mrecord, style=filled, fillcolor=" LISTNODE_COLOR ", "
				"label = \"ind: %d | data: %d | next: %d | prev: %d\"];\n",
				node.ind, node.ind, node.data, node.next, node.prev);
		}
		else {
			fprintf(list->logger, "\"node%d\" [shape=Mrecord, style=filled, fillcolor=" FREENODE_COLOR ", "
				"label = \"ind: %d | data: %d | next: %d | prev: %d\"];\n",
				node.ind, node.ind, node.data, node.next, node.prev);
		}
		
	}

	return 0;
}

int ConnectListNodes(List* list) {
	Node node1 = list->list_arr[list->head];

	while (node1.ind != list->tail) {
		int next = node1.next;
		Node node2 = list->list_arr[next];
		fprintf(list->logger, "node%d->node%d;\n", node1.ind, node2.ind);
		node1 = node2;
	}


	node1 = list->list_arr[list->free];
	int next = node1.next;
	while (next != LIST_POISON) {
		Node node2 = list->list_arr[next];
		fprintf(list->logger, "node%d->node%d;\n", node1.ind, node2.ind);
		node1 = node2;
		next = node1.next;
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
	fprintf(list->logger, "node%d [arrowsize = 0.0, weight = 10000, color = \"" BG_COLOR "\"];\n", node.ind);
	return 0;
}

// N
int PrintHeadTail(List* list) {
	
	fprintf(list->logger, "head[shape = rect, label = \"HEAD\", "
		"fillcolor = \"" HEADNTAIL_COLOR "\", style = filled];\n");
	fprintf(list->logger, "tail[shape = rect, label = \"TAIL\", "
		"fillcolor = \"" HEADNTAIL_COLOR "\", style = filled];\n");
	fprintf(list->logger, "free_head[shape = rect, label = \"FREE_HEAD\", "
		"fillcolor = \"" HEADNTAIL_COLOR "\", style = filled]; \n");
	fprintf(list->logger, "head->node%d[color = " HEADNTAIL_ARROW_COLOR "]; \n", list->head);
	fprintf(list->logger, "tail->node%d[color = " HEADNTAIL_ARROW_COLOR "];\n", list->tail);

	int free_head = list->free;
	fprintf(list->logger, "free_head->node%d[color = " HEADNTAIL_ARROW_COLOR "]; \n", free_head);
	return 0;
}

int PrintList(List* list) {
	fprintf(list->logger, "digraph List \n{ bgcolor=\"" BG_COLOR "\"; node[margin = \"0.01\"];\n rankdir = \"LR\";\n");

	PrintInfoNode(list);
	AlignListNodes(list);
	DefineListNode(list);
	ConnectListNodes(list);
	PrintHeadTail(list);

	fprintf(list->logger, "}");
	return 0;
}
