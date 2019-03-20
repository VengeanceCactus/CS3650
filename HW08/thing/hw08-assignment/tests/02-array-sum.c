
#include <stdio.h>
#include <string.h>

#include "hmalloc.h"
#include "tools.h"

int
main(int _ac, char* _av[])
{
    long* xs = hmalloc(1024 * sizeof(long));

    for (int ii = 0; ii < 1024; ++ii) {
        xs[ii] = ii;
    }

    int sum = 0;

    for (int ii = 0; ii < 1024; ++ii) {
        sum += xs[ii];
    }

    printf("Sum = %d\n", sum);

    hfree(xs);

    hprintstats();
    return 0;
}



