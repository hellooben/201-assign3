#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "integer.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1010);
    DLL *p = newDLL(displayINTEGER,freeINTEGER);
    int i;
    for (i = 0; i < 1000000; ++i)
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
    printf("value at 979551 is %d\n",getINTEGER(getDLL(p,979551)));
    printf("setting value at 979551 to 76\n");
    freeINTEGER(setDLL(p,979551,newINTEGER(76)));
    printf("value at 979551 now is %d\n",getINTEGER(getDLL(p,979551)));
    freeDLL(p);
    return 0;
    }
