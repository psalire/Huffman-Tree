
#ifndef _STACK_H
#define _STACK_H
#include "binaryTree.h"
#include "minheap.h"

/* Elem and function in minheap.h */
typedef struct Stack {
	Elem *top;
} Stack;

Stack *stackInit();
void stackPush(Stack *, Elem *);
Elem *stackPop(Stack *);
void freeStack(Stack *);

#endif
