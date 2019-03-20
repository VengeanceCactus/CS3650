#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <pthread.h>

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

int lock_init = 0;

const size_t PAGE_SIZE = 4096;
static hm_stats stats; // This initializes the stats to 0.
struct header* free_list = NULL;

pthread_mutex_t list_lock;
//struct header *head = NULL, *tail = NULL;

/*struct header *find_free(size_t size){
    struct header *cur = head;
    while(cur){
	if(cur->free && cur->size >= size){
            return cur;
	}
    }
    return NULL;
}*/

long
free_list_length()
{
    struct header* next = free_list;
    int i = 0;
    while(next != NULL){
	i++;
	next = next->next;
    }
    return i;
    // TODO: Calculate the length of the free list.
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
take_from_free_list(size_t size){
    if(free_list == NULL){
	return 0;
    }
    struct header* prev = NULL;
    struct header* head = free_list;
    void* block = NULL;
    while(head != NULL){
	if(head->size >= size){
	    if(prev == NULL){
		free_list = head->next;
	    }
	    else if(head->next == NULL){
                 prev->next = NULL;
            }
	    else{
		prev->next = head->next;
	    }
	    head->next = NULL;
	    block = head;
	    return block;
	}
	else{
            prev = head;
	    head = head->next;
	}
    }
    return block;
}

void
organize_list()
{
    struct header* head = free_list;
    size_t total = head->size;
    struct header* next = head->next;
    while(next != NULL){
	char* h = (char*)head;
	char* n = (char*)next;
	if(h + total == n){
	    head->size += next->size;
	    head->next = next->next;
	}
	else{
	    head = head->next;
	}
        total = head->size;
	next = head->next;
    }
    struct header *a, *b, *c;
    a = free_list;
    free_list = NULL;
    while(a != NULL){
	c = a;
	a = a->next;
	if(free_list != NULL){
	    if(c > free_list){
		b = free_list;
		while((b->next != NULL) && (c > b->next)){
		    b = b->next;
		}
	        c->next = b->next;
		b->next = c;
	    }
	    else{
		c->next = free_list;
		free_list = c;
	    }
	}
	else{
            c->next = NULL;
	    free_list = c;
	}
    }
}
	
void*
hmalloc(size_t size)
{

    if(!lock_init){
	pthread_mutex_init(&list_lock, 0);
	lock_init = 1;
    }
    size_t total;
    total = sizeof(struct header) + size;
    stats.chunks_allocated += 1;

    if(size < PAGE_SIZE){
	pthread_mutex_lock(&list_lock);
	void * block = NULL;
	int remainder;
	
	if(free_list != NULL){
             block = take_from_free_list(total);
	}

	if(block != NULL){
	    size_t* block_size = block;
	    remainder = *block_size - total;
	}
        else{
	    block = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	    stats.pages_mapped += 1;
	    remainder = PAGE_SIZE - total;
	}

	if(remainder >= sizeof(struct header)){
	    char* c = block;
	    c += total;
	    struct header* f = (struct header*)c;
	    f->size = remainder;
	    f->next = NULL;
	    if(free_list != NULL){
	        struct header* head = free_list;
		while(head->next != NULL){
		    head = head->next;
		}
		head->next = f;
	    }
	    else{
	        free_list = f;
	    }
	}
        pthread_mutex_unlock(&list_lock);

	size_t* ptr = block;
	*ptr = total;
	ptr ++;
	void* start = ptr;
	return start;
    }
    else{	
	int total_pages = div_up(total, PAGE_SIZE);
        stats.pages_mapped += total_pages;
        int mem_total = total_pages * PAGE_SIZE;
	void* block = mmap(0, mem_total, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	size_t *ptr = block;
	*ptr = mem_total;
	ptr++;
	void* start = ptr;
	return start;
    }
}

void
hfree(void* item)
{
    stats.chunks_freed += 1;
    void* less = item - sizeof(size_t);
    size_t* total = less;

    //printf("Freed this time is %zu\n", *total);

    struct header* free_mem = (struct header*)less;
    //size_t* total = (size_t*)item;
    //programbreak = sbrk(0);
    if(*total < PAGE_SIZE){
	pthread_mutex_lock(&list_lock);
	struct header* head = free_list;
	struct header* next = head->next;
	while(next != NULL){
	    head = next;
	    next = next->next;
	}
	head->next = free_mem;
	head->next->size = *total;
	head->next->next = NULL;
	organize_list();
	pthread_mutex_unlock(&list_lock);
    }
    else{
	long total_pages = div_up(*total, PAGE_SIZE);
	stats.pages_unmapped += total_pages;
	//printf("Total pages unmapped: %ld\n", stats.pages_unmapped);
	mmap(0, 0 - free_mem->size - sizeof(struct header), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    }
    // TODO: Actually free the item.
}

void*
hrealloc(void* prev, size_t bytes)
{
    if(prev == NULL){
	return hmalloc(bytes);
    }
    void* less = prev - sizeof(size_t);
    size_t* total = less;
    if(*total > bytes){
	int mem_extra = bytes - *total;
	if(mem_extra >= sizeof(struct header)){
	    pthread_mutex_lock(&list_lock);
	    less += *total;
	    struct header* block = (struct header*)less;
	    block->size = mem_extra;
	    block->next = NULL;
	    if(free_list != NULL){
		struct header* head = free_list;
		while(head->next != NULL){
	            head = head->next;
		}
		head->next = block;
	    }
	    else{
		free_list = block;
	    }
	    pthread_mutex_unlock(&list_lock);
	    *total = bytes;
	    return prev;
	}
    }
    else if(*total == bytes){
	return prev;
    }
    else{
	void* less = prev - sizeof(struct header);
	size_t* total = less;
        void* new_mem = hmalloc(bytes);
	memcpy(new_mem, prev, *total);
	hfree(prev);
	return new_mem;
    }
    return 0;
}
