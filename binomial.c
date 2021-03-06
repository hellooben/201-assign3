#include "binomial.h"
#include "dll.h"
#include "queue.h"
// #include "stack.h"
// #include "vertex.h"
// #include "edge.h"
// #include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------------------- BNODE Structure and Functions --------------------*/

typedef struct bnode BNODE;
void displayBNODE(void *,FILE *);
void displayDLLordered(BINOMIAL *b, DLL *items, FILE *fp);
void freeBNODE(void *);

struct bnode
{
    void *key;
    BNODE *parent;
    DLL *children;
    // struct node *owner;
    void *owner;
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
    new->children = newDLL(displayBNODE, freeBNODE);
    new->owner = NULL;
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

void setBNODEparent(BNODE *node, BNODE *val) {
    node->parent = val;
}

void freeBNODE(void *n) {
    BNODE *node = n;
    // printf("FREEING: ");
    // node->display(node->key, stdout);
    // printf("\n");
    node->free(node->key);
    free(n);
    return;
}

/*------------------- Auxiliary/Structure Function Definitions --------------------*/

void update(void *,void *);
BNODE *newBNODE(void (*display)(void *,FILE *),int (*compare)(void *,void *), void (*free)(void *),void *v);
// void displayBINOMIALlevel(FILE *fp, int count, QUEUE *list);
void *getBNODEkey(BNODE *);
void *getBNODEowner(BNODE *);
void setBNODEparent(BNODE *,BNODE *);
void consolidate(BINOMIAL *);
void updateConsolidationArray(BINOMIAL *,BNODE **,BNODE *);
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
    tree->rootList = newDLL(displayBNODE, freeBNODE);
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
    // printf("IN INSERT BINOMIAL!\n");
    BNODE *new = newBNODE(b->display, b->compare, b->free, value);
    new->parent = new;
    // insertDLL(b->rootList, 0, new);
    // printf("inserted into rootlist\n");
    void *temp = insertDLL(b->rootList, 0, new);
    // void *temp = insertDLL(b->rootList, sizeDLL(b->rootList), new);
    new->owner = temp;
    b->size ++;
    // printf("about to consolidate\n");
    // printf("Heap before Consolidate:\n");
    // displayBINOMIALdebug(b, stdout);
    // printf("\n");
    consolidate(b);
    // printf("Heap after consolidate: \n");
    // displayBINOMIALdebug(b, stdout);
    // printf("\n------------INSERTED------------\n\n");
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
    donor->rootList = newDLL(displayBNODE, freeBNODE);
    donor->size = 0;
    donor->extreme = 0;
    consolidate(recipient);
}

extern void
deleteBINOMIAL(BINOMIAL *b, void *node) {
    decreaseKeyBINOMIAL(b, node, NULL);
    extractBINOMIAL(b);
}

extern void
decreaseKeyBINOMIAL(BINOMIAL *b, void *node, void *value) {
    BNODE *n = node;
    n->key = value;
    n = bubbleUp(b, n);
    // printf("comparing ");
    // b->display(n->key, stdout);
    // printf(" and ");
    // b->display(getBNODEkey(b->extreme), stdout);
    // printf("\n");
    if (b->compare(n->key, getBNODEkey(b->extreme)) < 0) {
        // printf("setting DA extreme to ");
        // b->display(n->key, stdout);
        // printf("\n");
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
    // printf("\nIN EXTRACT\n");
    // printf("Rootlist size: %d\nROOTLIST LOOKS LIKE: \n", sizeDLL(b->rootList));
    // displayDLLordered(b, b->rootList, stdout);
    // printf("\n");

    BNODE *y = b->extreme;
    // printf("EXTREME: ");
    // b->display(y->key, stdout);
    // printf("\n");
    // struct node *owner = getBNODEowner(y);

    y = removeDLLnode(b->rootList, getBNODEowner(y));
    y->parent = NULL;

    // printf("ROOTLIST after remove: \n");
    // displayDLLordered(b, b->rootList, stdout);
    // printf("\n");

    DLL *list = y->children;
    // printf("CHILD LIST: \n");
    // displayDLLordered(b, list, stdout);
    // printf("\n");
    firstDLL(list);
    for (int i=0; i<sizeDLL(list); i++) {
        setBNODEparent(currentDLL(list), currentDLL(list));
        nextDLL(list);
    }
    // printf("ROOTLIST right before Union: \n");
    // displayDLLordered(b, b->rootList, stdout);
    // printf("\n");
    // printf("CHILD LIST right before union: \n");
    // displayDLLordered(b, list, stdout);
    // printf("\n");

    unionDLL(list, b->rootList);
    y->children = b->rootList;
    b->rootList = list;

    freeDLL(y->children);

    // unionDLL(b->rootList, list);

    // printf("rootList after union: \n");
    // displayDLLordered(b, b->rootList, stdout);
    consolidate(b);
    b->size --;
    y->parent = 0;
    y->children = 0;
    y->owner = 0;
    void *returnable = y->key;
    // freeDLL(y->children);
    free(y);
    return returnable;
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
    // printf("\n\n\nIN DISPLAYYYYYY\nsize of rootList is: %d\n", sizeDLL(b->rootList));
    double x = log((double)b->size);
    double y = log(2.0);
    double result = x/y;
    int size = (int)result + 1;
    int count = 0;

    fprintf(fp, "rootlist: ");
    firstDLL(b->rootList);
    // lastDLL(b->rootList);
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
            nextDLL(b->rootList);
            // prevDLL(b->rootList);
            count ++;
        }
    }
    fprintf(fp, "\n");
}

