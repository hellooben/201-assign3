#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "real.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1006);
    DLL *p = newDLL(displayREAL,freeREAL);
    int i;
    for (i = 0; i < 18; ++i)
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
    printf("value at 3 is %.2lf\n",getREAL(getDLL(p,3)));
    printf("setting value at 3 to 85.68\n");
    freeREAL(setDLL(p,3,newREAL(85.68)));
    printf("value at 3 now is %.2lf\n",getREAL(getDLL(p,3)));
    freeDLL(p);
    return 0;
    }
