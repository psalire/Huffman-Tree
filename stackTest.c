
#include "stack.h"
#include <stdio.h>

static void printStack(Stack *s) {
	if (s) {
		for (Elem *e = s->top; e; e = e->next) {
			printf("%u ", e->val);
		}
		printf("\n");
	}
}

int main() {
	Stack *s = stackInit();
	for (int i = 0; i < 300; i++) {
		stackPush(s, newElem(i, 0, '$'));
	}
	printStack(s);
	for (int i = 0; i < 100; i++) {
		Elem *e = stackPop(s);
		printf("Popped: %u\n", e->val);
		free(e);
		if (i % 3) {
			stackPush(s, newElem(i, 0, '$'));
		}
	}
	freeStack(s);
	return 0;
}
