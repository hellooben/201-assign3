#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"
#include "avl.h"
#include "binomial.h"
#include "integer.h"
#include "vertex.h"
#include "edge.h"
#include "dll.h"

static void update(void *v,void *n);
void MST_Prim(BINOMIAL *,AVL *,VERTEX *);

int main (int argc, char **argv) {
    FILE *data;
    AVL *vertices = newAVL(displayVERTEX, compareVERTEX, freeVERTEX);
    AVL *edges = newAVL(displayEDGE, compareEDGE, freeEDGE);
    BINOMIAL *bin = newBINOMIAL(displayVERTEX, compareVERTEX, update, freeVERTEX);

    if (strcmp(argv[1], "-v") == 0) {
        printf("Author: Ben Bailey\n");
        return 0;
    }
    else {
        data = fopen(argv[argc-1], "r");
    }

    int root = 0;
    VERTEX *rootVertex = NULL;

    char *temp = NULL;
    int v1 = 0;
    int v2 = 0;
    int edge = 0;

    temp = readToken(data);

    while (!feof(data)) {
        // printf("hey\n");

        v1 = atoi(temp);
        if (root == 0) {
            rootVertex = newVERTEX(v1);
            insertAVL(vertices, rootVertex);
            void *t = insertBINOMIAL(bin, rootVertex);
            setVERTEXowner(rootVertex, t);
            root = 1;
        }
        temp = readToken(data);
        v2 = atoi(temp);

        if (v1 == v2) {
            temp = readToken(data);
            if (strcmp(temp, ";") != 0) {
                readToken(data);
            }
            temp = readToken(data);
            continue;
        }

        temp = readToken(data);
        if (strcmp(temp, ";") != 0) {
            edge = atoi(temp);
            EDGE *ed = newEDGE(v1, v2, edge);
            EDGE *de = newEDGE(v2, v1, edge);

            if (findAVL(edges, ed) == NULL && findAVL(edges, de) == NULL) {
                insertAVL(edges, ed);
                VERTEX *one = newVERTEX(v1);
                VERTEX *two = newVERTEX(v2);
                VERTEX *find = findAVL(vertices, one);
                VERTEX *find2 = findAVL(vertices, two);

                if (find != NULL && find2 != NULL) {
                    one = find;
                    two = find2;
                }
                else if (find != NULL && find2 == NULL) {
                    one = find;
                    insertAVL(vertices, two);
                    void *t = insertBINOMIAL(bin, two);
                    setVERTEXowner(two, t);
                }
                else if (find == NULL && find2 != NULL) {
                    two = find2;
                    insertAVL(vertices, one);
                    void *t = insertBINOMIAL(bin, one);
                    setVERTEXowner(one, t);
                }
                else {
                    insertAVL(vertices, one);
                    void *t = insertBINOMIAL(bin, one);
                    setVERTEXowner(one, t);
                    insertAVL(vertices, two);
                    t = insertBINOMIAL(bin, two);
                    setVERTEXowner(two, t);
                }

                // insertDLL(getVERTEXneighbors(one), sizeDLL(getVERTEXneighbors(one)), two);
                // insertDLL(getVERTEXweights(one), sizeDLL(getVERTEXweights(one)), newINTEGER(edge));
                // insertDLL(getVERTEXneighbors(two), sizeDLL(getVERTEXneighbors(two)), one);
                // insertDLL(getVERTEXweights(two), sizeDLL(getVERTEXweights(two)), newINTEGER(edge));
                insertVERTEXneighbor(one, two);
                insertVERTEXneighbor(two, one);
                insertVERTEXweight(one, edge);
                insertVERTEXweight(two, edge);
            }

            // printf("EDGE: %d\n", edge);
            readToken(data);
        }
        else {
            // printf("adding the default edge\n");
            EDGE *edg = newEDGE(v1, v2, 1);
            EDGE *gde = newEDGE(v2, v1, 1);
            if (findAVL(edges, edg) == NULL && findAVL(edges, gde) == NULL) {
                insertAVL(edges, edg);
                VERTEX *one = newVERTEX(v1);
                VERTEX *two = newVERTEX(v2);
                void *find = findAVL(vertices, one);
                void *find2 = findAVL(vertices, two);

                if (find != NULL && find2 != NULL) {
                    one = find;
                    two = find2;
                }
                else if (find != NULL && find2 == NULL) {
                    one = find;
                    insertAVL(vertices, two);
                    void *t = insertBINOMIAL(bin, two);
                    setVERTEXowner(two, t);
                }
                else if (find == NULL && find2 != NULL) {
                    two = find2;
                    insertAVL(vertices, one);
                    void *t = insertBINOMIAL(bin, one);
                    setVERTEXowner(one, t);
                }
                else {
                    insertAVL(vertices, one);
                    void *t = insertBINOMIAL(bin, one);
                    setVERTEXowner(one, t);
                    insertAVL(vertices, two);
                    t = insertBINOMIAL(bin, two);
                    setVERTEXowner(two, t);
                }

                // insertDLL(getVERTEXneighbors(one), sizeDLL(getVERTEXneighbors(one)), two);
                // insertDLL(getVERTEXweights(one), sizeDLL(getVERTEXweights(one)), newINTEGER(1));
                // insertDLL(getVERTEXneighbors(two), sizeDLL(getVERTEXneighbors(two)), one);
                // insertDLL(getVERTEXweights(two), sizeDLL(getVERTEXweights(two)), newINTEGER(1));
                insertVERTEXneighbor(one, two);
                insertVERTEXneighbor(two, one);
                insertVERTEXweight(one, edge);
                insertVERTEXweight(two, edge);
            }
        }
        // if (feof(data)) {printf("breaking\n");break;}
        temp = readToken(data);
    }

    printf("Out of the data processing loop!\nVertices AVL looks like: \n");
    displayAVL(vertices, stdout);
    printf("\n");
    printf("Edges AVL looks like: \n");
    displayAVL(edges, stdout);
    printf("\n");
    printf("Priority Queue looks like: \n");
    displayBINOMIALdebug(bin, stdout);
    printf("\n");

    MST_Prim(bin, edges, rootVertex);

    return 0;
}

