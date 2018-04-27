#include "binomial.h"
#include "dll.h"
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------------------- Auxiliary/Structure Function Definitions --------------------*/

BNODE *newBNODE(void (*display)(void *,FILE *),int (*compare)(void *,void *), void (*free)(void *),void *v);
void consolidate(BINOMIAL *);
void updateConsolidationArray(BINOMIAL *,void *[],BNODE *);
BNODE *combine(BINOMIAL *, BNODE *, BNODE *);
void *bubbleUp(BINOMIAL *, void *);

/*------------------- Structures --------------------*/

typedef struct bnode
{
    void *key;
    BNODE *parent;
    DLL *children;
    // BNODE *rightSibling;
    int degree;
    // void (*display)(void *,FILE *);          //display
    // int (*compare)(void *,void *);           //comparator
    // void (*free)(void *);                    //free
} BNODE;

BNODE *newBNODE(
void (*display)(void *,FILE *),
int (*compare)(void *,void *),
void (*free)(void *),
void *value) {
    BNODE *new = malloc(sizeof(BNODE));
    new->key = value;
    new->parent = NULL;
    new->children = newDLL(display, free);
    new->degree = 0;
    return new;
}

struct binomial
{
    DLL *rootList;
    BNODE *extreme;
    int size;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*free)(void *);                    //free
    void (*update)(void *,void *)            //update
};

/*------------------- Main Functions --------------------*/

extern BINOMIAL *
newBINOMIAL(
void (*display)(void *,FILE *),
int (*compare)(void *,void *),
void (*update)(void *,void *),
void (*free)(void *)) {
    BINOMIAL *tree = malloc(sizeof(BINOMIAL));
    tree->rootList = newDLL(display, free);
    tree->extreme = 0;
    tree->size = 0;
    tree->display = display;
    tree->compare = compare;
    tree->update = update;
    tree->free = free;
    return tree;
}

extern void *
insertBINOMIAL(BINOMIAL *b,void *value) {
    BNODE *new = newBNODE(b->display, b->compare, b->free, value);
    new->parent = new;
    insertDLL(b->rootList, 0, new);
    b->size ++;
    consolidate(b);
    return new;
}

extern int 
sizeBINOMIAL(BINOMIAL *b) {
    return b->size;
}

extern void 
unionBINOMIAL(BINOMIAL *recipient, BINOMIAL *donor) {
    unionDLL(recipient->rootList, donor->rootList);
    recipient->size += donor->size;
    donor->rootList = newDLL(donor->display, donor->free);
    donor->size = 0;
    donor->extreme = 0;
    consolidate(recipient);
}

//TODO
extern void 
deleteBINOMIAL(BINOMIAL *b, void *node) {

}

//TODO
extern void 
decreaseKeyBINOMIAL(BINOMIAL *b,void *node,void *value) {

}

/*------------------- Auxiliary Functions --------------------*/

void consolidate(BINOMIAL *b) {
    DLL *list = b->rootList;
    double x = log((double)b->size);
    double y = log(2.0);
    double result = x/y;
    int size = (int)result + 1;
    void *array[size];

    int i = 0;
    firstDLL(list);
    while (moreDLL(list) == 1) {
        BNODE *spot = currentDLL(list);
        spot = removeDLLnode(list, spot);
        updateConsolidationArray(b, array, spot);
        i++;
        firstDLL(list);
    }

    b->extreme = NULL;
    for (int i=0; i<b->size; i++) {
        if (array[i] != NULL) {
            insertDLL(list, 0, array[i]);
            if (b->extreme != NULL) {
                if (b->compare(b->extreme, array[i]) > 0) {
                    b->extreme = array[i];
                }
            }
        }
    }
    free(array);
}

void updateConsolidationArray(BINOMIAL *b, void *array[], BNODE *node) {
    int degree = sizeDLL(node->children);
    while (array[degree] != NULL) {
        node = combine(b, node, array[degree]);
        array[degree] == NULL;
        degree ++;
    }
    array[degree] = node;
}

BNODE *combine(BINOMIAL *b, BNODE *n1, BNODE *n2) {
    if (b->compare(n1, n2) > 0) {
        insertDLL(n2->children, 0, n1);
        n1->parent = n2;
        return n1;
    }
    else {
        insertDLL(n1->children, 0, n2);
        n2->parent = n1;
        return n2;
    }
}

//TODO
void *bubbleUp(BINOMIAL *b, void *node) {
    BNODE *n = node;
    BNODE *p = n->parent;
    if (n == p) {
        return n;
    }
    if (b->compare(n->key, p->key) >= 0) {
        return n;
    }
    if (b->update) {

    }
}