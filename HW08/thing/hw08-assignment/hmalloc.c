
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

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

typedef struct free_cell {
	size_t size;
	struct free_cell* next;
} free_cell;

const size_t PAGE_SIZE = 4096;
static hm_stats stats; // This initializes the stats to 0.
free_cell* free_list = NULL;
pthread_mutex_t free_list_lock;
int free_list_lock_initialized = 0;

long
free_list_length()
{
    	free_cell* nxt = free_list;
	int count = 0;
	while(nxt != NULL) {
		count++;
		nxt = nxt->next;
	}
 	return count;
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
removeMemFromFreeList(size_t size)
{
	if (free_list == NULL) {
		return 0;
	}
	free_cell* prev = NULL;
	free_cell* head = free_list;
	void* cell = NULL;
	while(head != NULL) {
		if (head->size >= size) {
			if (prev == NULL) {
				free_list = head->next;
			} else if (head->next == NULL) {
				prev->next = NULL;
			} else {
				prev->next = head->next;
			}
			head->next = NULL;
			cell = head;
			return cell;
		} else {
			prev = head;
			head = head->next;
		}
	}
	return cell;
}

void
organize_free_list()
{
	free_cell* x, *y, *e;

        x = free_list;
        free_list = NULL;

        while(x != NULL) {
                e = x;
                x = x->next;
                if (free_list != NULL) {
                        if (e > free_list) {
                                y = free_list;
                                while ((y->next != NULL) && (e > y->next)) {
                                        y = y->next;
                                }
                                e->next = y->next;
                                y->next = e;
                        } else {
                                e->next = free_list;
                                free_list = e;
                        }
                } else {
                        e->next = NULL;
                        free_list = e;
                }
        }

	free_cell* head = free_list;
	size_t size = head->size;
	free_cell* next = head->next;
	long length = free_list_length();
	while(next != NULL) {
		size_t nextSize = next->size;
		char* cHead = (char*)head;
		char* cNext = (char*)next;
		int differenceInBytes = (cNext - cHead);
		if (cHead + size == cNext) {
			head->size += next->size;
			head->next = next->next;
		} else {
			head = head->next;
		}
		size = head->size;
		next = head->next;
	}
}

void*
hmalloc(size_t size)
{
	if (!free_list_lock_initialized) {
		pthread_mutex_init(&free_list_lock, 0);
		free_list_lock_initialized = 1;
	}
    	stats.chunks_allocated += 1;
    	size += sizeof(size_t);
	if (size < PAGE_SIZE) {
		pthread_mutex_lock(&free_list_lock);
		// Try to remove a *size* amount of memory from the free_list
		void* cell = NULL;
		int memLeftover;
		if (free_list != NULL) {
			cell = removeMemFromFreeList(size);
		}
		if (cell != NULL) { // Enough memory on free_list
			size_t* cellSize = cell;
			memLeftover = *cellSize - size;
		} else { // Not enough memory on free_list
			// Allocate a page of memory
			cell = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE,
					 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
			stats.pages_mapped += 1;
			memLeftover = PAGE_SIZE - size;
		}
		// Add any extra memory back onto the free_list
		if (memLeftover >= sizeof(free_cell)) {
			char* cCell = cell;
			cCell += size;
			free_cell* fCell = (free_cell*)cCell;
			fCell->size = memLeftover;
			fCell->next = NULL;
			if (free_list != NULL) {
				free_cell* head = free_list;
				while(head->next != NULL) {
					head = head->next;
				}
				head->next = fCell;
			} else {
				free_list = fCell;
			}
		}
		pthread_mutex_unlock(&free_list_lock);
		// Store the new size at the beginning of the memory
		size_t* sizePtr = cell;
		*sizePtr = size;
		sizePtr++;
		void* start = sizePtr;
		return start;
	} else {
		// Calculate number of pages needed for this block
		int numOfPages = div_up(size, PAGE_SIZE);
		stats.pages_mapped += numOfPages;
		// Allocate that many pages with mmap
		int numOfMem = numOfPages * PAGE_SIZE;
		void* cell = mmap(0, numOfMem, PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		// Fill in the size of the block as (# of pages * 4096)
		size_t* sizePtr = cell;
		*sizePtr = numOfMem;
		sizePtr++;
		// Return a pointer to the block *after* the size field
		void* start = sizePtr;
		return start;
	}
}

void
hfree(void* item)
{
    	stats.chunks_freed += 1;
	void* start = item - sizeof(size_t);
    	size_t* size = start;
	free_cell* freedCell = (free_cell*)start;
	if (*size < PAGE_SIZE) {
		pthread_mutex_lock(&free_list_lock);
		free_cell* head = free_list;
		free_cell* next = head->next;
		while(next != NULL) {
			head = next;
			next = next->next;
		}
		head->next = freedCell;
		head->next->size = *size;
		head->next->next = NULL;

                organize_free_list();
		pthread_mutex_unlock(&free_list_lock);
	} else {
		int numOfPages = div_up(*size, PAGE_SIZE);
		stats.pages_unmapped += numOfPages;
		munmap(item, *size);
	}
}

void* 
hrealloc(void* prev, size_t bytes)
{
	if (prev == NULL) {
		return hmalloc(bytes);
	}
	void* start = prev - sizeof(size_t);
	size_t* size = start;
	if (*size > bytes) {
		int memLeftover = bytes - *size;
		if (memLeftover >= sizeof(free_cell)) {
			pthread_mutex_lock(&free_list_lock);
			start += *size;
			free_cell* fCell = (free_cell*)start;
			fCell->size = memLeftover;
			fCell->next = NULL;
			if (free_list != NULL) {
				free_cell* head = free_list;
				while(head->next != NULL) {
					head = head->next;
				}
				head->next = fCell;
			} else {
				free_list = fCell;
			}
			pthread_mutex_unlock(&free_list_lock);
			*size = bytes;
			return prev;
		}
	} else if (*size == bytes) {
		return prev;
	} else {
		void* start = prev - sizeof(size_t);
		size_t* size = start;
		// hmalloc new memory
		void* newMem = hmalloc(bytes);
		// copy over data to new memory
		memcpy(newMem, prev, *size);
		// hfree old memory
		hfree(prev);
		return newMem;
	}
	return 0;
}

