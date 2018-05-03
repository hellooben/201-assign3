#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "real.h"

struct REAL
    {
    double value;
    };

REAL *
newREAL(double x)
    {
    REAL *p = malloc(sizeof(REAL));
    assert(p != 0);
    p->value = x;
    return p;
    }

double
getREAL(REAL *v)
    {
    return v->value;
    }

double
setREAL(REAL *v,double x)
    {
    double old = v->value;
    v->value = x;
    return old;
    }

void
displayREAL(void *v,FILE *fp)
    {
    fprintf(fp,"%f",getREAL((REAL *) v));
    }

int
compareREAL(void *v,void *w) {
    if (getREAL(v) > getREAL(w)) {return 1;}
    else if (getREAL(w) > getREAL(v)) {return -1;}
    else {return 0;}
}

int
rcompareREAL(void *v,void *w) {
    if (getREAL(v) > getREAL(w)) {return -1;}
    else if (getREAL(w) > getREAL(v)) {return 1;}
    else {return 0;}
}

void
freeREAL(void *v)
    {
        // printf("REAL FREEING: ");
        // displayREAL(v, stdout);
        // printf("\n");
    free((REAL *) v);
    }
