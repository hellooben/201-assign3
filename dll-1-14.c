#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "integer.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1015);
    DLL *p = newDLL(displayINTEGER,freeINTEGER);
    int i;
    for (i = 0; i < 1000000; ++i)
        {
        int j = random() % 100;
        int k = random() % 5 + 1;
        if (i < k) k = i;
        insertDLL(p,i - k,newINTEGER(j));
        }
    printf("done inserting\n");
    for (i = 0; i < 999980; ++i)
        {
        int j = sizeDLL(p);
        int k = random() % 5 + 1;
        if (j < k) k = j;
        INTEGER *x = getDLL(p,j - k);
        int value = getINTEGER(x);
        freeINTEGER(setDLL(p,j - k,newINTEGER(value+1)));
        }
    printf("done getting and setting\n");
    for (i = 0; i < 999980; ++i)
        {
        int j = sizeDLL(p);
        int k = random() % 5 + 1;
        if (j < k) k = j;
        freeINTEGER(removeDLL(p,j - k));
        }
    printf("done removing\n");
    if (sizeDLL(p) < 100)
        {
        displayDLL(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeDLL(p));
    printf("value at 4 is %d\n",getINTEGER(getDLL(p,4)));
    printf("setting value at 4 to 23\n");
    freeINTEGER(setDLL(p,4,newINTEGER(23)));
    printf("value at 4 now is %d\n",getINTEGER(getDLL(p,4)));
    freeDLL(p);
    return 0;
    }
