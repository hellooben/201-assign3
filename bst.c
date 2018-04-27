#include "bst.h"
#include "sll.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

BSTNODE *binaryInsert(BST *t, BSTNODE *node, BSTNODE *prev, void *value);
BSTNODE *binaryFind(BST *t, BSTNODE *node, void *value);
void displayRecursive(BST *t, BSTNODE *root, FILE *fp);
void freeRecursive(BSTNODE *node, BST *t);
void levelOrder(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq);
int BSTmaximum(BSTNODE *root);
int BSTminimum(BSTNODE *root);
void levelOrderDecorated(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq);
int isLeft(BST *t, BSTNODE *node, BSTNODE *parent);

struct bstnode
{
    void *data;
    struct bstnode *parent;
    struct bstnode *left;
    struct bstnode *right;
};

extern BSTNODE *
newBSTNODE(void *v) {
    BSTNODE *new = malloc(sizeof(BSTNODE));
    new->data = v;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    return new;
}

extern void *
getBSTNODEvalue(BSTNODE *n) {
    return n->data;
}

extern void
setBSTNODEvalue(BSTNODE *n,void *value) {
    n->data = value;
    return;
}

extern BSTNODE *
getBSTNODEleft(BSTNODE *n) {
    if (n->left != NULL) {
        return n->left;
    }
    else {
        return NULL;
    }
}

extern void
setBSTNODEleft(BSTNODE *n,BSTNODE *replacement) {
    n->left = replacement;
    return;
}

extern BSTNODE *
getBSTNODEright(BSTNODE *n) {
    if (n->right != NULL) {
        return n->right;
    }
    else {
        return NULL;
    }
}

extern void
setBSTNODEright(BSTNODE *n,BSTNODE *replacement) {
    n->right = replacement;
    return;
}

extern BSTNODE *
getBSTNODEparent(BSTNODE *n) {
    return n->parent;
}

extern void
setBSTNODEparent(BSTNODE *n,BSTNODE *replacement) {
    n->parent = replacement;
    return;
}

extern void
freeBSTNODE(BSTNODE *n,void (*f)(void *)) {
    if (f != NULL) {
        f(n->data);
        free(n);
    }
    return;
}


/*----------------------------------------------------------------------*/


struct bst
{
    int size;
    BSTNODE *root;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*swap)(BSTNODE *,BSTNODE *);     //swapper
    void (*free)(void *);                  //free
};

extern BST *
newBST(void (*d)(void *,FILE *),
int (*c)(void *,void *),
void (*s)(BSTNODE *,BSTNODE *),
void (*f)(void *)) {
    BST *tree = malloc(sizeof(BST));
    assert(tree!=0);
    tree->size = 0;
    tree->root = NULL;
    tree->display = d;
    tree->compare = c;
    tree->swap = s;
    tree->free = f;
    return tree;
}

extern BSTNODE *
getBSTroot(BST *t) {
    if (t->root != NULL) {
        return t->root;
    }
    else {
        return NULL;
    }
}

extern void
setBSTroot(BST *t,BSTNODE *replacement) {
    t->root = replacement;
    return;
}

extern void
setBSTsize(BST *t,int s) {
    t->size = s;
    return;
}

extern BSTNODE *
insertBST(BST *t,void *value) {
    if (t->root == NULL) {
        BSTNODE *new = newBSTNODE(value);
        assert(new!=0);
        t->root = new;
        setBSTNODEparent(t->root, new);
        t->size ++;
        return new;
    }
    else {
        BSTNODE *returnable = binaryInsert(t, t->root, t->root, value);
        assert(returnable!=0);
        t->size ++;
        return returnable;
    }
}

BSTNODE *
binaryInsert(BST *t, BSTNODE *node, BSTNODE *prev, void *value) {
    if (node == NULL) {
        BSTNODE *new = newBSTNODE(value);
        new->parent = prev;
        if (t->compare(prev->data, value) < 0) {
            prev->right = new;
        }
        else {
            prev->left = new;
        }
        return new;
    }
    if (t->compare(node->data, value) < 0) {
        return binaryInsert(t, node->right, node, value);
    }
    else if (t->compare(node->data, value) > 0) {
        return binaryInsert(t, node->left, node, value);
    }
    else {
        return node;
    }
}

