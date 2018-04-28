#include "binomial.h"
#include "dll.h"
// #include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------------------- BNODE Structure and Functions --------------------*/

typedef struct bnode BNODE;

struct bnode
{
    void *key;
    BNODE *parent;
    DLL *children;
    struct node *owner;
    // BNODE *rightSibling;
    // int degree;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*free)(void *);                    //free
};

BNODE *newBNODE(
void (*display)(void *,FILE *),
int (*compare)(void *,void *),
void (*free)(void *),
void *value) {
    BNODE *new = malloc(sizeof(BNODE));
    new->key = value;
    new->parent = NULL;
    new->children = newDLL(display, free);
    // new->degree = 0;
    new->display = display;
    new->compare = compare;
    new->free = free;
    return new;
}

void *getBNODEkey(BNODE *node) {
    return node->key;
}

void *getBNODEowner(BNODE *node) {
    return node->owner;
}

void setBNODEparent(BNODE *node, void *val) {
    node->parent = val;
}

/*------------------- Auxiliary/Structure Function Definitions --------------------*/

void update(void *,void *);
BNODE *newBNODE(void (*display)(void *,FILE *),int (*compare)(void *,void *), void (*free)(void *),void *v);
void *getBNODEkey(BNODE *);
void *getBNODEowner(BNODE *);
void setBNODEparent(BNODE *,void *);
void consolidate(BINOMIAL *);
void updateConsolidationArray(BINOMIAL *,void *[],BNODE *);
BNODE *combine(BINOMIAL *, BNODE *, BNODE *);
void *bubbleUp(BINOMIAL *, void *);
void swapValues(BNODE *, BNODE *);

/*------------------- Binomial Struct --------------------*/

struct binomial
{
    DLL *rootList;
    BNODE *extreme;
    int size;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*free)(void *);                    //free
    void (*update)(void *,void *);           //update
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
    new->owner = insertDLL(b->rootList, 0, new);
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
    decreaseKeyBINOMIAL(b, node, NULL);
    extractBINOMIAL(b);
}

//TODO
extern void 
decreaseKeyBINOMIAL(BINOMIAL *b,void *node,void *value) {
    BNODE *n = node;
    n->key = value;
    n = bubbleUp(b, n);
    if (b->compare(n->key, getBNODEkey(b->extreme)) < 0) {
        b->extreme = n;
    }
}

extern void *
peekBINOMIAL(BINOMIAL *b) {
    if (b->extreme) {
        return getBNODEkey(b->extreme);
    }
    else {return NULL;}
}

extern void *
extractBINOMIAL(BINOMIAL *b) {
    BNODE *y = b->extreme;
    y = removeDLLnode(b->rootList, getBNODEowner(y));
    DLL *list = y->children;
    firstDLL(list);
    for (int i=0; i<sizeDLL(list); i++) {
        setBNODEparent(currentDLL(list), currentDLL(list));
        nextDLL(list);
    }
    unionDLL(b->rootList, list);
    consolidate(b);
    b->size --;
    return y->key;
}

extern void 
statisticsBINOMIAL(BINOMIAL *b,FILE *fp) {
    
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
        spot = removeDLLnode(list, getBNODEowner(spot));
        updateConsolidationArray(b, array, spot);
        i++;
        firstDLL(list);
    }

    b->extreme = NULL;
    for (int i=0; i<b->size; i++) {
        if (array[i] != NULL) {
            insertDLL(list, 0, array[i]);
            if (b->extreme != NULL) {
                if (b->compare(getBNODEkey(b->extreme), getBNODEkey(array[i])) > 0) {
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
        array[degree] = NULL;
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
    else if (b->compare(n->key, p->key) >= 0) {
        return n;
    }
    else {
        if (b->update) {
            update(n, p);
        }
        swapValues(n, p);
        return bubbleUp(b, p);
    }
}

void update(void *node, void *parent) {
    BNODE *n = node;
    BNODE *p = parent;
    struct node *pOwner = p->owner;

    p->owner = n->owner;
    n->owner = pOwner;
}

void swapValues(BNODE *n1, BNODE *n2) {
    void *temp = n1->key;
    n1->key = n2->key;
    n2->key = temp;
}