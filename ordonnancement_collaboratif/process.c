#include "process.h"
#include "allocateMemory.h"

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size)
{
	ctx->pc = f; // pour avoir l'adresse
	ctx->sp = AllocateMemory(stack_size)+(stack_size-1)*4;
	ctx->registres = (uint32_t*) malloc(sizeof(uint32_t)*13);
}
