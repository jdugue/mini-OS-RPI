#include "dispatcher.h"
#include "stdint.h"

void switch_to(struct ctx_s* ctx){
	
	uint32_t* varLR = current_ctx->pc;
	uint32_t* varSP = current_ctx->sp;
	__asm("mov %0, lr" : "=r"(varLR)  );
	__asm("mov %0, sp" : "=r"(varSP));


	current_ctx->pc = ctx->pc;
	current_ctx->sp = ctx->sp;

	//Restauration
	varLR = current_ctx->pc;
	varSP = current_ctx->sp;
	__asm("mov lr, %0" : : "r"(varLR));
	__asm("mov sp, %0" : : "r"(varSP));	
}
