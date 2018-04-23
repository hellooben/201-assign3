#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "real.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1012);
    DLL *p = newDLL(displayREAL,freeREAL);
    int i;
    for (i = 0; i < 10000; ++i)
        {
        int j = random() % 100;
        int k = random() % (sizeDLL(p) + 1);
        j += (random() % 100) / 100.0;
        insertDLL(p,k,newREAL(j));
        }
    if (sizeDLL(p) < 100)
        {
        displayDLL(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeDLL(p));
    printf("value at 3793 is %.2lf\n",getREAL(getDLL(p,3793)));
    printf("setting value at 3793 to 90.88\n");
    freeREAL(setDLL(p,3793,newREAL(90.88)));
    printf("value at 3793 now is %.2lf\n",getREAL(getDLL(p,3793)));
    freeDLL(p);
    return 0;
    }
