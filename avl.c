#include "avl.h"
#include "bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct aval
{
    void *data;
    int freq;
    int height;
    int balance;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*free)(void *);
} AVAL;

/*-------------PRIVATE FUNCTIONS-------------*/
AVAL *newAVAL(void*v,
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),           //comparator
void (*f)(void *));
void adisplay(void *,FILE *);
int compareAVAL(void *,void *);
void swapper(BSTNODE *,BSTNODE *);
void freeAVAL(void *);
void freeAVALalmost(AVAL *);
void *getAVAL(AVAL *);
int getFREQ(AVAL *);
void updateFREQ(AVAL *, int);
int getHeight(AVAL *);
void setHeight(AVAL *, int);
void addHeight(AVAL *, int);
int getBalance(AVAL *);
void setBalance(BSTNODE *);
void calculateHeight(BSTNODE *);
BSTNODE *findFavorite(BSTNODE *);
BSTNODE *getSibling(BSTNODE *);
int testLeft(BSTNODE *,BSTNODE *);
void insertionFixup(BST *, BSTNODE *);
void deleteFixup(BST *, BSTNODE *);
int isLinear(BSTNODE *, BSTNODE *, BSTNODE *);
void rotate(BST *,BSTNODE *,BSTNODE *);
void leftRotate(BST *,BSTNODE *,BSTNODE *);
void rightRotate(BST *,BSTNODE *,BSTNODE *);


void adisplay(void *val, FILE *fp) {
    AVAL *temp = val;
    temp->display(getAVAL(temp), fp);
    if (getFREQ(temp) > 1) {
        fprintf(fp, "[%d]", getFREQ(temp));
    }
    if (getBalance(temp) > 0) {
        fprintf(fp, "+");
    }
    if (getBalance(temp) < 0) {
        fprintf(fp, "-");
    }
    return;
}

int compareAVAL(void *v1, void *v2) {
    AVAL *temp = v1;
    AVAL *temp2 = v2;
    return temp->compare(getAVAL(temp),getAVAL(temp2));
}

void swapper(BSTNODE *a, BSTNODE *b) {
    AVAL *ta = getBSTNODEvalue(a);
    AVAL *tb = getBSTNODEvalue(b);

    void *temp = ta->data;
    ta->data = tb->data;
    tb->data = temp;

    int count = ta->freq;
    ta->freq = tb->freq;
    tb->freq = count;

    return;
}

void freeAVAL(void *val) {
    AVAL *temp = val;
    if (temp->free) {
        temp->free(getAVAL(temp));
        free(val);
    }
    return;
}

void freeAVALalmost(AVAL *val) {
    free(val);
    return;
}

void *getAVAL(AVAL *val) {
    return val->data;
}

int getFREQ(AVAL *val) {
    if (val != NULL) {
        return val->freq;
    }
    else {return 0;}
}

void updateFREQ(AVAL *val, int count) {
    val->freq += count;
    return;
}

int getHeight(AVAL *val) {
    return val->height;
}

void addHeight(AVAL *val, int num) {
    val->height += num;
    return;
}

void setHeight (AVAL *val, int num) {
    val->height = num;
    return;
}

int getBalance(AVAL *val) {
    return val->balance;
}

void setBalance(BSTNODE *node) {
    calculateHeight(node);

    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);

    AVAL *val = getBSTNODEvalue(node);
    if (l!=NULL && r!=NULL) {
        val->balance = getHeight(getBSTNODEvalue(l)) - getHeight(getBSTNODEvalue(r));
    }
    else if (l!=NULL && r==NULL) {
        val->balance = getHeight(getBSTNODEvalue(l)) + 1;
    }
    else if (r!= NULL && l == NULL) {
        val->balance = (-1) - getHeight(getBSTNODEvalue(r));
    }
    else {
        val->balance = 0;
    }
    return;
}

void calculateHeight(BSTNODE *node) {
    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);

    if (node == NULL) {return;}
    if (l == NULL && r == NULL) {
        setHeight(getBSTNODEvalue(node), 0);
        return;
    }
    else if (l != NULL && r == NULL) {
        setHeight(getBSTNODEvalue(node), getHeight(getBSTNODEvalue(l))+1);
    }
    else if (r != NULL && l == NULL) {
        setHeight(getBSTNODEvalue(node), getHeight(getBSTNODEvalue(r))+1);
    }
    else {
        int numl = getHeight(getBSTNODEvalue(l));
        int numr = getHeight(getBSTNODEvalue(r));
        if (numl > numr) {
            setHeight(getBSTNODEvalue(node), numl+1);
        }
        else {
            setHeight(getBSTNODEvalue(node), numr+1);
        }
    }
    return;
}

