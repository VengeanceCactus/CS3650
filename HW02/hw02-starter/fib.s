#Written by Andrew Ricci

.global main
.text
main:
    
    mov %rdi, %rax
    cmp %rax, $0
    je fib_zero

    cmp %rax, $1
    je fib_one
    jg fib_sq

    fib_zero:
    mov $0, $rsi
    jmp done

    fib_one:
    mov $1, rsi
    jmp done

    fib_sq:
    #recursive function

    
    done:
    #Borrowed from Nat's "Print_Int" function
    mov $format, %rdi
    mov $0, %al
    call printf

    leave
    ret
.data
format: .string "%d\n"

