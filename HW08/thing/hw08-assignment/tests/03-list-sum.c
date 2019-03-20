
#include <stdio.h>
#include <string.h>

#include "hmalloc.h"
#include "tools.h"

int
main(int _ac, char* _av[])
{
    icell* xs = 0;

    for (int ii = 0; ii < 10; ++ii) {
        xs = icons(ii, xs);
    }

    int sum = 0;

    for (icell* pp = xs; pp != 0; pp = pp->next) {
        sum += pp->data;
    }

    printf("Sum = %d\n", sum);

    free_icell(xs);

    hprintstats();
    return 0;
}



