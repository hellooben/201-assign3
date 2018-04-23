#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "string.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1003);
    DLL *p = newDLL(displaySTRING,freeSTRING);
    insertDLL(p,0,newSTRING("a"));
    insertDLL(p,sizeDLL(p),newSTRING("b"));
    displayDLL(p,stdout);
    printf("\n");
    freeSTRING(removeDLL(p,1));
    displayDLL(p,stdout);
    printf("\n");
    freeSTRING(removeDLL(p,0));
    displayDLL(p,stdout);
    printf("\n");
    DLL *q = newDLL(displaySTRING,freeSTRING);
    insertDLL(p,0,newSTRING("e"));
    insertDLL(p,0,newSTRING("f"));
    insertDLL(q,0,newSTRING("c"));
    insertDLL(q,0,newSTRING("d"));
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