BSTNODE *findFavorite(BSTNODE *node) {
    if (node == NULL) {return NULL;}
    else if (getBalance(getBSTNODEvalue(node)) >= 1) {
        return getBSTNODEleft(node);
    }
    else if (getBalance(getBSTNODEvalue(node)) <= -1) {
        return getBSTNODEright(node);
    }
    else {
        return NULL;
    }
}

BSTNODE *getSibling(BSTNODE *node) {
    BSTNODE *parent = getBSTNODEparent(node);
    if (parent == node && parent != NULL) {return NULL;}
    if (getBSTNODEleft(parent) == node) {
        return getBSTNODEright(parent);
    }
    else if (getBSTNODEright(parent) == node) {
        return getBSTNODEleft(parent);
    }
    else {
        return NULL;
    }
}

int
testLeft(BSTNODE *node, BSTNODE *parent) {
    BSTNODE *maybe = getBSTNODEleft(parent);
    if (maybe != NULL && maybe == node) {
        return 1;
    }
    else {
        return 0;
    }
}

/*------------------------------------------*/

AVAL *newAVAL(void*v,
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),           //comparator
void (*f)(void *)) {
    AVAL *new = malloc(sizeof(AVAL));
    new->data = v;
    new->freq = 1;
    new->height = 0;
    new->balance = 0;
    new->display = d;
    new->compare = c;
    new->free = f;
    return new;
}

struct avl
{
    int size;
    BST *bstree;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*free)(void *);                    //free
};

extern AVL *
newAVL (
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),            //comparator
void (*f)(void *)) {                //free
    AVL *tree = malloc(sizeof(AVL));
    tree->size = 0;
    tree->bstree = newBST(adisplay, compareAVAL, swapper, freeAVAL);
    tree->display = d;
    tree->compare = c;
    tree->free = f;
    return tree;
}

extern void
insertAVL(AVL *a,void *value) {
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);

    if (temp != NULL) {
        updateFREQ(getBSTNODEvalue(temp), 1);
        a->size ++;
        freeAVALalmost(new);
    }
    else {
        BSTNODE *node = insertBST(a->bstree, new);
        insertionFixup(a->bstree, node);
        a->size ++;
    }
    return;
}

void insertionFixup(BST *t, BSTNODE *node) {
    while (1) {
        BSTNODE *parent = getBSTNODEparent(node);
        BSTNODE *fav = findFavorite(node);
        BSTNODE *sibling = getSibling(node);

        if (parent == node) {
            setBalance(node);
            break;
        }
        else if (findFavorite(parent) != NULL && findFavorite(parent) == sibling) { //case 1
            setBalance(parent);
            break;
        }
        else if (findFavorite(parent) == NULL) { // case 2
            setBalance(parent);
            node = parent;
            continue;
        }
        else {
            if (fav != NULL && isLinear(node, parent, fav) == 0) { //case 3
                // printf("case 3\n");
                rotate(t, fav, node);
                rotate(t, fav, parent);

                setBalance(fav);
                setBalance(parent);
                setBalance(node);
            }
            else { //case 4
                rotate(t, node, parent);
                setBalance(parent);
                setBalance(node);
            }
            break;
        }
    }
    return;
}

