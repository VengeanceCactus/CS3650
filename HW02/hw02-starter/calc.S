#Written by Andrew Ricci

.global main
.text
main:
    enter $0, $0
    #mov %rdi, %rsi
    #mov %rdi, %r12
    #mov %rsi, %r13
    #mov %rdx, %r14

    #cmp %r13, 0x42
    #je mult
    #cmp %r13, 0x43
    #je add
    #cmp %r13, 0x45
    #je sub
    #cmp %r13, 0x47
    #je divide
    
    #add:
    #imul $10, %rsi
    #jmp done

    #sub:
    #sub %r14, %r12
    #jmp done

    #mult:
    #imul %r14, %r12
    #jmp done

    #divide:
    #imul %r14, %r12
    #jmp done

    #done:
    #Borrowed from Nat's "Print_Int" function
    #mov 8(%rsi), %rdx
    #add $10, %rdx
    mov -8(%rbp), %rsi
    mov $format, %rdi
    mov $0, %al
    call printf

    leave
    ret
.data
format: .string "%ld\n"