extern void
displayBINOMIALdebug(BINOMIAL *b, FILE *fp) {
    // QUEUE *list = newQUEUE(b->display, b->free);
    // enqueue(list, b->rootList);
    // displayBINOMIALlevel(fp, 1, list);
    // freeQUEUE(list);
    DLL *list = NULL;
    int x = 0;
    QUEUE *newq = newQUEUE(b->display, b->free);
    // STACK *news = newSTACK(b->display, b->free);
    QUEUE *oldq = newQUEUE(b->display, b->free);
    // STACK *stack = newSTACK(b->display, b->free);

    // displayDLLordered(b, b->rootList, stdout);
    displayDLL(b->rootList, stdout);
    printf("\n");
    x ++;

    enqueue(oldq, b->rootList);
    // push(stack, b->rootList);

    while (x < b->size) {
        int oldsize = sizeQUEUE(oldq);
        // int oldsize = sizeSTACK(stack);
        // printf("OLDSIZE : %d\n", oldsize);
        for (int i=0; i<oldsize; i++) {
            list = dequeue(oldq);
            // list = pop(stack);
            firstDLL(list);
            // lastDLL(list);
            while (moreDLL(list) == 1) {
                BNODE *curr = currentDLL(list);
                // printf("CURR: ");
                // b->display(curr->key, stdout);
                // printf("\n");
                if (sizeDLL(curr->children) > 0) {
                    // printf("this node has children\n");
                    enqueue(newq, curr->children);
                    // push(news, curr->children);
                }
                nextDLL(list);
                // prevDLL(list);
                x ++;
            }
        }
        // printf("here\n");
        int newsize = sizeQUEUE(newq);
        // int newsize = sizeSTACK(news);
        // printf("NEWSIZE : %d\n", newsize);
        for (int i=0; i<newsize; i++) {
            // printf("IN THIS DISPLAY LOOP\n");
            list = dequeue(newq);
            // list = pop(news);
            // displayDLLordered(b, list, fp);
            displayDLL(list, fp);
            enqueue(oldq, list);
            // push(stack, list);
            // x ++;
        }
        // printf("OUT OF THE DISPLAY LOOP\n");
        printf("\n");
    }
    while (sizeQUEUE(oldq) > 0) {
        dequeue(oldq);
    }
    // printf("RETURNING\n");
    freeQUEUE(oldq);
    freeQUEUE(newq);
    // printf("\n");
    return;
}