int isLinear(BSTNODE *node, BSTNODE *parent, BSTNODE *fav) {
    int total = 0;
    total += testLeft(node, parent);
    total += testLeft(fav, node);
    if (total == 2 || total == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void rotate(BST *t, BSTNODE *src, BSTNODE *dst) {
    if (testLeft(src, dst) == 0) {
        leftRotate(t, src, dst);
        return;
    }
    else {
        rightRotate(t, src, dst);
        return;
    }
}

void leftRotate(BST *t, BSTNODE *y, BSTNODE *x) {
    BSTNODE *yLeft = getBSTNODEleft(y);
    BSTNODE *xParent = getBSTNODEparent(x);

    setBSTNODEright(x, yLeft);

    if (yLeft != NULL) {
        setBSTNODEparent(yLeft, x);
    }

    setBSTNODEparent(y, xParent);

    if (xParent == x) {
        setBSTroot(t, y);
        setBSTNODEparent(y, y);
    }
    else if (testLeft(x, xParent) == 1) {
        setBSTNODEleft(xParent, y);
    }
    else {
        setBSTNODEright(xParent, y);
    }

    setBSTNODEleft(y, x);
    setBSTNODEparent(x, y);

    calculateHeight(x);
    calculateHeight(y);
    return;
}

void rightRotate(BST *t, BSTNODE *y, BSTNODE *x) {
    BSTNODE *yRight = getBSTNODEright(y);
    BSTNODE *xParent = getBSTNODEparent(x);

    setBSTNODEleft(x, yRight);

    if (yRight != NULL) {
        setBSTNODEparent(yRight, x);
    }

    setBSTNODEparent(y, xParent);

    if (xParent == x) {
        setBSTroot(t, y);
        setBSTNODEparent(y, y);
    }
    else if (testLeft(x, xParent) == 0) {
        setBSTNODEright(xParent, y);
    }
    else {
        setBSTNODEleft(xParent, y);
    }

    setBSTNODEright(y, x);
    setBSTNODEparent(x, y);

    calculateHeight(x);
    calculateHeight(y);

    return;
}

extern void *
deleteAVL(AVL *a,void *value) {
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);
    if (temp != NULL) {
        void *returnable = getAVAL(getBSTNODEvalue(temp));
        if (getFREQ(getBSTNODEvalue(temp)) > 1) {
            updateFREQ(getBSTNODEvalue(temp), -1);
            a->size --;
            freeAVALalmost(new);
            return NULL;
        }
        else {
            temp = swapToLeafBST(a->bstree, temp);
            deleteFixup(a->bstree, temp);
            pruneLeafBST(a->bstree, temp);

            setBSTsize(a->bstree, sizeBST(a->bstree)-1);
            a->size --;

            freeAVALalmost(new);
            freeBSTNODE(temp, a->free);
            return returnable;
        }
    }
    else {
        freeAVALalmost(new);
        return NULL;
    }
}

void deleteFixup(BST *t, BSTNODE *node) {
    setHeight(getBSTNODEvalue(node), -1);

    while (1) {
        BSTNODE *parent = getBSTNODEparent(node);
        BSTNODE *sibling = getSibling(node);

        if (parent == node) {
            break;
        }
        else if (findFavorite(parent)!=NULL && findFavorite(parent) == node) { //case 1
            setBalance(parent);
            node = parent;
            continue;
        }
        else if (findFavorite(parent) == NULL) { //case 2
            setBalance(parent);
            break;
        }
        else {
            BSTNODE *fav = findFavorite(sibling);
            if (fav != NULL && isLinear(sibling, parent, fav) == 0) { // case 3
                if (fav != sibling) {
                    rotate(t, fav, sibling);
                    rotate(t, fav, parent);
                    setBalance(parent);
                    setBalance(sibling);
                    setBalance(fav);
                    node = fav;
                    continue;
                }
            }
            else { //case 4
                rotate(t, sibling, parent);
                setBalance(parent);
                setBalance(sibling);
                if (fav == NULL) {
                    break;
                }
                node = sibling;
                continue;
            }
        }
    }
    return;
}

extern int
findAVLcount(AVL *a,void *value) {
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);
    if (temp != NULL) {
        freeAVALalmost(new);
        return getFREQ(getBSTNODEvalue(temp));
    }
    else {
        freeAVALalmost(new);
        return 0;
    }
}

extern void *
findAVL(AVL *a,void *value) {
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);
    if (temp != NULL) {
        void *returnable = getAVAL(getBSTNODEvalue(temp));
        freeAVALalmost(new);
        return returnable;
    }
    else {
        freeAVALalmost(new);
        return NULL;
    }
}

extern int
sizeAVL(AVL *a) {
    return sizeBST(a->bstree);
}

extern int
duplicatesAVL(AVL *a) {
    return a->size - sizeBST(a->bstree);
}

extern void
statisticsAVL(AVL *a,FILE *fp) {
    fprintf(fp, "Duplicates: %d\n", duplicatesAVL(a));
    statisticsBST(a->bstree, fp);
    return;
}

extern void
displayAVL(AVL *a,FILE *fp) {
    if (getBSTroot(a->bstree) == NULL) {
        fprintf(fp, "EMPTY\n");
        return;
    }
    else {
        displayBSTdecorated(a->bstree, fp);
    }
    return;
}

extern void
displayAVLdebug(AVL *a,FILE *fp) {
    if (getBSTroot(a->bstree) == NULL) {
        return;
    }
    displayBST(a->bstree, fp);
    return;
}

extern void
freeAVL(AVL *a) {
    freeBST(a->bstree);
    free(a);
    return;
}
