#include "dispatcher.h"
#include "stdint.h"

void switch_to(struct ctx_s* ctx){
	
	__asm("mov %0, lr" : "=r"(current_ctx->pc));
	__asm("mov %0, sp" : "=r"(current_ctx->sp));

	current_ctx = ctx;

	//Restauration
	__asm("mov lr, %0" : : "r"(current_ctx->pc));
	__asm("mov sp, %0" : : "r"(current_ctx->sp));	
}
