/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>
#include <errno.h>
#include <system.h>

#define LECTURA 0
#define ESCRIPTURA 1
#define MIDA_BUFF 2048

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}

int sys_write(int fd, char *buffer, int size) {
	//printc_xy(69, 13, 's');
	//printc_xy(70, 13, 'j');
	//fd = 1;
	if ((check_fd(fd, ESCRIPTURA)) != 0) return -EACCES;
	if (size < 0) return -EINVAL;
	if (buffer == NULL) return -EINVAL;
		
	int n = size;
	char buff [MIDA_BUFF];
	// Més gran que la mida del buffer del kernel
	if (n > MIDA_BUFF) {
		//bucle que vaig llegint x parts
		while (n > MIDA_BUFF) {
			copy_from_user(buffer, buff, MIDA_BUFF);
			int b = sys_write_console(buff, MIDA_BUFF);
			n -= b;
			buffer += b;
		}
		if (n > 0) {
			copy_from_user(buff, buffer, n);
			int b = sys_write_console(buff, n);
		}
		return size;
	}
	// Més petit que la mida del buffer del kernel
	else {
		copy_from_user(buffer, buff, size);
		sys_write_console(buff, size);
		return size;
	}
	//xq asi retorne el codi de l'error al wrapper, i alla el tractem?
	//return a;
}

int sys_gettime() {
	return zeos_ticks;
} 
