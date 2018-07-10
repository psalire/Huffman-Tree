
#ifndef MAXHEAP_H
#define MAXHEAP_H
#include <stdlib.h>
#include <stdint.h>
#include "binaryTree.h"

/* Employ binaryTree as singly linked-list to make priority queue */
typedef Node Elem;
/* struct elem {
	uint32_t val, index;
	uint8_t symbol;
	Elem *next;
}; */
typedef struct Heap {
	Elem *parent, *end;
	uint32_t size;
} Heap;

Heap *newHeap();
Elem *newElem(uint64_t, uint32_t, uint8_t);
Elem *getElemByIndex(Heap *, uint32_t);
Elem *getLeftChild(Heap *, Elem *);
Elem *getRightChild(Heap *, Elem *);
Elem *getParent(Heap *, Elem *);
void swapEnd(Heap *);
void push(Heap *, Elem *); // Insert Elem of val uint32_t and heapify/sort
Elem *pop(Heap *);
void swapParent(Heap *, Elem *);
void isHeap(Heap *, Elem *, uint8_t *);
void freeHeap(Heap *);
void printElem(Elem *);
void printHeap(Heap *);

#endif