extern void
freeBINOMIAL(BINOMIAL *b) {
    QUEUE *oldq = newQUEUE(b->display, b->free);
    // QUEUE *newq = newQUEUE(b->display, b->free);
    DLL *list = b->rootList;
    enqueue(oldq, list);

    int x = 0;
    // printf("X: %d\n", x);
    // firstDLL(list);
    // while (x < sizeBINOMIAL(b)) {
    while (sizeQUEUE(oldq) > 0) {
        // printf("IN THE BIG WHILE\n");
        int oldsize = sizeQUEUE(oldq);
        // printf("Q SIZE IS: %d\n", oldsize);
        for (int i=0; i<oldsize; i++) {
            list = dequeue(oldq);
            // printf("DEQUEUED THE LIST\n");

            firstDLL(list);
            // lastDLL(list);
            // printf("FIRSTED\n");
            while (moreDLL(list) == 1) {
                // printf("IN THE MORE WHILE\n");
                BNODE *curr = currentDLL(list);
                if (curr->children != NULL) {
                    // b->display(curr->key, stdout);
                    // printf(" : this node has children\n");
                    enqueue(oldq, curr->children);
                }
                nextDLL(list);
                // prevDLL(list);
            }
            // printf("About to free a DLL: \n");
            // displayDLL(list, stdout);
            // printf("\n");
            freeDLL(list);
            // printf("FREED THE DLL\n");

            x++;
            // printf("X: %d\n", x);
        }
    }
    freeQUEUE(oldq);
    free(b);
    return;
}

/*------------------- Auxiliary Functions --------------------*/

void consolidate(BINOMIAL *b) {
    // printf("\nIN CONSOLIDATE\n");
    int size;
    DLL *list = b->rootList;
    // printf("Rootlist size is: %d\n", sizeDLL(list));
    // printf("Rootlist: ");
    // displayDLLordered(b, list, stdout);
    // firstDLL(list);
    // for (int i=0; i<sizeDLL(list); i++) {
    // while (moreDLL(list) == 1) {
    //     BNODE *curr = currentDLL(list);
    //     b->display(curr->key, stdout);
    //     printf(" ");
    //     nextDLL(list);
    // }
    // printf("\n");

    double x = log((double)b->size);
    // printf("DOUBLE X: %f\n", x);
    double y = log(2.0);
    // printf("DOUBLE Y: %f\n", y);
    double result = x/y;
    // printf("result: %f\n", result);
    size = (int)result + 1;

    // printf("BINOMIAL size is: %d\nD array size is: %d\nRootlist size is: %d\n", b->size, size, sizeDLL(b->rootList));
    BNODE **array = malloc(sizeof(BNODE *) * size);
    // printf("Created the array\n");
    for (int i=0; i<size; i++) {
        array[i] = NULL;
    }

    int i = 0;
    firstDLL(list);
    // lastDLL(list);
    while (moreDLL(list) == 1) {
    // while (sizeDLL(list) != 0) {
        // printf("in the while, moreDLL is %d\n", moreDLL(list));
        // firstDLL(list);
        BNODE *spot = currentDLL(list);

        nextDLL(list);
        // prevDLL(list);
        // printf("TRYING TO REMOVE: ");
        // b->display(spot->key, stdout);
        // printf("\n");
        // printf("\nfrom CONSOLIDATE, ");
        spot = removeDLLnode(list, getBNODEowner(spot));
        // printf("SPOT after remove: ");
        // b->display(spot->key, stdout);
        // printf("\n");
        // printf("Array before Consolidating: \n[");
        // for (int i=0; i<size; i++) {
        //     if (array[i]) {
        //         b->display(getBNODEkey(array[i]), stdout);
        //     }
        //     else {printf("NULL");}
        //     printf(" ");
        // }
        // printf("]\n");
        updateConsolidationArray(b, array, spot);
        i++;
        // nextDLL(list);
        // printf("-------MOREDLL: %d\n", moreDLL(list));
    }

    b->extreme = NULL;
    // printf("Set extreme to null, about to go into for loop\n");
    // printf("New array after each pass: \n");
    for (int i=0; i<size; i++) {
    // for (int i=size-1; i>=0; i--) {
        BNODE *node = array[i];
        if (node != NULL) {
            // printf("array[%d] is NOT NULL\nDLL size is %d\n", i, sizeDLL(list));
            // void *temp = insertDLL(list, 0, node);
            void *temp = insertDLL(list, sizeDLL(list), node);
            node->owner = temp;

            // printf("inserted DLL\n");
            if (b->extreme == NULL) {
                // printf("extreme is NULL\n");
                b->extreme = array[i];
            }
            else if (b->extreme != NULL) {
                // printf("hello\nnodes are : ");
                // b->display(getBNODEkey(b->extreme), stdout);
                // printf(" and ");
                // b->display(getBNODEkey(array[i]), stdout);
                // printf("\n");
                int res = b->compare(getBNODEkey(b->extreme), getBNODEkey(array[i]));
                // printf("RESULT OF COMPARE : %d\n", res);
                // if (b->compare(getBNODEkey(b->extreme), getBNODEkey(array[i])) > 0) {
                if (res > 0) {
                    // printf("Setting the extreme to : ");
                    // b->display(getBNODEkey(array[i]), stdout);
                    // printf("\n");
                    b->extreme = array[i];
                }
            }
        }
        // displayDLLordered(b, list, stdout);
        // printf("\n");
    }
    // printf("made it out of the for loop\n");
    // for (int i=0; i<size; i++) {
    //     // freeBNODE(array[i]);
    //     printf("trying to free: ");
    //     b->display(getBNODEkey(array[i]), stdout);
    //     printf("\n");
    //     free(array[i]);
    // }
    free(array);
}

