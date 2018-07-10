
#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H
#include <stdint.h>

/* Singly-linked-list with index */
typedef struct node Node;
struct node {
	uint32_t val, index;
	Node *next;
	uint8_t symbol;
};

#endif
