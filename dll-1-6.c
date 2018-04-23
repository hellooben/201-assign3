#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "real.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(1007);
    DLL *p = newDLL(displayREAL,freeREAL);
    /* do some inserts */
    int i;
    for (i = 0; i < 36; ++i)
        {
        int j = random() % 100;
        int k = random() % (sizeDLL(p) + 1);
        j += (random() % 100) / 100.0;
        insertDLL(p,k,newREAL(j));
        }
    /* now random inserts and removes */
    for (i = 0; i < 36; ++i)
        {
        if (random() % 2 == 0)
            {
            int x = random() % (sizeDLL(p) + 1);
            double j = random() % 1000 / 10.0;
            insertDLL(p,x,newREAL(j));
            }
        else if (sizeDLL(p) > 0)
            {
            int x = random() % sizeDLL(p);
            freeREAL(removeDLL(p,x));
            }
        }
    if (sizeDLL(p) < 100)
        {
        displayDLL(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeDLL(p));
    printf("value at 12 is %.2lf\n",getREAL(getDLL(p,12)));
    printf("setting value at 12 to 26.56\n");
    freeREAL(setDLL(p,12,newREAL(26.56)));
    printf("value at 12 now is %.2lf\n",getREAL(getDLL(p,12)));
    freeDLL(p);
    return 0;
    }
