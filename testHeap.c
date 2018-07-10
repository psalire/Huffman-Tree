
#include "minheap.h"
#include <stdio.h>

static void printLL(Heap *ll) {
	Elem *t = ll->parent;
	if (t) {
		for (; t; t = t->right) {
			printf("%u: %u\n", t->index, t->val);
			if (t == ll->end) {
				puts("End");
			}
		}
	}
	else {
		puts("Parent NULL");
	}
	printf("Size : %u\n", ll->size);
}
static void printLeftChild(Heap *ll, Elem *i) {
	if (i) {
		Elem *t = getLeftChild(ll, i);
		if (t) {
			printf("Left child of (%u : %u) : (%u : %u)\n", i->index, i->val, t->index, t->val);
		}
		else {
			printf("Left child of (%u : %u) : NULL\n", i->index, i->val);
		}
	}
}
static void printRightChild(Heap *ll, Elem *i) {
	if (i) {
		Elem *t = getRightChild(ll, i);
		if (t) {
			printf("Right child of (%u : %u) : (%u : %u)\n", i->index, i->val, t->index, t->val);
		}
		else {
			printf("Right child of (%u : %u) : NULL\n", i->index, i->val);
		}
	}
}
static void printParents(Heap *ll, Elem *i) {
	if (i) {
		Elem *t = getParent(ll, i);
		if (t) {
			printf("Parent of (%u : %u) : (%u : %u)\n", i->index, i->val, t->index, t->val);
		}
		else {
			printf("Parent of (%u : %u) : NULL\n", i->index, i->val);
		}
	}
}

int main() {
	printf("Heap size: ");
	uint32_t s, max;
	scanf("%u", &s);
	printf("Max val: ");
	scanf("%u", &max);
	srand(0);
	Heap *h = newHeap();
	for (uint32_t i = 0; i < s; i++) {
		srand(i);
		push(h, newElem(rand() % max, h->size, (i % 95) + 32));
	}
	/* printHeap(h);
	for (uint8_t i = 0; i < h->size; i++) {
		printLeftChild(h, getElemByIndex(h, i));
		printRightChild(h, getElemByIndex(h, i));
	}
	uint8_t heap;
	isHeap(h, h->parent, &heap);
	printf("Heap? : %u\n", heap);
	puts("popping"); */
	uint32_t size = h->size;
	for (uint32_t i = 0; i < size; i++) {
		Elem *n = pop(h);
		printf("Pop : (%c, %u)\n", n->symbol, n->val);
		free(n);
	}
	freeHeap(h);
	return 0;
}