extern BSTNODE *
findBST(BST *t,void *value) {
    if (t->root == NULL) {
        return NULL;
    }
    else {
        BSTNODE *curr = t->root;
        BSTNODE *returnable = binaryFind(t, curr, value);
        return returnable;
    }
}

BSTNODE *
binaryFind(BST *t, BSTNODE *node, void *value) {
    if (node == NULL) {
        return node;
    }
    if (t->compare(node->data, value) == 0) {
        return node;
    }
    else if (t->compare(node->data, value) > 0) {
        return binaryFind(t, node->left, value);
    }
    else if (t->compare(node->data, value) < 0) {
        return binaryFind(t, node->right, value);
    }
    else {
        return NULL;
    }
}

extern BSTNODE *
deleteBST(BST *t,void *value) {
    BSTNODE *current = findBST(t, value);
    current = swapToLeafBST(t, current);
    pruneLeafBST(t, current);
    t->size --;
    return current;
}

extern BSTNODE *
swapToLeafBST(BST *t,BSTNODE *node) {
    if (t->size == 1) {
        return node;
    }
    else if (node->left == NULL && node->right == NULL) {
        return node;
    }
    else {
        BSTNODE *temp = node;
        if (node->right != NULL) {
            node = node->right;
            while (node->left != NULL) {
                node = node->left;
            }
        }
        else {
            node = node->left;
            while (node->right != NULL) {
                node = node->right;
            }
        }
        if (t->swap) {
            t->swap(temp, node);
        }
        else {
            void *data = getBSTNODEvalue(temp);
            setBSTNODEvalue(temp, getBSTNODEvalue(node));
            setBSTNODEvalue(node, data);
        }
        return swapToLeafBST(t, node);
    }
}

extern void
pruneLeafBST(BST *t,BSTNODE *leaf) {
    if (getBSTNODEleft(leaf) || getBSTNODEright(leaf)) {
        return;
    }
    else {
        if (leaf->parent == leaf) {
            t->root = NULL;
            return;
        }
        BSTNODE *par = getBSTNODEparent(leaf);
        if (par->left != NULL) {
            if (t->compare(getBSTNODEvalue(par->left), getBSTNODEvalue(leaf)) == 0) {
                par->left = NULL;
                leaf->parent = NULL;
                return;
            }
        }
        if (par->right != NULL) {
            par->right = NULL;
            leaf->parent = NULL;
            return;
        }
        return;
    }
}

extern int
sizeBST(BST *t) {
    return t->size;
}

extern void
statisticsBST(BST *t,FILE *fp) {
    BSTNODE *current = t->root;
    if (current == NULL) {
        fprintf(fp, "Nodes: %d\nMinimum depth: %d\nMaximum depth: %d\n", 0, -1, -1);
        return;
    }
    else {
        // printf("gonna find the min and max\n");
        int min = BSTminimum(t->root);
        int max = BSTmaximum(t->root);
        // printf("found the min and max\n");
        fprintf(fp, "Nodes: %d\nMinimum depth: %d\nMaximum depth: %d\n", sizeBST(t), min, max);

        return;
    }
}

int
BSTminimum(BSTNODE *root) {
    if (root == NULL) {
        return -1;
    }
    else {
        int leeft = BSTminimum(root->left);
        int reet = BSTminimum(root->right);

        if (leeft < reet) {
            return leeft + 1;
        }
        else {
            return reet + 1;
        }
    }
}

int
BSTmaximum(BSTNODE *root) {
    if (root == NULL) {
        return -1;
    }
    else {
        int leeft = BSTmaximum(root->left);
        int reet = BSTmaximum(root->right);

        if (leeft > reet) {
            return leeft + 1;
        }
        else {
            return reet + 1;
        }
    }
}

extern void
displayBST(BST *t,FILE *fp) {
    if (t->root == NULL) {
        printf("[]");
        return;
    }
    else {
        displayRecursive(t, t->root, fp);
        return;
    }
}

extern void
displayBSTdebug(BST *t,FILE *fp) {
    if (t->root == NULL) {
        return;
    }
    else {
        QUEUE *newq = newQUEUE(t->display, t->free);
        QUEUE *oldq = newQUEUE(t->display, t->free);
        levelOrder(t, t->root, fp, newq, oldq);
        freeQUEUE(newq);
        freeQUEUE(oldq);
        return;
    }
}

