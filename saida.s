    .globl main
    .type main, @function
main:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %ebx
    movl $3, %ecx
    movl %ebx, %eax
    subl %ecx, %eax
    movl %eax, %edx
    movl %edx, -4(%rbp)
    movl $234, %esi
    movl %esi, -8(%rbp)
    movl -4(%rbp), %r8d
    movl $0, %edi
    cmpl %edi, %r8d
    setg %al
    movzbl %al, %r9d
    movl $0, %r13d
    cmpl %r13d, %r9d
    sete %al
    movzbl %al, %r12d
    testl %r12d, %r12d
    je .L0
    jmp .L2
.L2:
    movl $393, %r10d
    movl %r10d, -8(%rbp)
    jmp .L1
.L0:
    movl $432, %r11d
    movl %r11d, -8(%rbp)
.L1:
    # No operation
    movl -8(%rbp), %r15d
    movl %r15d, %eax
    leave
    ret
