/*
 * wrappers.S - Wrappers de les funcions de sistema
 */

#include <asm.h>
#include <segment.h>
#include <errno.h>


ENTRY(write)

	//dynamic link
	pushl %ebp
	movl %esp, %ebp
	
	//paso de parámetros
	movl 8(%ebp), %ebx	
	movl 12(%ebp), %ecx 
	movl 16(%ebp), %edx
	movl $0x04, %eax
	
	//TRAP
	int $0x80
	
	//control errores
	cmpl $0x0, %eax
	jge fin_write
	
	neg %eax 
	movl %eax, errno
	movl $0xffffffff, %eax 
fin_write:
	popl %ebp
	ret
	

ENTRY(gettime)

	//dynamic link
	pushl %ebp
	movl %esp, %ebp
	
	movl $10, %eax
	
	//TRAP
	int $0x80
	
	//control errores
	cmpl $0x0, %eax
	jge fin_gettime
	
	neg %eax 
	movl %eax, errno
	movl $0xffffffff, %eax 
fin_gettime:
	popl %ebp
	ret
	

