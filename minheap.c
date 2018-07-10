
#include "minheap.h"
#include <stdio.h>

Heap *newHeap() {
	Heap *new = (Heap *) malloc(sizeof(Heap));
	if (new) {
		new->parent = NULL;
		new->end  = new->parent;
		new->size = 0;
	}
	else {
		puts("newHeap() malloc fail");
		exit(-1);
	}
	return new;
}
Elem *newElem(uint64_t val, uint32_t index, uint8_t sym) {
	Elem *new = (Elem *) malloc(sizeof(Elem));
	if (new) {
		new->val = val;
		new->index = index;
		new->symbol = sym;
		new->next = NULL;
		new->left = NULL;
		new->right = NULL;
	}
	else {
		puts("newElem() malloc fail");
		exit(-1);
	}
	return new;
}
Elem *getElemByIndex(Heap *h, uint32_t index) {
	if (!h || index >= h->size) {
		return NULL;
	}
	Elem *temp = h->parent;
	if (temp && temp->index != index) {
		for (; temp && temp->index < h->size && temp->index != index; temp = temp->next);
		if (temp && temp->index >= h->size) {
			return NULL;
		}
	}
	return temp;
}
Elem *getLeftChild(Heap *h, Elem *n) {
	return getElemByIndex(h, (n->index << 1) + 1);
}
Elem *getRightChild(Heap *h, Elem *n) {
	return getElemByIndex(h, (n->index << 1) + 2);
}
Elem *getParent(Heap *h, Elem *n) {
	if (n && n->index) {
		return getElemByIndex(h, (n->index - 1) >> 1);
	}
	return NULL;
}
void swapEnd(Heap *h) {
	if (h) {
		Elem *t = getElemByIndex(h, h->end->index - 1);
		if (t) {
			if (t->index) {
				h->end->index = 0;
				h->parent->index = h->size - 1;
				h->end->next = h->parent->next;
				h->parent->next = NULL;
				t->next = h->parent;
				h->parent = h->end;
				h->end = getElemByIndex(h, h->size - 1);
			}
			else {
				h->end->next = h->parent;
				h->parent->next = NULL;
				Elem *t = h->parent;
				h->parent = h->end;
				h->end = t;
				h->parent->index = 0;
				h->end->index = 1;
			}
		}
	}
}
void swapParent(Heap *h, Elem *n) {
	Elem *p = getParent(h, n);
	if (h && p) {
		if (p != h->parent) {
			Elem *temp = p->next;
			Elem *pp = getElemByIndex(h, p->index - 1);
			Elem *np = getElemByIndex(h, n->index - 1);
			pp->next = n;
			p->next = n->next;
			np->next = p;
			n->next = temp;
		}
		else {
			if (p->next == n) {
				p->next = n->next;
				n->next = p;
			}
			else {
				Elem *np = getElemByIndex(h, n->index - 1);
				Elem *pTemp = p->next;
				Elem *nTemp  = n->next;
				n->next = pTemp;
				p->next = nTemp;
				np->next = p;
			}
		}
		uint32_t childIndex = n->index;
		n->index = p->index;
		p->index = childIndex;
		if (p->index == h->size - 1) {
			h->end = p;
		}
		if (n->index == 0) {
			h->parent = n;
		}
	}
}
void push(Heap *h, Elem *n) {
	if (h) {
		n->index = h->size;
		if (h->size++) {
			h->end->next = n;
			h->end = n;
			Elem *p = getParent(h, n);
			while (p && n && p->val > n->val) {
				uint32_t pIndex = p->index;
				swapParent(h, n);
				n = getElemByIndex(h, pIndex);
				p = getParent(h, n);
			}
		}
		else {
			h->end = n;
			h->parent = h->end;
		}
	}
}
Elem *pop(Heap *h) {
	if (!h) {
		puts("Error: Heap NULL");
		exit(-1);
	}
	if (h->size == 1) {
		Elem *retVal = h->parent;
		h->parent = NULL;
		h->end = NULL;
		h->size--;
		return retVal;
	}
	else if (h->size) {
		Elem *retVal = h->parent;
		swapEnd(h);
		Elem *t = getElemByIndex(h, h->end->index - 1);
		h->end = t;
		if (t) {
			t->next = NULL;
		}
		Elem *p = h->parent;
		Elem *l = getLeftChild(h, p);
		Elem *r = getRightChild(h, p);
		h->size--;
		uint32_t lIndex, rIndex;
		while (p && ((l && p->val > l->val) || (r && p->val > r->val))) {
			if (l) {
				lIndex = l->index;
			}
			if (r) {
				rIndex = r->index;
			}
			if (l && r && p->val > l->val && p->val > r->val) {
				if (l->val < r->val) {
					swapParent(h, l);
					p = getElemByIndex(h, lIndex);
				}
				else {
					swapParent(h, r);
					p = getElemByIndex(h, rIndex);
				}
			}
			else if (l && p->val > l->val) {
				swapParent(h, l);
				p = getElemByIndex(h, lIndex);
			}
			else if (r && p->val > r->val) {
				swapParent(h, r);
				p = getElemByIndex(h, rIndex);
			}
			if (p) {
				l = getLeftChild(h, p);
				r = getRightChild(h, p);
			}
		}
		return retVal;
	}
	else {
		return NULL;
	}
}
void printElem(Elem *e) {
	switch(e->symbol) {
		case ' ': {
			printf("%u : (%lu, SPACE)\n", e->index, e->val);
			break;
		}
		case '\n': {
			printf("%u : (%lu, NL)\n", e->index, e->val);
			break;
		}
		case '\r': {
			printf("%u : (%lu, CR)\n", e->index, e->val);
			break;
		}
		case '\t': {
			printf("%u : (%lu, TAB)\n", e->index, e->val);
			break;
		}
		default:{
			printf("%u : (%lu, %c)\n", e->index, e->val, e->symbol);
			break;
		}
	}
}
void printHeap(Heap *h) {
	if (!h) {
		puts("HEAP NULL");
		return;
	}
	if (!h->parent) {
		puts("parent NULL");
		return;
	}
	for (Elem *e = h->parent; e; e = e->next) {
		printElem(e);
		if (e == h->end) {
			puts("END");
		}
	}
}
void freeHeap(Heap *h) {
	if (h && h->parent) {
		for (Elem *i = h->parent; i;) {
			Elem *t = i->next;
			free(i);
			i = NULL;
			i = t;
		}
		free(h);
		h = NULL;
	}
}
