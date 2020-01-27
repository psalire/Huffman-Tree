
#include "stack.h"
#include <stdlib.h>

Stack *stackInit() {
    Stack *s = malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}
void stackPush(Stack *s, Elem *e) {
    if (s && e) {
        e->next = s->top;
        s->top = e;
    }
}
Elem *stackPop(Stack *s) {
    if (s) {
        Elem *ret = s->top;
        if (ret) {
            s->top = s->top->next;
            ret->next = NULL;
        }
        return ret;
    }
    return NULL;
}
void freeStack(Stack *s) {
    if (s) {
        Elem *t;
        for (Elem *e = s->top; e; e = t) {
            t = e->next;
            free(e);
        }
        free(s);
    }
}
