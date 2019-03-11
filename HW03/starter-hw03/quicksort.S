#Written by Andrew Ricci

.global main
.data
bad_input: .string "bad input"
format: .string "%ld"
out_format: .string "%ld "
out_format2: .string "%ld \n"

.text
main:
  enter $16, $0
  lea 0(%rsp), %rdi
  call read_int

  mov 0(%rsp), %rdi
  imul $8, %rdi
  #extern malloc
  call malloc
  mov %rax, %r14

  #ii = r13, nn = 0(%rsp), xs = r14

  mov 0(%rsp), %r12
  mov $0, %r13
  read_for:
  
  cmp %r12, %r13
  je read_for_done
  
  lea (%r14, %r13, 8), %rdi 
  call read_int  

  inc %r13
  jmp read_for

  read_for_done: 

  mov %r12, %rsi
  mov $out_format2, %rdi
  mov $0, %al
  call printf

  mov $0, %r13
  call quicksort 
  #call partition

  mov $0, %r13
  print_loop:

  cmp %r12, %r13
  je print_loop_done

  mov (%r14, %r13, 8), %rsi
  mov $out_format, %rdi
  mov $0, %al
  call printf

  inc %r13
  jmp print_loop

  print_loop_done:


  #bad:
    #mov $bad_input, %rdi
    #call puts

  #end:
  leave
  ret

swap:
  #xs = %r14, ii = %r13, jj = %rbx
  enter $0, $0

  cmp %r13, %rbx
  je endswap

  mov (%r14, %r13, 8), %r8
  mov (%r14, %rbx, 8), %r9
  mov %r9, (%r14, %r13, 8)
  mov %r8, (%r14, %rbx, 8)
  
  endswap:
    leave
    ret

partition:
  #xs = %r14, lo = %r13, hi = %r12
  enter $0, $0
  #pivot = %r15, ii = %r13, jj = %rbx

  mov %r12, %rax
  sub $1, %rax
  mov (%r14, %rax, 8), %r15

  mov %r13, %rbx
  mov %r12, %r9
  sub $1, %r9
  #hi = %rbp
 
  part_loop:

  cmp %rbx, %r9
  je part_loop_end

  cmp (%r14, %rbx , 8), %r15
  je part_loop_point
  call swap
  inc %r13

  part_loop_point:
  inc %rbx
  jmp part_loop  

  part_loop_end:

  mov %r12, %r8
  sub $1, %r8
  mov %r8, %rbx
  call swap

  mov %r13, %r15
  leave
  ret

quicksort:
  #xs = %r14, lo = %r13, hi = %r12
  enter $0, $0

  mov %r12, %r8

  sub %r13, %r8
  cmp $1, %r8

  jl qsdone
  
  #pp = %r15
  call partition
  
  push %r12
  mov %r15, %r12
  call quicksort
 
  pop %r12
  push %r13

  inc %r15
  mov %r15, %r13
  call quicksort
  pop %r13

  qsdone:
    leave
    ret

read_int:
  #
  enter $0, $0

  mov %rdi, %rsi
  mov $format, %rdi
  mov $0, %al
  call scanf
  #stored at &nn, %rax
  cmp $1, %rax
  je read_done

  mov $bad_input, %rdi
  call puts

  mov $1, %rdi
  call exit

  read_done:
    leave
    ret
