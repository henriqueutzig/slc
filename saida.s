    .globl main
    .type main, @function
main:
    pushq %rbp
    movq %rsp, %rbp
    movl $234, %ebx
    movl %ebx, -4(%rbp)
    movl $456, %ecx
    movl %ecx, -12(%rbp)
    movl $98, %edx
    movl %edx, -16(%rbp)
    movl $1, %esi
    movl %esi, -8(%rbp)
    movl -8(%rbp), %r8d
    movl $0, %edi
    cmpl %edi, %r8d
    setg %al
    movzbl %al, %r9d
    movl $0, %r15d
    cmpl %r15d, %r9d
    sete %al
    movzbl %al, %r14d
    testl %r14d, %r14d
    je .L0
    jmp .L2
.L2:
    movl $123, %r10d
    movl %r10d, -16(%rbp)
    movl -4(%rbp), %r11d
    movl -16(%rbp), %r12d
    movl %r11d, %eax
    addl %r12d, %eax
    movl %eax, %r13d
    movl %r13d, -12(%rbp)
    jmp .L1
.L0:
    # No operation
.L1:
    # No operation
    movl $0, %r17d
    movl %r17d, %eax
    leave
    ret
