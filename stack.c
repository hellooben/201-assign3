/********************
CS201, Assign0, stack.c
Ben Bailey
This class implements a stack that can be used with any generic data type. The stack is built
using the doubly linked list class, and includes a doubly linked list within it's data structure.
The DLL allows stack to simplly call DLL functions to accomplish its tasks.
Stacks are last in, first out lists.
********************/

#include "stack.h"
#include "dll.h"
#include <stdlib.h>
#include <assert.h>

struct stack //the stack structure
{
    DLL *list;
    int size;
    void (*display)(void *, FILE *);
    void (*free)(void *);
};

extern STACK *
newSTACK(void (*d)(void *,FILE *),void (*f)(void *)) { //constructor for the stack, which also calls the doubly linked list constructor
    STACK *items = malloc(sizeof(STACK));
    assert(items != 0);
    items->list = newDLL(d, f);
    items->size = 0;
    items->display = d;
    items->free = f;
    return items;
}

extern void
push(STACK *items,void *value) { //inserts a node to the top of the stack
    insertDLL(items->list, 0, value);
    items->size ++;
    return;
}

extern void *
pop(STACK *items) { //removes a node from the top of the stack
    assert(items->size > 0);
    void *returnable = removeDLL(items->list, 0);
    items->size --;
    return returnable;
}

extern void *
peekSTACK(STACK *items) { //returns the data of the node on the top of the stack
    assert(items->size > 0);
    return getDLL(items->list, 0);
}

extern int
sizeSTACK(STACK *items) { //returns the number of nodes in the stack
    return items->size;
}

extern void
displaySTACK(STACK *items,FILE *fp) { //displays the stack as an array
    printf("|");
    for (int i=0; i<items->size; i++) {
        void *current = getDLL(items->list, i);
        items->display(current, fp);
        if (i != items->size-1) {printf(",");}
    }
    printf("|");
    return;
}

extern void
displaySTACKdebug(STACK *items,FILE *fp) { //displays the stack as an array, while also highlighting where the head and tail nodes are
    displayDLLdebug(items->list, fp);
    return;
}

extern void
freeSTACK(STACK *items) { //frees the stack by freeing the DLL, and then the stack itself
    freeDLL(items->list);
    free(items);
    return;
}
