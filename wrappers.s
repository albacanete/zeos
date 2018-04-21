# 1 "wrappers.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "wrappers.S"




# 1 "include/asm.h" 1
# 6 "wrappers.S" 2
# 1 "include/segment.h" 1
# 7 "wrappers.S" 2
# 1 "include/errno.h" 1
# 8 "wrappers.S" 2


.globl write; .type write, @function; .align 0; write:


 pushl %ebp
 movl %esp, %ebp


 movl 8(%ebp), %ebx
 movl 12(%ebp), %ecx
 movl 16(%ebp), %edx
 movl $0x04, %eax


 int $0x80


 cmpl $0x0, %eax
 jge fin_write

 neg %eax
 movl %eax, errno
 movl $0xffffffff, %eax
fin_write:
 movl %ebp, %esp
 popl %ebp
 ret


.globl gettime; .type gettime, @function; .align 0; gettime:


 pushl %ebp
 movl %esp, %ebp

 movl $10, %eax


 int $0x80


 cmpl $0x0, %eax
 jge fin_gettime

 neg %eax
 movl %eax, errno
 movl $0xffffffff, %eax
fin_gettime:
 movl %ebp, %esp
 popl %ebp
 ret


.globl getpid; .type getpid, @function; .align 0; getpid:


 pushl %ebp
 movl %esp, %ebp

 movl $20, %eax


 int $0x80


 cmpl $0x0, %eax
 jge fin_getpid

 neg %eax
 movl %eax, errno
 movl $0xffffffff, %eax
fin_getpid:
 movl %ebp, %esp
 popl %ebp
 ret


.globl fork; .type fork, @function; .align 0; fork:


 pushl %ebp
 movl %esp, %ebp

 movl $2, %eax


 int $0x80


 cmpl $0x0, %eax
 jge fin_fork

 neg %eax
 movl %eax, errno
 movl $0xffffffff, %eax
fin_fork:
 movl %ebp, %esp
 popl %ebp
 ret
