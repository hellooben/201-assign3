/********************
CS201, Assign0, dll.c
Ben Bailey
This class implements a doubly linked list that can be used with any generic data type.
********************/

#include "dll.h"
#include <stdlib.h>
#include <assert.h>

struct node //the node structure that makes up the doubly linked list
{
    void *data;
    struct node *next;
    struct node *prev;
}node;

struct dll //the doubly linked list structure
{
    void *head;
    void *tail;
    int size;
    void (*display)(void *, FILE *);
    void (*free)(void *);
    struct node *iterator;
};

extern DLL *
newDLL(void (*d)(void *,FILE *),void (*f)(void *)) { //constructor for the doubly linked list
    DLL *items = malloc(sizeof(DLL));
    assert(items != 0);
    items->head = 0;
    items->tail = 0;
    items->size = 0;
    items->display = d;
    items->free = f;
    items->iterator = NULL;
    return items;
}

extern void *
insertDLL(DLL *items,int index,void *value) { //inserts a node anywhere into the doubly linked list, based on a given index
    assert(index >= 0 && index <= items->size);
    struct node *new = malloc(sizeof(node));
    if (items->size == 0) {
        new->data = value;
        new->next = NULL;
        new->prev = NULL;
        items->head = new;
        items->tail = new;
        items->size ++;
        return new;
    }
    else if (index == 0) {
        struct node *current = items->head;
        new->data = value;
        new->next = current;
        new->prev = 0;
        current->prev = new;
        items->head = new;
        items->size ++;
        return new;
    }
    else if (index == items->size) {
        struct node *current = items->tail;
        current->next = new;
        new->data = value;
        new->prev = current;
        new->next = NULL;
        items->tail = new;
        items->size ++;
        return new;
    }
    else if (index >= items->size/2 && items->size > 20) {
        struct node *current = items->tail;
        for (int i=0; i<items->size-index-1; i++) {
            current = current->prev;
        }
        struct node *currprev = current->prev;
        current->prev = new;
        new->data = value;
        new->prev = currprev;
        new->next = current;
        currprev->next = new;
        items->size ++;
        return new;
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
        new->prev = current;
        currnext->prev = new;
        items->size ++;
        return new;
    }
}

extern void *
removeDLL(DLL *items,int index) { //removes a node from the doubly linked list, based on a given index
    assert(items->size > 0);
    assert(index >= 0 && index < items->size);
    if (index == 0) {
        if (items->size == 1) {
            struct node *removable = items->head;
            void *returnable = removable->data;
            free(removable);
            items->head = 0;
            items->tail = 0;
            items->size --;
            return returnable;
        }
        else {
            struct node *removable = items->head;
            struct node *current = removable->next;
            items->head = current;
            current->prev = 0;
            items->size --;
            void *returnable = removable->data;
            free(removable);
            return returnable;
        }
    }
    else if (index == items->size-1) {
        struct node *removable = items->tail;
        struct node *current = removable->prev;
        items->tail = current;
        current->next = 0;
        items->size --;
        void *returnable = removable->data;
        free(removable);
        return returnable;
    }
    else if (index >= items->size/2 && items->size > 20) {
        struct node *current = items->tail;
        for (int i=0; i<items->size-index-2; i++) {
            current = current->prev;
        }
        struct node *removable = current->prev;
        struct node *currprev = removable->prev;
        current->prev = currprev;
        currprev->next = current;
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
        struct node *currnext = removable->next;
        current->next = currnext;
        currnext->prev = current;
        items->size --;
        void *returnable = removable->data;
        free(removable);
        return returnable;
    }
}

extern void
unionDLL(DLL *recipient,DLL *donor) { //merges two doubly linked lists. the first parameter is the list to be added to, and the second is the donor list
    struct node *donorHead = donor->head;
    struct node *recTail = recipient->tail;

    if (donorHead == NULL && donor->size == 0) {
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
        recTail->next = donorHead;
        recipient->tail = donor->tail;
        recipient->size += donor->size;

        donor->head = 0;
        donor->tail = 0;
        donor->size = 0;
        return;
    }
    return;
}

