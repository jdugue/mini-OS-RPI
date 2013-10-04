#include <stdint.h>

typedef void (*func_t) ( void);

struct ctx_s {
	uint32_t * sp ; //equivalent a uint32_t ?
	uint32_t * pc; //idem
};

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);