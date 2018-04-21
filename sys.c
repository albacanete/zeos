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

int pid = 1000;

//unsigned long *get_ebp();

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

int ret_from_fork() {
	return 0;
}

int sys_fork()
{
	int PID=-1;

	// creates the child process
	if(list_empty(&freequeue)) {
		return -ENOMEM;
	}
	else {
		// Buscar PCB lliure
		struct list_head *new_lh = list_first(&freequeue);
		list_del(new_lh); 
		
		// Copiar context execució (task_union pare a fill)
		struct task_struct *new_ts = list_head_to_task_struct(new_lh);
		copy_data((union task_union*) current(), (union task_union*) new_ts, 4096);
		
		// Inicialitar el camp dir_pages_baseAddr
		allocate_DIR(new_ts);		
		/*
		// Buscar pàgines físiques lliures
		int i;
		for (i = 0; i < NUM_PAG_DATA; ++i) {
			if (alloc_frame() == -1) // TRACTAR ERROR -> NO FREE FRAMES
		}
		*/
		/* Crear nou espai d'adreçes
		 * 		codi i dades de sistema i codi d'usuari -> compartit amb pare
		 * 		dades i pila d'usuari -> propis
		 */
		page_table_entry *TP_new = get_PT(new_ts);
		page_table_entry *TP_current = get_PT(current());
		// codi i dades sistema i codi usuari
		copy_data(TP_current, TP_new, 255+NUM_PAG_CODE);
		// dades i pila usuari
		int i;
		// Assignar memoria
		for (i = 0; i < NUM_PAG_DATA; ++i) {
			int frame = alloc_frame();
			if (frame == -1) return -ENOMEM;
			set_ss_pag(TP_new, NUM_PAG_KERNEL+NUM_PAG_CODE+i, frame);
		}
		
		int valid_entries = NUM_PAG_KERNEL + NUM_PAG_CODE + NUM_PAG_DATA;
		// Copiar contingut pare a fill
		for (i = 0; i < NUM_PAG_DATA; ++i) {
			// Es mapejen les @ físiques que acabem d'allocatar en la TP del pare
			// perquè poder copiar el user data+stack al fill
			set_ss_pag(TP_current, valid_entries + i, get_frame(TP_new, NUM_PAG_KERNEL + NUM_PAG_CODE + i));
			// Copiar dades
			copy_data((void*)(NUM_PAG_KERNEL + NUM_PAG_CODE + i), (void*)(valid_entries + i), PAGE_SIZE);
			// Eliminar entrada perquè pare no pugui accedir a mem. fill
			del_ss_pag(TP_current, valid_entries + i);
		}
		set_cr3(TP_current);

		// Assignar PID
		PID = pid;
		++pid;
		new_ts->PID = PID;
		
		//unsigned long ebp = (unsigned long) get_ebp();
		
		//new_ts->kernel_esp = ebp;
		//ebp = &ret_from_fork;		
				
				
		// Afegir a la readyqueue
		list_add_tail(&new_lh, &readyqueue);
	}
	
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
