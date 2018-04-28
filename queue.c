/********************
CS201, Assign0, queue.c
Ben Bailey
This class implements a queue that can be used with any generic data type. The queue is built
using the singly linked list class, and includes a singly linked list within its data structure.
The SLL allows stack to simplly call SLL functions to accomplish its tasks.
Queues are first in, first out lists.
********************/

#include "queue.h"
#include "sll.h"
#include <stdlib.h>
#include <assert.h>

struct queue //the queue structure
{
    SLL *list;
    int size;
    void (*display)(void *, FILE *);
    void (*free)(void *);
};

extern QUEUE *
newQUEUE(void (*d)(void *,FILE *),void (*f)(void *)) { //constructor for the queue, which also calls the singly linked list constructor
    QUEUE *items = malloc(sizeof(QUEUE));
    assert(items != 0);
    items->list = newSLL(d, f);
    items->size = 0;
    items->display = d;
    items->free = f;
    return items;
}

extern void
enqueue(QUEUE *items,void *value) { //inserts a node to the end of the queue
    insertSLL(items->list, items->size, value);
    items->size ++;
    return;
}

extern void *
dequeue(QUEUE *items) { //removes a node from the beginning of the queue
    assert(items->size > 0);
    void *returnable = removeSLL(items->list, 0);
    items->size --;
    return returnable;
}

extern void *
peekQUEUE(QUEUE *items) { //returns the data of the node in the queue ready to be dequeued
    assert(items->size > 0);
    return getSLL(items->list, 0);
}

extern int
sizeQUEUE(QUEUE *items) { //returns the number of nodes in the queue
    return items->size;
}

extern void
displayQUEUE(QUEUE *items,FILE *fp) { //displays the queue as an array
    printf("<");
    for (int i=0; i<items->size; i++) {
        void *current = getSLL(items->list, i);
        items->display(current, fp);
        if (i != items->size-1) {printf(",");}
    }
    printf(">");
    return;
}

extern void
displayQUEUEdebug(QUEUE *items,FILE *fp) { //displays the queue as an array, while also highlighting where the head and tail nodes are
    displaySLLdebug(items->list, fp);
    return;
}

extern void
freeQUEUE(QUEUE *items) { //frees the queue by freeing the SLL, and then the queue itself
    freeSLL(items->list);
    free(items);
    return;
}
