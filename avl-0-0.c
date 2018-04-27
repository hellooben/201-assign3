#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "queue.h"
#include "bst.h"
#include "avl.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(41);
    //simple INTEGER test of AVL
    AVL *p = newAVL(displayINTEGER,compareINTEGER,freeINTEGER);
    // printf("MADE THE NEW AVL\n");
    insertAVL(p,newINTEGER(2));
    // printf("inserted the first one\n");
    insertAVL(p,newINTEGER(3));
    insertAVL(p,newINTEGER(1));
    // printf("INSERTED\n");
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    INTEGER *q = newINTEGER(2);
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    INTEGER *r = newINTEGER(3);
    freeINTEGER(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    setINTEGER(r,2);
    freeINTEGER(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeAVL(p));
    freeINTEGER(q);
    freeINTEGER(r);
    freeAVL(p);
    return 0;
    }
