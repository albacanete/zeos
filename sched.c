/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <list.h>
#include <errno.h>

/**
 * Container for the Task array and 2 additional pages (the first and the last one)
 * to protect against out of bound accesses.
 */
union task_union protected_tasks[NR_TASKS+2]
  __attribute__((__section__(".data.task")));

union task_union *task = &protected_tasks[1]; /* == union task_union task[NR_TASKS] */

//#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
//#endif

extern struct list_head blocked;
struct task_struct *idle_task;
int ticks_left;

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
	struct task_struct *its = list_head_to_task_struct(ilh);
	its->PID = 0;
	allocate_DIR(its);
		
	union task_union *itu = (union task_union*)its;
	//task_switch(&itu);
	
	itu->stack[1020] = &(cpu_idle);
	itu->stack[1016] = 0;
	itu->task.kernel_esp  = &(itu->stack[1016]);
	
	idle_task = its;
}

void init_task1(void)
{
	struct list_head *init_lh = list_first(&freequeue);
	list_del(init_lh); //l'eliminem, sino segueix a la freequeue
	struct task_struct *init_ts = list_head_to_task_struct(init_lh);
	init_ts->PID = 1;
	allocate_DIR(init_ts);
	set_user_pages(init_ts);
	tss.esp0 = &(((union task_union *) init_ts) -> stack);  //stack[1024]
	set_cr3(init_ts -> dir_pages_baseAddr);
	
}


void init_sched(){
	INIT_LIST_HEAD(&freequeue);
	INIT_LIST_HEAD(&readyqueue);
	
	/* Add the new element to the list, using the element's anchor.   
	 * list_add( &(el.anchor), &list );
	 * void list_add(struct list_head *new, struct list_head *head)
	 */
	 int i = 1;
	 while (i < NR_TASKS -1) {
		 list_add_tail(&(task[i].task.list), &freequeue);
		 ++i;
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

// 1. Guardar esi edi ebx
	//save_registers();
	
	// 2. Llamar a inner_task_switch
	// es la que realment canvia el proces 
	//inner_task_switch(&new);
	
	// 3. Restauramos esi edi ebx
	
void inner_task_switch(union task_union*new) {
	//1) update de tss
	tss.esp0 = &(((union task_union*) new) -> stack[1024]);  
	//2) change user address space --> update the current page directory
	struct task_struct *new_ts = (struct task_struct*)new;  //agafo el task_struct del nou proces a executar
	page_table_entry* new_TD = new_ts->dir_pages_baseAddr; //agafo el punter a la seva taula de pagines
	set_cr3(new_TD);
}

/*
 * APUNTS TEORIA
 * void task_switch (struct task_struct *new) {
 * 		dynamic link
 * 		cr3 = new -> dir
 * 		tss esp0 = &(((task_union*) new) -> stack[1024])
 * 		current()->kernel_esp = ebp
 * 		esp = new->kernel_esp
 * 		cr3 = new->dir_pages_baseAddr    // flush TLB
 * 		pop ebp
 * 		ret
 */

// Function to update the relevant information to take scheduling decisions
void update_sched_data_rr(){
	--ticks_left;
}

// Function to decide if it is necessary to change the current process
int needs_sched_rr(){
	return (ticks_left == 0);
}

// Function to update the state of a process
void update_process_state_rr(struct task_struct *t, struct list_head *dest){
	
}

// Function to select the next function to execute
void sched_next_rr(){
	struct list_head *next_lh = list_first(&readyqueue);
	list_del(next_lh); 
	//task_switch((union task_union*) list_head_to_task_struct(next_lh));
}

void schedule () {
	update_sched_data_rr();
	if (needs_sched_rr()) {
		if (list_empty(&readyqueue)) {
			 //task_switch((union task_union*) idle_task);
		 }
		else {
			update_process_state_rr(current(), &readyqueue);
			sched_next_rr();
		}
	}
}

int getquantum(struct task_struct *t) {
	return t->quantum;
}
 
void set_quantum(struct task_struct *t, int new_quantum) {
	t->quantum = new_quantum;
}

