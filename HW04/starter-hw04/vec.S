// CS3650 Starter Code
// Author: Nat Tuck

.text

  /*
typedef struct vec {
    long* data;
    long size;
    long cap;
} vec;
  */

//vec* make_vec();
.global make_vec
make_vec:
  // first use %rbp instead of rbx
  push %rbx // ys
  enter $8, $0
  mov $24, %rdi
  call malloc
  mov %rax, %rbx
  mov $16, %rdi
  call malloc
  mov %rax, 0(%rbx) // ys->data = malloc(2 slots);
  movq $0, 8(%rbx) // ys->size = 0;
  movq $2, 16(%rbx) // ys->cap = 2;
  mov %rbx, %rax
  leave
  pop %rbx
  ret

//void free_vec(vec* xs);
.global free_vec
free_vec:
  enter $16, $0
  mov %rdi, 0(%rsp) // xs
  mov 0(%rdi), %rdi
  call free
  mov 0(%rsp), %rdi
  call free
  leave
  ret

//void push_vec(vec* xs, long xx);
.global push_vec
push_vec:
  push %r12 // xs
  push %r13 // xx
  enter $0, $0
  // %r10, %r11 as temporaries

  mov %rdi, %r12
  mov %rsi, %r13

  mov 8(%r12), %r10 // size
  cmp %r10, 16(%r12) // cap > size?
  jg push_no_resize

  add %r10, %r10 // 2 * size
  mov %r10, 16(%r12) // cap = %r10
  mov $0, %rsi
  lea (%rsi,%r10,8), %rsi // 8 * %r10
  mov 0(%r12), %rdi // start with "mov %r12, %rdi"
before_realloc:
  call realloc
  mov %rax, 0(%r12)

push_no_resize:
  mov 0(%r12), %r10 // data
  mov 8(%r12), %r11 // size
  mov %r13, (%r10,%r11,8) // xs->data[xs->size] = xx
  incq 8(%r12) // size++

push_done:
  leave
  pop %r13
  pop %r12
  ret
