#include <libc.h>

char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */


	int s = set_sched_policy(1);


	/*
	 * PROVA DE CPU (modo user)
	 */
	
	/*int pids[3];
	int p, i;
	pids[0] = getpid();

	for (i = 1; i < 3; ++i) {
		p = fork();
		switch(p) {
			case 0:
				pids[1] = getpid();
				
				if (i == 1) for (int j = 0; j < 100000000; ++j);
				else for (int j = 0; j < 80000000; ++j);
				break;
				
			case -1:
				write(1, "error", strlen("error"));
				write(1, "\n", strlen("\n"));
				exit(0);
			
			default:
				for (int j = 0; j < 50000000; ++j);	
				break;
		}
	} 
	
	for (i = 0; i < 3; ++i) {
		int pid = pids[i];
		write(1, "PID: ", strlen("PID: "));
		char pc[128];
		itoa(pid, pc);
		write(1, pc, strlen(pc));
		write(1, "\n", strlen("\n"));		
		struct stats stp;
		int statsp = get_stats(pid, &stp);
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
		write(1, "ready: ", strlen("ready: "));
		char readyp[128];
		itoa(stp.ready_ticks, readyp);
		write(1, readyp, strlen(readyp));
		write(1, "\n", strlen("\n"));
	}*/

	
	/*
	 * PROVA DE I/O
	 */
	/*
	int pids[3];
	int p, i, r;
	int espera_hijo = 1000;
	int espera_padre = 3000;
	char c[128];
	pids[0] = getpid();

	for (i = 1; i < 3; ++i) {
		p = fork();
		switch(p) {
			case 0:
				pids[1] = getpid();
				
				if (i == 1) r = read(0, c, 2*espera_hijo);
				else r = read(0, c, espera_hijo);
				
				break;
				
			case -1:
				write(1, "error", strlen("error"));
				write(1, "\n", strlen("\n"));
				exit(0);
			
			default:
				r = read(0, c, espera_padre);
				break;
		}
	} 
	
	for (i = 0; i < 3; ++i) {
		int pid = pids[i];
		write(1, "PID: ", strlen("PID: "));
		char pc[128];
		itoa(pid, pc);
		write(1, pc, strlen(pc));
		write(1, "\n", strlen("\n"));		
		struct stats stp;
		int statsp = get_stats(pid, &stp);
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
		write(1, "ready: ", strlen("ready: "));
		char readyp[128];
		itoa(stp.ready_ticks, readyp);
		write(1, readyp, strlen(readyp));
		write(1, "\n", strlen("\n"));
	}
	*/
	
	/*
	 * PROVA DE CPU + I/O
	 */
	 
	int pids[3];
	int p, i, r;
	int espera_hijo = 1000;
	int espera_padre = 3000;
	char c[128];
	pids[0] = getpid();

	for (i = 1; i < 3; ++i) {
		p = fork();
		switch(p) {
			case 0:
				pids[1] = getpid();
				
				for (int i = 0; i < 9000; ++i);
				
				if (i == 1) r = read(0, c, 2*espera_hijo);
				else r = read(0, c, espera_hijo);
				
				for (int i = 0; i < 10000; ++i);
				
				write(1, "PID: ", strlen("PID: "));
				char pch[128];
				itoa(getpid(), pch);
				write(1, pch, strlen(pch));
				write(1, "\n", strlen("\n"));
				struct stats stph;
				int statsp = get_stats(getpid(), &stph);
				write(1, "user: ", strlen("user: "));
				char uph[128];
				itoa(stph.user_ticks, uph);
				write(1, uph, strlen(uph));
				write(1, "\n", strlen("\n"));
				write(1, "system: ", strlen("system: "));
				char sysph[128];
				itoa(stph.system_ticks, sysph);
				write(1, sysph, strlen(sysph));
				write(1, "\n", strlen("\n"));
				write(1, "blocked: ", strlen("blocked: "));
				char blockph[128];
				itoa(stph.blocked_ticks, blockph);
				write(1, blockph, strlen(blockph));
				write(1, "\n", strlen("\n"));
				write(1, "ready: ", strlen("ready: "));
				char readyph[128];
				itoa(stph.ready_ticks, readyph);
				write(1, readyph, strlen(readyph));
				write(1, "\n", strlen("\n"));
				break;
				
			case -1:
				write(1, "error", strlen("error"));
				write(1, "\n", strlen("\n"));
				exit(0);
			
			default:
				for (int i = 0; i < 30000; ++i);
				r = read(0, c, espera_padre);
				for (int i = 0; i < 9000; ++i);
				break;
		}
	} 
	
	for (i = 0; i < 3; ++i) {
		int pid = pids[i];
		write(1, "PID: ", strlen("PID: "));
		char pc[128];
		itoa(pid, pc);
		write(1, pc, strlen(pc));
		write(1, "\n", strlen("\n"));		
		struct stats stp;
		int statsp = get_stats(pid, &stp);
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
		write(1, "ready: ", strlen("ready: "));
		char readyp[128];
		itoa(stp.ready_ticks, readyp);
		write(1, readyp, strlen(readyp));
		write(1, "\n", strlen("\n"));
	}
	 

	while(1) { }
}
