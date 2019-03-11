
#include <stdio.h>
#include <stdlib.h>

void
swap(long* xs, long ii, long jj)
{
    if (ii != jj) {
        long tmp = xs[ii];
        xs[ii] = xs[jj];
        xs[jj] = tmp;
    }
}

long
partition(long* xs, long lo, long hi)
{
    long pivot = xs[hi - 1];

    long ii = lo;
    long jj = lo;
    for (; jj < (hi - 1); ++jj) {
        if (xs[jj] < pivot) {
            swap(xs, ii, jj);
            ++ii;
        }
    }
    swap(xs, ii, hi - 1);
    return ii;
}

void
quicksort(long* xs, long lo, long hi)
{
    if (hi - lo < 1) {
        return;
    }

    long pp = partition(xs, lo, hi);
    quicksort(xs, lo, pp);
    quicksort(xs, pp + 1, hi);
}

void
read_int(long* ptr)
{
    if (scanf("%ld", ptr) != 1) {
        puts("bad input");
        exit(1);
    }
}

int
main(int _argc, char* _argv[])
{
    long nn;
    read_int(&nn);

    long* xs = malloc(nn * sizeof(long));

    for (long ii = 0; ii < nn; ++ii) {
        read_int(&(xs[ii]));
    }

    quicksort(xs, 0, nn);

    printf("%ld\n", nn);
    for (long ii = 0; ii < nn; ++ii) {
        printf("%ld ", xs[ii]);
    }
    printf("\n");

    free(xs);
    return 0;
}


