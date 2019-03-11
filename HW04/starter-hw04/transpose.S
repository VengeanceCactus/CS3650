.global main

.data
five: .string "5 \n"

.text

main:
  enter $0, $0

  mov $1, %rdi
  mov $five, %rsi
  mov $2, %rdx
  mov $1, %rax
  syscall

  mov $0, %rax
  leave
  ret
