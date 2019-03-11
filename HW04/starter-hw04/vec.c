// CS3650 HW04 Starter Code
// Author: Nat Tuck

#include <stdlib.h>

#include "vec.h"

/*
typedef struct vec {
    long* data;
    long size;
    long cap;
} vec;
*/

vec*
make_vec()
{
    vec* xs = malloc(sizeof(vec));
    xs->data = malloc(2 * sizeof(long));
    xs->size = 0;
    xs->cap  = 2;
    return xs;
}

void
free_vec(vec* xs)
{
    free(xs->data);
    free(xs);
}

void
push_vec(vec* xs, long xx)
{
    if (xs->size >= xs->cap) {
        xs->cap *= 2;
        xs->data = realloc(xs->data, xs->cap * sizeof(long));
    }

    xs->data[xs->size] = xx;
    xs->size += 1;
}

