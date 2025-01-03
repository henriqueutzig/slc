    .globl main
    .type main, @function
main:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %ebx
    movl $4, %ecx
    movl $2, %esi
    movl %ecx, %eax
    cltd
    idivl %esi
    movl %eax, %edi
    movl %ebx, %eax
    addl %edi, %eax
    movl %eax, %r8d
    movl %r8d, %eax
    leave
    ret