void updateConsolidationArray(BINOMIAL *b, BNODE **array, BNODE *node) {
    // printf("UPDATING CONSOLIDATION ARRAY\n");
    int degree = sizeDLL(node->children);
    // printf("Degree: %d\n", degree);

    // if (array[degree]) {
    //     printf("WHATS UP TESTING THIS NULL BULLSHIT\n");
    //     printf("array[%d] = ", degree);
    //     b->display(node->key, stdout);
    //     printf("!\n");
    // }
    // else {printf("array[%d] = NULL\n", degree);}

    while (array[degree]) {
        // printf("array[%d] is NOT NULL\n", degree);

        node = combine(b, node, array[degree]);
        array[degree] = NULL;
        degree ++;
    }

    array[degree] = node;
    // printf("array[%d] = ", degree);
    // b->display(node->key, stdout);
    // printf("\nDONE UPDATING CONSOLIDATION ARRAY\n\n");
    return;
}

BNODE *combine(BINOMIAL *b, BNODE *n1, BNODE *n2) {
    // printf("IN COMBINE\nCOMBINING ");
    // b->display(n1->key, stdout);
    // printf(" and ");
    // b->display(n2->key, stdout);
    // printf("\n");
    if (b->compare(n1->key, n2->key) >= 0) {
        // printf("N1: ");
        // b->display(n1->key, stdout);
        // printf(" going into N2: ");
        // b->display(n2->key, stdout);
        // printf("\n");
        // void *temp = insertDLL(n2->children, 0, n1);
        void *temp = insertDLL(n2->children, sizeDLL(n2->children), n1);
        n1->owner = temp;
        n1->parent = n2;
        return n2;
    }
    else {
        // printf("N2: ");
        // b->display(n2->key, stdout);
        // printf(" going into N1: ");
        // b->display(n1->key, stdout);
        // printf("\n");
        // void *temp = insertDLL(n1->children, 0, n2);
        void *temp = insertDLL(n1->children, sizeDLL(n1->children), n2);
        n2->owner = temp;
        n2->parent = n1;
        return n1;
    }
}

void *bubbleUp(BINOMIAL *b, void *node) {
    BNODE *n = node;
    BNODE *p = n->parent;
    if (n == p) {
        return n;
    }
    else if (b->compare(n->key, p->key) > 0) {
        return n;
    }
    else {
        if (b->update) {
            b->update(n->key, p);
            b->update(p->key, n);
        }
        swapValues(n, p);
        return bubbleUp(b, p);
    }
}

//TODO: OWNER IS A DLL NODE, NOT A BNODE
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

// void
// displayBINOMIALlevel(FILE *fp, int count, QUEUE *list)
// {
//   DLL *curr = dequeue(list);
//   while (count > 0)
//   {
//     displayDLL(curr, fp);
//     firstDLL(curr);
//     for (int i=0; i<sizeDLL(curr); i++)
//     {
//       BNODE *temp = currentDLL(curr);
//       if (temp)
//       {
//         if (sizeDLL(temp->children) > 0) enqueue(list, temp->children);
//       }
//       nextDLL(curr);
//     }
//     count--;
//     if (count != 0) curr = dequeue(list);
//   }
//   printf("\n");
//   if (sizeQUEUE(list) > 0)
//   {
//     displayBINOMIALlevel(fp,sizeQUEUE(list), list);
//   }
//   return;
// }
