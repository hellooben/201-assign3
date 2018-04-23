#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "real.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1002);
    DLL *p = newDLL(displayREAL,freeREAL);
    insertDLL(p,0,newREAL(3.3));
    insertDLL(p,sizeDLL(p),newREAL(2.2));
    displayDLL(p,stdout);
    printf("\n");
    freeREAL(removeDLL(p,1));
    displayDLL(p,stdout);
    printf("\n");
    freeREAL(removeDLL(p,0));
    displayDLL(p,stdout);
    printf("\n");
    DLL *q = newDLL(displayREAL,freeREAL);
    insertDLL(p,0,newREAL(6.7));
    insertDLL(p,0,newREAL(8.9));
    insertDLL(q,0,newREAL(4.3));
    insertDLL(q,0,newREAL(5.2));
    displayDLL(p,stdout);
    printf("\n");
    displayDLL(q,stdout);
    printf("\n");
    unionDLL(p,q);
    displayDLL(p,stdout);
    displayDLL(q,stdout);
    printf("\n");
    displayDLLdebug(p,stdout);
    printf("\n");
    displayDLLdebug(q,stdout);
    printf("\n");
    freeDLL(p);
    freeDLL(q);
    return 0;
    }
