
#include <stdio.h>
#include <string.h>

#include "hmalloc.h"
#include "tools.h"

int
main(int _ac, char* _av[])
{
    icell* xs = 0;
    icell* ys = 0;

    for (int ii = 0; ii < 100; ++ii) {
        xs = icons(ii, xs);
        ys = icons(ii, ys);
    }

    int sum = 0;

    for (icell* pp = xs; pp != 0; pp = pp->next) {
        sum += pp->data;
    }

    for (icell* pp = ys; pp != 0; pp = pp->next) {
        sum += pp->data;
    }

    printf("Sum = %d\n", sum);

    free_icell(xs);
    free_icell(ys);

    hprintstats();
    return 0;
}



