#include <libc.h>

char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */


	int s = set_sched_policy(0);


	/*
	 * PROVA DE CPU (modo user)
	 */
	 
	int p, pidh, pidp;
	write(1, "PID PADRE: ", strlen("PID PADRE: "));
	pidp = getpid();
	char pp[128];
	itoa(pidp, pp);
	write(1, pp, strlen(pp));
	write(1, "\n", strlen("\n"));
	for (int i = 0; i < 2; ++i) {
		p = fork();
		switch(p) {
			case 0:
				write(1, "PID HIJO: ", strlen("PID HIJO: "));
				pidh = getpid();
				char ph[128];
				itoa(pidh, ph);
				write(1, ph, strlen(ph));
				write(1, "\n", strlen("\n"));
				for (int i = 0; i < 100000000; ++i);
				
				struct stats st_h;
				int statsh = get_stats(pidh, &st_h);
				write(1, "user: ", strlen("user: "));
				char uh[128];
				itoa(st_h.user_ticks, uh);
				write(1, uh, strlen(uh));
				write(1, "\n", strlen("\n"));
				write(1, "system: ", strlen("system: "));
				char sysh[128];
				itoa(st_h.system_ticks, sysh);
				write(1, sysh, strlen(sysh));
				write(1, "\n", strlen("\n"));
				write(1, "blocked: ", strlen("blocked: "));
				char blockh[128];
				itoa(st_h.blocked_ticks, blockh);
				write(1, blockh, strlen(blockh));
				write(1, "\n", strlen("\n"));
				write(1, "\n", strlen("\n"));
				exit(0);
			
			case -1:
				write(1, "error", strlen("error"));
				write(1, "\n", strlen("\n"));
				exit(0);
			
			default:
				for (int i = 0; i < 5000000; ++i);
				
				struct stats st;
				int stats = get_stats(pidh, &st);
				write(1, "user: ", strlen("user: "));
				char u[128];
				itoa(st.user_ticks, u);
				write(1, u, strlen(u));
				write(1, "\n", strlen("\n"));
				write(1, "system: ", strlen("system: "));
				char sys[128];
				itoa(st.system_ticks, sys);
				write(1, sys, strlen(sys));
				write(1, "\n", strlen("\n"));
				write(1, "blocked: ", strlen("blocked: "));
				char block[128];
				itoa(st.blocked_ticks, block);
				write(1, block, strlen(block));
				write(1, "\n", strlen("\n"));
				write(1, "\n", strlen("\n"));			
		}
	} 
	
	for (int i = 0; i < 9000000; ++i);
				
	struct stats stp;
	int statsp = get_stats(pidp, &stp);
	write(1, "user: ", strlen("user: "));
	char up[128];
	itoa(stp.user_ticks, up);
	write(1, up, strlen(up));
	write(1, "\n", strlen("\n"));
	write(1, "system: ", strlen("system: "));
	char sysp[128];
	itoa(stp.system_ticks, sysp);
	write(1, sysp, strlen(sysp));
	write(1, "\n", strlen("\n"));
	write(1, "blocked: ", strlen("blocked: "));
	char blockp[128];
	itoa(stp.blocked_ticks, blockp);
	write(1, blockp, strlen(blockp));
	write(1, "\n", strlen("\n"));
	write(1, "\n", strlen("\n"));

	
	/*
	 * PROVA DE I/O
	 */
	
	/*int i, rp, rh, p;
	int espera_hijo = 1000;
	int espera_padre = 3000;
	char c[128];
	for (i = 0; i < 3; ++i) {
		p = fork();
		switch(p) {
			case 0:
				rh = read(0, c, espera_hijo);
				itoa(i, c);
				write(1, c, strlen(c));
				break;
			
			case -1:
				write(1, "error", strlen("error"));
				write(1, "\n", strlen("\n"));
				exit(0);
			
			default:
				rp = read(0, c, espera_padre);
				itoa(i, c);
				write(1, c, strlen(c));
		}
		
	}
	
	write(1, "\n", strlen("\n"));
	p = getpid();
	struct stats st;
	int stats = get_stats(p, &st);
	write(1, "user: ", strlen("user: "));
	char u[128];
	itoa(st.user_ticks, u);
	write(1, u, strlen(u));
	write(1, "\n", strlen("\n"));
	write(1, "system: ", strlen("system: "));
	char sys[128];
	itoa(st.system_ticks, sys);
	write(1, sys, strlen(sys));
	write(1, "\n", strlen("\n"));
	write(1, "blocked: ", strlen("blocked: "));
	char block[128];
	itoa(st.blocked_ticks, block);
	write(1, block, strlen(block));
	write(1, "\n", strlen("\n"));*/

	
	/*
	 * PROVA DE CPU + I/O
	 */
	 
	/*int rp, rh, p;
	int espera_hijo = 1000;
	int espera_padre = 3000;
	int pidp;
	char c[128];
	for (int i = 0; i < 3; ++i) {
		p = fork();
		switch(p) {
			case 0:
				for (int i = 0; i < 10000; ++i);
				rh = read(0, c, espera_hijo);
				itoa(i, c);
				write(1, c, strlen(c));
				for (int i = 0; i < 10000; ++i);
				break;
			
			case -1:
				write(1, "error", strlen("error"));
				write(1, "\n", strlen("\n"));
				exit(0);
			
			default:
				for (int i = 0; i < 10000; ++i);
				rp = read(0, c, espera_padre);
				itoa(i, c);
				write(1, c, strlen(c));
				for (int i = 0; i < 10000; ++i);
		}
	} 
	
	pidp = getpid();
	if (pidp == 1) {
		write(1, "\n", strlen("\n"));
		struct stats st;
		int stats = get_stats(pidp, &st);
		write(1, "user: ", strlen("user: "));
		char u[128];
		itoa(st.user_ticks, u);
		write(1, u, strlen(u));
		write(1, "\n", strlen("\n"));
		write(1, "system: ", strlen("system: "));
		char sys[128];
		itoa(st.system_ticks, sys);
		write(1, sys, strlen(sys));
		write(1, "\n", strlen("\n"));
		write(1, "blocked: ", strlen("blocked: "));
		char block[128];
		itoa(st.blocked_ticks, block);
		write(1, block, strlen(block));
		write(1, "\n", strlen("\n"));
	}*/
	 
	
	while(1) { }
}
