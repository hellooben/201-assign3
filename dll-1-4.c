#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "integer.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1005);
    DLL *p = newDLL(displayINTEGER,freeINTEGER);
    int i;
    for (i = 0; i < 15; ++i)
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
    printf("value at 9 is %d\n",getINTEGER(getDLL(p,9)));
    printf("setting value at 9 to 93\n");
    freeINTEGER(setDLL(p,9,newINTEGER(93)));
    printf("value at 9 now is %d\n",getINTEGER(getDLL(p,9)));
    freeDLL(p);
    return 0;
    }
