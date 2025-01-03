    .globl main
    .type main, @function
main:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %ecx
    movl $0, %edi
    cmpl %edi, %ecx
    sete %al
    movzbl %al, %esi
    testl %esi, %esi
    je .L1
    jmp .L0
.L0:
    movl $1, %edx
    jmp .L2
.L1:
    movl $0, %edx
.L2:
    # No operation
    movl %edx, -4(%rbp)
    movl -4(%rbp), %r8d
    movl %r8d, %eax
    leave
    ret