extern void *
getDLL(DLL *items,int index) { //returns the data of a node in the list, based on a given index
    assert(index >= 0 && index < items->size);
    struct node *returnable = items->head;
    if (index == 0) {
        return returnable->data;
    }
    else if (index == items->size-1) {
        returnable = items->tail;
        return returnable->data;
    }
    else if (index >= items->size/2 && items->size > 20) {
        returnable = items->tail;
        for (int i=0; i<items->size-index-1; i++) {
            returnable = returnable->prev;
        }
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
setDLL(DLL *items,int index,void *value) { //sets the data of a node in the list, based on a given index and value to be put in
    assert(index >= 0 && index <= items->size);
    struct node *current = items->head;
    if (items->size == 0) {
        insertDLL(items, index, value);
        struct node *returnable = NULL;
        return returnable;
    }
    if (index == 0) {
        void *returnable = current->data;
        current->data = value;
        return returnable;
    }
    else if (index == items->size-1) {
        current = items->tail;
        void *returnable = current->data;
        current->data = value;
        return returnable;
    }
    else if (index == items->size) {
        insertDLL(items, index, value);
        struct node *returnable = NULL;
        return returnable;
    }
    else if (index >= items->size/2 && items->size > 20) {
        current = items->tail;
        for (int i=0; i<items->size-index-1; i++) {
            current = current->prev;
        }
        void *returnable = current->data;
        current->data = value;
        return returnable;
    }
    else {
        for (int i=0; i<index; i++) {
            current = current->next;
        }
        void *returnable = current->data;
        current->data = value;
        return returnable;
    }
}

extern int
sizeDLL(DLL *items) { //returns the number of nodes in the list
    return items->size;
}

extern void
displayDLL(DLL *items,FILE *fp) { //displays the doubly linked list as an array
    printf("{{");
    struct node *current = items->head;
    int x = 0;
    while (x < sizeDLL(items)) {
        items->display(current->data, fp);
        if (current == items->tail) {break;}
        if (current->next != NULL) {printf(",");}
        current = current->next;
        x ++;
    }
    printf("}}");
    return;
}

extern void
displayDLLdebug(DLL *items,FILE *fp) { //displays the doubly linked list as an array, while also highliting where the head and tail nodes are
    if (items->size == 0) {
        printf("head->{{}},tail->{{}}");
        return;
    }
    else {
        printf("head->{{");
        struct node *current = items->head;
        while (current != NULL) {
            items->display(current->data, fp);
            if (current == items->tail) {break;}
            if (current->next != NULL) {printf(",");}
            current = current->next;
        }
        struct node *daTail = items->tail;
        printf("}},tail->{{");
        items->display(daTail->data, fp);
        printf("}}");
        return;
    }
}

extern void
freeDLL(DLL *items) { //frees each node's data, the node itself, and the linked list data structure as whole
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

/*------------------- New Functions --------------------*/

extern void
removeDLLall(DLL *items) {
    struct node *temp = items->head;
    while (temp) {
        struct node *freeable = temp;
        temp = temp->next;
        free(freeable);
    }
    items->head = 0;
    items->tail = 0;
    items->size = 0;
    return;
}

extern void *
removeDLLnode(DLL *items, void *value) {
    // printf("\nIN REMOVE DLLNODE\nNODE IS: ");
    struct node *node = value;
    // items->display(node->data, stdout);
    // printf("\n");

    if (items->size == 1) {
        printf("Size is 1\n");
        items->head = 0;
        items->tail = 0;
        node->next = 0;
        node->prev = 0;
        items->size --;
        return node->data;
    }
    else {
        struct node *p = node->prev;
        struct node *n = node->next;
        // printf("made the other nodes\n");

        if (node == items->head) {
            // printf("node->prev == NULL\n");
            items->head = n;
        }
        if (node == items->tail) {
            // printf("node->next == NULL\n");
            items->tail = p;
        }
        node->prev = 0;
        node->next = 0;
        if (p) {
            // printf("previous is NOT NULL\n");
            p->next = n;
        }   
        if (n) {
            // printf("next is NOT NULL\n");
            n->prev = p;
        }
        items->size --;

        // printf("returning the data with DLL size %d\n", items->size);
        return node->data;
    }
}

extern void
firstDLL(DLL *items) {
    items->iterator = items->head;
    // items->display()
    return;
}

extern void
lastDLL(DLL *items) {
    items->iterator = items->tail;
    return;
}

extern int
moreDLL(DLL *items) {
    if (items->iterator) {return 1;}
    else {return 0;}
}

extern void
nextDLL(DLL *items) {
    struct node *it = items->iterator;
    if (it->next == NULL) {
        items->iterator = NULL;
    }
    else {
        it = it->next;
        items->iterator = it;
    }
    return;
}

extern void
prevDLL(DLL *items) {
    struct node *it = items->iterator;
    if (it->prev == NULL) {
        // printf("setting null\n");
        items->iterator = NULL;
    }
    else {
        // printf("setting a prev\n");
        it = it->prev;
        items->iterator = it;
    }
    // printf("returning from prevDLL\n");
    return;
}

extern void *
currentDLL(DLL *items) {
    struct node *it = items->iterator;
    if (it) {
        return it->data;
    }
    else {return NULL;}
}
