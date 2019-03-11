.global cube
.text
cube:
    #push %r12
    #push %r13
    enter $0, $0
    #Copies parameter input to print output and alu source
    mov %rdi, %rax
    mov %rsi, %r10
    #mov %rd, %r14

    #Cubing function
    add %r10, %rax
    mov $0, al    
    #Borrowed from Nat's "Print_Int" function
    #mov %rax, %rsi
    #mov $format, %rdi
    #mov $0, %al
    #call printf

    leave
    #pop %r12
    #pop %r13
    ret
.data
format: .string "%d\n"
