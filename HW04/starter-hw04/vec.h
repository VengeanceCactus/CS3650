// CS3650 HW04 Starter Code
// Author: Nat Tuck

#ifndef VEC_H
#define VEC_H

typedef struct vec {
    long* data;
    long size;
    long cap;
} vec;

vec* make_vec();
void free_vec(vec* xs);
void push_vec(vec* xs, long xx);

#endif
