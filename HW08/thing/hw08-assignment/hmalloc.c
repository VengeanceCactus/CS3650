
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>

#include "hmalloc.h"

/*
  typedef struct hm_stats {
  long pages_mapped;
  long pages_unmapped;
  long chunks_allocated;
  long chunks_freed;
  long free_length;
  } hm_stats;
*/

const size_t PAGE_SIZE = 4096;
static hm_stats stats; // This initializes the stats to 0.

long
free_list_length()
{
    // TODO: Calculate the length of the free list.
    return 0;
}

hm_stats*
hgetstats()
{
    stats.free_length = free_list_length();
    return &stats;
}

void
hprintstats()
{
    stats.free_length = free_list_length();
    fprintf(stderr, "\n== husky malloc stats ==\n");
    fprintf(stderr, "Mapped:   %ld\n", stats.pages_mapped);
    fprintf(stderr, "Unmapped: %ld\n", stats.pages_unmapped);
    fprintf(stderr, "Allocs:   %ld\n", stats.chunks_allocated);
    fprintf(stderr, "Frees:    %ld\n", stats.chunks_freed);
    fprintf(stderr, "Freelen:  %ld\n", stats.free_length);
}

static
size_t
div_up(size_t xx, size_t yy)
{
    // This is useful to calculate # of pages
    // for large allocations.
    size_t zz = xx / yy;

    if (zz * yy == xx) {
        return zz;
    }
    else {
        return zz + 1;
    }
}

void*
hmalloc(size_t size)
{
    stats.chunks_allocated += 1;
    size += sizeof(size_t);

    // TODO: Actually allocate memory with mmap and a free list.
    return (void*) 0xDEADBEEF;
}

void
hfree(void* item)
{
    stats.chunks_freed += 1;

    // TODO: Actually free the item.
}

