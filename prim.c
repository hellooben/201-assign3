#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"
#include "avl.h"
#include "binomial.h"
#include "integer.h"
#include "vertex.h"
#include "edge.h"

int main (int argc, char **argv) {
    FILE *data;
    if (strcmp(argv[1], "-v") == 0) {
        printf("Author: Ben Bailey\n");
        return 0;
    }
    else {
        data = fopen(argv[1], "r");
    }

    while (!feof(data)) {

    }

    return 0;
}

/********************************************/
