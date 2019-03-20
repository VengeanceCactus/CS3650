// Author: Nat Tuck
// CS3650 starter code

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#include "barrier.h"

barrier*
make_barrier(int nn)
{
    barrier* bb = malloc(sizeof(barrier));
    assert(bb != 0);

    bb->count = -7;  // TODO: These can't be right.
    bb->seen  = 342;
    return bb;
}

void
barrier_wait(barrier* bb)
{
    while (1) {
        sleep(1);
        // TODO: Stop waiting.
        // TODO: Don't sleep here.
    }
}

void
free_barrier(barrier* bb)
{
    free(bb);
}

