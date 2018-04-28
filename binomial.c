#include "binomial.h"
#include "dll.h"
#include "queue.h"
#include "stack.h"
// #include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------------------- BNODE Structure and Functions --------------------*/

typedef struct bnode BNODE;
void displayBNODE(void *,FILE *);
void displayDLLbackwards(BINOMIAL *b, DLL *items, FILE *fp);

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
    new->children = newDLL(displayBNODE, free);
    // new->degree = 0;
    new->display = display;
    new->compare = compare;
    new->free = free;
    return new;
}

void displayBNODE(void *v, FILE *fp) {
    BNODE *node = v;
    node->display(node->key, fp);
    return;
}

void *getBNODEkey(BNODE *node) {
    if (node->key) {
        return node->key;
    }
    else {return NULL;}
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
    tree->rootList = newDLL(displayBNODE, free);
    tree->extreme = 0;
    tree->size = 0;
    tree->display = display;
    tree->compare = compare;
    tree->update = update;
    tree->free = free;
    return tree;
}

extern void *
insertBINOMIAL(BINOMIAL *b, void *value) {
    printf("IN INSERT BINOMIAL!\n");
    BNODE *new = newBNODE(b->display, b->compare, b->free, value);
    new->parent = new;
    // insertDLL(b->rootList, 0, new);
    printf("inserted into rootlist\n");
    new->owner = insertDLL(b->rootList, 0, new);
    b->size ++;
    printf("about to consolidate\n");
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
decreaseKeyBINOMIAL(BINOMIAL *b, void *node, void *value) {
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
statisticsBINOMIAL(BINOMIAL *b, FILE *fp) {
    fprintf(fp, "size: %d\nrootlist size: %d\n", b->size, sizeDLL(b->rootList));
    if (b->extreme != NULL) {
        fprintf(fp, "extreme: ");
        b->display(getBNODEkey(b->extreme), fp);
    }
}

extern void 
displayBINOMIAL(BINOMIAL *b, FILE *fp) {
    printf("\n\n\nIN DISPLAYYYYYY\nsize of rootList is: %d\n", sizeDLL(b->rootList));
    double x = log((double)b->size);
    double y = log(2.0);
    double result = x/y;
    int size = (int)result + 1;
    int count = 0;

    fprintf(fp, "rootlist: ");
    // firstDLL(b->rootList);
    lastDLL(b->rootList);
    while (count < size) {
        BNODE *temp = currentDLL(b->rootList);
        if (count < sizeDLL(temp->children)) {
            fprintf(fp, "NULL");
            if (count != size-1) {fprintf(fp, " ");}
            count ++;
        }
        else {
            b->display(temp->key, fp);
            if (b->extreme  == temp) {fprintf(fp, "*");}
            if (count != size-1) {fprintf(fp, " ");}
            // nextDLL(b->rootList);
            prevDLL(b->rootList);
            count ++;
        }
    }
    fprintf(fp, "\n");
}

extern void 
displayBINOMIALdebug(BINOMIAL *b, FILE *fp) {
    DLL *list = NULL;
    int x = 0;
    // QUEUE *newq = newQUEUE(b->display, b->free);
    STACK *news = newSTACK(b->display, b->free);
    // QUEUE *oldq = newQUEUE(b->display, b->free);
    STACK *stack = newSTACK(b->display, b->free);

    displayDLLbackwards(b, b->rootList, stdout);
    printf("\n");
    x ++;

    // enqueue(oldq, b->rootList);
    push(stack, b->rootList);
    
    while (x < b->size) {
        // int oldsize = sizeQUEUE(oldq);
        int oldsize = sizeSTACK(stack);
        // printf("OLDSIZE : %d\n", oldsize);
        for (int i=0; i<oldsize; i++) {
            // list = dequeue(oldq);
            list = pop(stack);
            firstDLL(list);
            while (moreDLL(list) == 1) {
                BNODE *curr = currentDLL(list);
                // printf("CURR: ");
                // b->display(curr->key, stdout);
                // printf("\n");
                if (sizeDLL(curr->children) > 0) {
                    // printf("this node has children\n");
                    // enqueue(newq, curr->children);
                    push(news, curr->children);
                }
                nextDLL(list);
                x ++;
            }
        }
        // printf("here\n");
        // int newsize = sizeQUEUE(newq);
        int newsize = sizeSTACK(news);
        // printf("NEWSIZE : %d\n", newsize);
        for (int i=0; i<newsize; i++) {
            // list = dequeue(newq);
            list = pop(news);
            displayDLLbackwards(b, list, fp);
            // enqueue(oldq, list);
            push(stack, list);
            // x ++;
        }
        printf("\n");
    }
    // while (sizeQUEUE(oldq) > 0) {
    //     dequeue(oldq);
    // }
    printf("\n");
    return;
}

/*------------------- Auxiliary Functions --------------------*/

void consolidate(BINOMIAL *b) {
    printf("\nIN CONSOLIDATE\n");
    int size;
    DLL *list = b->rootList;
    printf("Rootlist size is: %d\n", sizeDLL(list));
    printf("Rootlist: ");
    firstDLL(list);
    // for (int i=0; i<sizeDLL(list); i++) {
    while (moreDLL(list) == 1) {
        BNODE *curr = currentDLL(list);
        b->display(curr->key, stdout);
        printf(" ");
        nextDLL(list);
    }
    printf("\n");
    
    double x = log((double)b->size);
    printf("DOUBLE X: %f\n", x);
    double y = log(2.0);
    printf("DOUBLE Y: %f\n", y);
    double result = x/y;
    printf("result: %f\n", result);
    size = (int)result + 1;
    
    printf("BINOMIAL size is: %d\nD array size is: %d\nRootlist size is: %d\n", b->size, size, sizeDLL(b->rootList));
    void* array[size];
    printf("Created the array\n");
    for (int i=0; i<size; i++) {
        array[i] = NULL;
    }

    int i = 0;
    firstDLL(list);
    while (moreDLL(list) == 1) {
        printf("in the while, moreDLL is %d\n", moreDLL(list));
        BNODE *spot = currentDLL(list);
        printf("SPOT using current: ");
        b->display(spot->key, stdout);
        printf("\n");
        nextDLL(list);
        spot = removeDLLnode(list, getBNODEowner(spot));
        // printf("SPOT after remove: ");
        // b->display(spot->key, stdout);
        // printf("\n");
        updateConsolidationArray(b, array, spot);
        i++;
        // nextDLL(list);
        printf("-------MOREDLL: %d\n", moreDLL(list));
    }

    b->extreme = NULL;
    printf("Set extreme to null, about to go into for loop\n");
    for (int i=0; i<size; i++) {
        if (array[i] != NULL) {
            printf("array[%d] is NOT NULL\nDLL size is %d\n", i, sizeDLL(list));
            insertDLL(list, 0, array[i]);
            printf("inserted DLL\n");
            if (b->extreme == NULL) {
                b->extreme = array[i];
            }
            else if (b->extreme != NULL) {
                if (b->compare(getBNODEkey(b->extreme), getBNODEkey(array[i])) > 0) {
                    b->extreme = array[i];
                }
            }
        }
    }
    printf("made it out of the for loop\n");
    // free(array);
}

void updateConsolidationArray(BINOMIAL *b, void **array, BNODE *node) {
    printf("\nUPDATING CONSOLIDATION ARRAY\n");
    int degree = sizeDLL(node->children);
    printf("Degree: %d\n", degree);

    if (array[degree]) {
        printf("WHATS UP TESTING THIS NULL BULLSHIT\n");
        printf("array[%d] = ", degree);
        b->display(node->key, stdout);
        printf("\n");
    }

    while (array[degree]) {
        printf("array[%d] is NOT NULL: ", degree);
       
        node = combine(b, node, array[degree]);
        array[degree] = NULL;
        degree ++;
    }
    
    array[degree] = node;
    printf("array[%d] = ", degree);
    b->display(node->key, stdout);
    printf("\nDONE UPDATING CONSOLIDATION ARRAY\n");
    return;
}

BNODE *combine(BINOMIAL *b, BNODE *n1, BNODE *n2) {
    printf("\nIN COMBINE\n");
    if (b->compare(n1, n2) > 0) {
        printf("N1: ");
        b->display(n1->key, stdout);
        printf(" going into N2: ");
        b->display(n2->key, stdout);
        printf("\n");
        insertDLL(n2->children, 0, n1);
        n1->parent = n2;
        return n2;
    }
    else {
        printf("N2: ");
        b->display(n2->key, stdout);
        printf(" going into N1: ");
        b->display(n1->key, stdout);
        printf("\n");
        insertDLL(n1->children, 0, n2);
        n2->parent = n1;
        return n1;
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

void displayDLLbackwards(BINOMIAL *b, DLL *items, FILE *fp) {
    printf("{{");
    lastDLL(items);
    BNODE *current = currentDLL(items);
    int x = 0;
    while (x < sizeDLL(items)) {
    // while (moreDLL(items) == 1) {
        
        b->display(current->key, fp);
        // printf("\n");

        prevDLL(items);
        if (currentDLL(items) != NULL) {printf(",");}
        // if (current == items->head) {break;}
        if (moreDLL(items) == 0) {
            // printf("breaking\n");
            break;
        }
        
        current = currentDLL(items);
        
        x ++;
        // printf("X: %d\n", x);
    }
    // printf("outside of thie loop\n");
    printf("}}");
    // printf("\n");
    return;
}