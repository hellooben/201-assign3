/********************
CS201, Assign0, sll.c
Ben Bailey
This class implements a singly linked list that can be used with any generic data type.
********************/

#include "sll.h"
#include <stdlib.h>
#include <assert.h>

struct node //the node structure that makes up the linked list
{
    void *data;
    struct node *next;
}node;

struct sll //the singly linked list structure
{
    void *head;
    void *tail;
    int size;
    void (*display)(void *, FILE *);
    void (*free)(void *);
};

extern SLL *
newSLL(void (*d)(void *,FILE *),void (*f)(void *)) { //constructor for the singly linked list
    SLL *items = malloc(sizeof(SLL));
    assert(items != 0);
    items->head = 0;
    items->tail = 0;
    items->size = 0;
    items->display = d;
    items->free = f;
    return items;
}

extern void
insertSLL(SLL *items,int index,void *value) { //inserts a node anywhere into the singly linked list, based on a given index
    assert(index >= 0 && index <= items->size);
    struct node *new = malloc(sizeof(node));
    if (items->size == 0) {
        new->data = value;
        new->next = NULL;
        items->head = new;
        items->tail = new;
        items->size ++;
        return;
    }
    else if (index == 0) {
        struct node *current = items->head;
        new->data = value;
        new->next = current;
        items->head = new;
        items->size ++;
        return;
    }
    else if (index == items->size) {
        struct node *current = items->tail;
        current->next = new;
        new->data = value;
        new->next = NULL;
        items->tail = new;
        items->size ++;
        return;
    }
    else {
        struct node *current = items->head;
        for (int i=0; i<index-1; i++) {
            current = current->next;
        }
        struct node *currnext = current->next;
        current->next = new;
        new->data = value;
        new->next = currnext;
        items->size ++;
        return;
    }
}

extern void *
removeSLL(SLL *items,int index) { //removes a node from the singly linked list, based on a given index
    assert(items->size > 0);
    assert(index >= 0 && index < items->size);
    if (index == 0) {
        struct node *current = items->head;
        items->head = current->next;
        items->size --;
        void *returnable = current->data;
        free(current);
        return returnable;
    }
    else if (index == items->size-1) {
        struct node *current = items->head;
        for (int i=0; i<index-1; i++) {
            current = current->next;
        }
        struct node *removable = current->next;
        current->next = NULL;
        items->tail = current;
        items->size --;
        void *returnable = removable->data;
        free(removable);
        return returnable;
    }
    else {
        struct node *current = items->head;
        for (int i=0; i<index-1; i++) {
            current = current->next;
        }
        struct node *removable = current->next;
        current->next = removable->next;
        items->size --;
        void *returnable = removable->data;
        free(removable);
        return returnable;
    }
}

extern void
unionSLL(SLL *recipient,SLL *donor) { //merges two singly linked lists. the first parameter is the list to be added to, and the second is the donor list
    if (donor->head == NULL && donor->size == 0) {
        return;
    }
    else if (recipient->head == NULL && recipient->size == 0) {
        recipient->head = donor->head;
        recipient->tail = donor->tail;
        recipient->size += donor->size;

        donor->head = 0;
        donor->tail = 0;
        donor->size = 0;
        return;
    }
    else {
        struct node *recTail = recipient->tail;
        recTail->next = donor->head;
        recipient->tail = donor->tail;
        recipient->size += donor->size;

        donor->head = 0;
        donor->tail = 0;
        donor->size = 0;
        return;
    }
}

extern void *
getSLL(SLL *items,int index) { //returns the data of a node in the list, based on a given index
    assert(index >= 0 && index < items->size);
    struct node *returnable = items->head;
    if (index == 0) {
        return returnable->data;
    }
    else if (index == items->size-1) {
        returnable = items->tail;
        return returnable->data;
    }
    else {
        for (int i=0; i<index; i++) {
            returnable = returnable->next;
        }
        return returnable->data;
    }
}

extern void *
setSLL(SLL *items,int index,void *value) { //sets the data of a node in the list, based on a given index and value to be put in
    assert(index >= 0 && index <= items->size);
    if (items->size == 0) {
        insertSLL(items, index, value);
        struct node *returnable = NULL;
        return returnable;
    }
    if (index == 0) {
        struct node *current = items->head;
        void *returnable = current->data;
        current->data = value;
        return returnable;
    }
    else if (index == items->size-1) {
        struct node *current = items->tail;
        void *returnable = current->data;
        current->data = value;
        return returnable;
    }
    else if (index == items->size) {
        insertSLL(items, index, value);
        struct node *returnable = NULL;
        return returnable;
    }
    else {
        struct node *current = items->head;
        for (int i=0; i<index; i++) {
            current = current->next;
        }
        void *returnable = current->data;
        current->data = value;
        return returnable;
    }
}

extern int
sizeSLL(SLL *items) { //returns the number of nodes in the list
    return items->size;
}

extern void
displaySLL(SLL *items,FILE *fp) { //displays the list as an array
    printf("{");
    struct node *current = items->head;
    while (current != NULL) {
        items->display(current->data, fp);
        if (current->next != NULL) {printf(",");}
        current = current->next;
    }
    printf("}");
    return;
}

extern void
displaySLLdebug(SLL *items,FILE *fp) { //displays the list as an array, while also highlighting where the head and tail nodes are
    if (items->size == 0) {
        printf("head->{},tail->{}");
        return;
    }
    else {
        printf("head->{");
        struct node *current = items->head;
        while (current != NULL) {
            items->display(current->data, fp);
            if (current->next != NULL) {printf(",");}
            current = current->next;
        }
        struct node *daTail = items->tail;
        printf("},tail->{");
        items->display(daTail->data, fp);
        printf("}");
        return;
    }
}

extern void
freeSLL(SLL *items) { //frees each node's data, the node itself, and the linked list data structure as a whole
    if (items->size ==0) {
        free(items);
        return;
    }
    else {
        struct node *current = items->head;
        while (current != NULL) {
            struct node *freeable = current;
            if (items->free != NULL) {
                items->free(current->data);
            }
            current = current->next;
            free(freeable);
        }
        free(items);
        return;
    }
    return;
}
