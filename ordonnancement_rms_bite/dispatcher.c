#include "hw.h"
#include "sched.h"

void __attribute__ ((naked))
ctx_switch ( void )
{

	DISABLE_IRQ();
	
	__asm("sub lr, lr, #4");
	__asm("srsdb sp!, #0x13");
	__asm("cps #0x13");

	//Save r0-r12
	//A vérifier : Pas de destination car par defaut push sur la pile
	__asm volatile("push {r0-r12,lr}"); 
	__asm("mov %0, sp": "=r"(current_process->sp));

	
	// Choisi le prochain processus a executer
	//schedule();
	select_next();
	
	if(current_process->state == NEW)
	{
		__asm("mov sp, %0" : : "r"(current_process->sp));
		current_process->state = READY;
		
		// Rearme du timer
		set_tick_and_enable_timer();
		ENABLE_IRQ();
		
		current_process->entry_point(current_process->args);
		current_process->state = TERMINATED;
		ctx_switch();

	}
	else if (current_process->state == READY)
	{	
		__asm("mov sp, %0" : : "r"(current_process->sp));
		//Restituer les registres
		__asm volatile("pop {r0-r12,lr}"); 
	}
	
	// Rearme du timer
	set_tick_and_enable_timer();
	ENABLE_IRQ();
	
	__asm("rfefd sp!");
}
