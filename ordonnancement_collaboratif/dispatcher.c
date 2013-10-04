#include "dispatcher.h"

void switch_to(struct ctx_s* ctx)
{	
	current_ctx.pc = ctx.pc;
	current_ctx.sp = ctx.sp;
}
