/*
 * sched.S - Funcions en ensablador de scheduler
 */

#include<asm.h>

ENTRY(task_switch)   //ebx -> union task_union *new
	// dynamic link
	pushl %ebp
	movl %esp, %ebp
	
	// guardar registres
	pushl %esi
	pushl %edi
	pushl %ebp
	
	// crida inner task switch
	push %ebx
	call inner_task_switch
	popl %ebx
	
	//current()->kernel_esp = ebp
	// @task_struct = esp & 0xFFFFF000
	movl %esp, %ecx
	andl %ecx, 0xFFFFF000
	
	// kernel_esp <- ebp
 	movl %ebp, 4(%ecx)
	
	// esp = new->kernel_esp
	movl 4(%ebx), %edx 
	movl %edx, %esp
	
	// restore registers
	popl %ebp
	popl %edi
	popl %esi
	
	ret
	
	
ENTRY(get_ebp)
	//dynamic link
	pushl %ebp
	movl %esp, %ebp
	
	// eax <- ebp
	movl %ebp, %eax
		
	// undo dynamic link
	popl %ebp
	ret
