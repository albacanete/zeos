/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <list.h>

/**
 * Container for the Task array and 2 additional pages (the first and the last one)
 * to protect against out of bound accesses.
 */
union task_union protected_tasks[NR_TASKS+2]
  __attribute__((__section__(".data.task")));

union task_union *task = &protected_tasks[1]; /* == union task_union task[NR_TASKS] */

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;

struct list_head freequeue;
struct list_head readyqueue;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}


void init_idle (void)
{
	struct list_head *ilh = list_first(&freequeue);
	list_del(ilh); //l'eliminem, sino segueix a la freequeue
	struct task_struct *its = list_head_to_task_struct(&ilh);
	its.PID = 0;
	if (allocate_DIR(&its) != 1) {
		}//falta tratar error
		
	
	union taskunion *itu = (union task_union*)tu;
	task_switch(&tu);
	
}

void init_task1(void)
{
}


void init_sched(){
	INIT_LIST_HEAD(&freequeue);
	INIT_LIST_HEAD(&readyqueue);
	
	/* Add the new element to the list, using the element's anchor.   
	 * list_add( &(el.anchor), &list );
	 * void list_add(struct list_head *new, struct list_head *head)
	 */
	 int i =1;
	 union task_union *t = task;
	 while (i < NR_TASKS -1) {
		 list_add(&(t -> task.list), &freequeue);
		 ++i;
		 ++t;
	 }
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

void task_switch(union task_union*t) {
	
	__asm__ __volatile__(
		//tenemos que guardar esi edi ebx
	)
	//llamar a inner_task_switch. no entenc xq fer una funcio apart.
	//crec que es la que realment canvia el proces 
	
	__asm__ __volatile__(
		//aqui restauramos esi edi ebx
	)
}


void inner_task_switch(union task_union*t) {
	//1) update de tss
	
	//2) change user address space --> update the current page directory
	 struct task_struct *ts = (struct task_struct*)t;  //agafo el task_struct del nou proces a executar
	 page_table_entry* new_TD = ts.dir_pages_baseAddr; //agafo el punter a la seva taula de pagines
	 set_cr3(new_TD);
	 
	 //3) store value of ebp in the pcb wtf?
	 //4,5..
}

/**
 * APUNTS TEORIA
 * void task_switch (struct task_struct *new) {
 * 		dynamic link
 * 		current()->kernel_esp = ebp
 * 		esp = new->kernel_esp
 * 		cr3 = new->dir_pages_baseAddr    // flush TLB
 * 		pop ebp
 * 		ret
 * /