/********************************************/

static void
update(void *v,void *n) {
    VERTEX *p = v;
    setVERTEXowner(p, n);
    return;
}

void
MST_Prim(BINOMIAL *b, AVL *e, VERTEX *root) {
    printf("WELCOME TO PRIM\nBinomial:\n");
    displayBINOMIALdebug(b, stdout);
    printf("\n");
    // decreaseKeyBINOMIAL(b, getVERTEXowner(root), newINTEGER(-1));
    setVERTEXkey(root, 0);
    decreaseKeyBINOMIAL(b, getVERTEXowner(root), root);
    printf("Binomial, after decreasing root key:\n");
    displayBINOMIALdebug(b, stdout);
    printf("\n");
    printf("sizeBINOMIAL: %d\n", sizeBINOMIAL(b));
    while (sizeBINOMIAL(b) > 0) {

        VERTEX *u = extractBINOMIAL(b);
        printf("extracted\n");
        displayVERTEX(u, stdout);
        printf(" was just extracted.\nBinomial, after an extract min:\n");
        displayBINOMIALdebug(b, stdout);
        printf("\n");

        setVERTEXflag(u, 1);
        printf("The vertex flag was set to: %d\n", getVERTEXflag(u));

        DLL *nbors = getVERTEXneighbors(u);
        displayVERTEX(u, stdout);
        printf("'s Neighbors: \n");
        displayDLL(nbors, stdout);
        printf("\n");

        firstDLL(nbors);
        int x = 0;

        while (moreDLL(nbors)) {
            printf("moreDLL returning true\nx:%d\n", x);
            VERTEX *v = currentDLL(nbors);
            printf("VERTEX under question: \n");
            displayVERTEXdebug(v, stdout);
            printf("\n");

            int wght = getINTEGER(getDLL(getVERTEXweights(u), x));
            printf("WGHT: %d\n", wght);
            x++;

            if (getVERTEXflag(v) == 0 && wght < getVERTEXkey(v)) {
                printf("node isn't in the tree, and weight[x] < the current's key\n");
                setVERTEXpred(v, u);
                // decreaseKeyBINOMIAL(b, getVERTEXowner(v), newINTEGER(wght));
                setVERTEXkey(v, wght);
                decreaseKeyBINOMIAL(b, getVERTEXowner(v), v);
            }
            nextDLL(nbors);
        }
    }
    return;
}