void
levelOrder(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq) {
    int j = 0;
    t->display(node->data, fp);
    printf("\n");
    j ++;
    enqueue(oldq, node);

    while (j<sizeBST(t)) {
        int oldsize = sizeQUEUE(oldq);
        for (int i=0; i<oldsize; i++) {
            node = dequeue(oldq);
            if (node->left != NULL) {
                enqueue(newq, node->left);
            }
            if (node->right != NULL) {
                enqueue(newq, node->right);
            }
        }
        int newsize = sizeQUEUE(newq);
        for (int x=0; x<newsize; x++) {
            node = dequeue(newq);
            t->display(node->data, fp);
            enqueue(oldq, node);
            if (x < newsize-1) {printf(" ");}
            j ++;
        }
        printf("\n");
    }
    while (sizeQUEUE(oldq) > 0) {
        dequeue(oldq);
    }
    return;
}

extern void
displayBSTdecorated(BST *t,FILE *fp) {
    if (t->root == NULL) {
        return;
    }
    else {
        QUEUE *newq = newQUEUE(t->display, t->free);
        QUEUE *oldq = newQUEUE(t->display, t->free);
        levelOrderDecorated(t, t->root, fp, newq, oldq);
        freeQUEUE(newq);
        freeQUEUE(oldq);
        return;
    }
}

void
levelOrderDecorated(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq) {
    int j = 0;
    int lines = 0;
    fprintf(fp, "%d: ", lines);
    t->display(node->data, fp);
    fprintf(fp, "(");
    t->display(getBSTNODEparent(node)->data, fp);
    fprintf(fp, ")");
    fprintf(fp, "X");
    fprintf(fp, "\n");
    j ++;
    lines ++;
    enqueue(oldq, node);

    while (j<sizeBST(t)) {
        int oldsize = sizeQUEUE(oldq);
        for (int i=0; i<oldsize; i++) {
            node = dequeue(oldq);
            if (node->left != NULL) {
                enqueue(newq, node->left);
            }
            if (node->right != NULL) {
                enqueue(newq, node->right);
            }
        }
        int newsize = sizeQUEUE(newq);
        fprintf(fp, "%d: ", lines);
        for (int x=0; x<newsize; x++) {
            node = dequeue(newq);
            if (getBSTNODEleft(node)==NULL && getBSTNODEright(node)==NULL) {
                fprintf(fp, "=");
            }
            t->display(node->data, fp);
            fprintf(fp, "(");
            t->display(getBSTNODEvalue(getBSTNODEparent(node)), fp);
            fprintf(fp, ")");
            if (isLeft(t, node, getBSTNODEparent(node)) == 1) {
                fprintf(fp, "L");
            }
            else {
                fprintf(fp, "R");
            }
            enqueue(oldq, node);
            if (x < newsize-1) {fprintf(fp, " ");}
            j ++;
        }
        fprintf(fp, "\n");
        lines ++;
    }
    while (sizeQUEUE(oldq) > 0) {
        dequeue(oldq);
    }
    return;

}

int
isLeft(BST *t, BSTNODE *node, BSTNODE *parent) {
    BSTNODE *maybe = getBSTNODEleft(parent);
    if (maybe != NULL && t->compare(getBSTNODEvalue(node),getBSTNODEvalue(maybe)) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

extern void freeBST(BST *t) {
    if (t->root == NULL) {
        free(t);
        return;
    }
    else {
        BSTNODE *curr = t->root;
        freeRecursive(curr, t);
        free(t);
        return;
    }
}

void
freeRecursive(BSTNODE *node, BST *t) {
    if (node == NULL) {
        return;
    }
    BSTNODE *temp = node;
    if (temp->right != NULL && t->free != NULL) {
        freeRecursive(getBSTNODEright(temp), t);
    }
    if (temp->left != NULL && t->free != NULL) {
        freeRecursive(getBSTNODEleft(temp), t);
    }
    if (t->free != NULL) {
        freeBSTNODE(temp, t->free);
    }
}

void
displayRecursive(BST *t, BSTNODE *root, FILE *fp) {
    if (root == NULL) {
        printf("[]");
        return;
    }

    printf("[");
    t->display(root->data, fp);

    if (root->left != NULL) {
        printf(" ");
        displayRecursive(t, root->left, fp);
    }
    if (root->right != NULL) {
        printf(" ");
        displayRecursive(t, root->right, fp);
    }
    printf("]");
}
