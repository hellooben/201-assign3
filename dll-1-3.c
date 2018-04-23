#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "integer.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1004);
    DLL *p = newDLL(displayINTEGER,freeINTEGER);
    int i;
    for (i = 0; i < 14; ++i)
        {
        int j = random() % 100;
        insertDLL(p,0,newINTEGER(j));
        }
    if (sizeDLL(p) < 100)
        {
        displayDLL(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeDLL(p));
    printf("value at 5 is %d\n",getINTEGER(getDLL(p,5)));
    printf("setting value at 5 to 61\n");
    freeINTEGER(setDLL(p,5,newINTEGER(61)));
    printf("value at 5 now is %d\n",getINTEGER(getDLL(p,5)));
    freeDLL(p);
    return 0;
    }
