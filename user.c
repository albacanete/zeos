#include <libc.h>

char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
    
    char tiempo [128];
    
    	
	char *c = "\nhola profe :)\n";
	write(1, c, strlen(c));
    
	int t = gettime();
	if (t < 0) perror();
	else {
		itoa(t, tiempo);
		write(1, tiempo, strlen(tiempo));
	}
	
	int pid = getpid();
	char p[128];
	itoa(pid, p);
	write(1, p, strlen(p));
	int f = fork();
				
  while(1) { }
}
