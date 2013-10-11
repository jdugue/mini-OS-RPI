#include <stdint.h>

typedef void (*func_t) ( void);

typedef struct ctx_s {
	uint32_t * sp ; //equivalent a uint32_t ?
	uint32_t * pc; //idem
	uint32_t registres[13];
}ctx_s;

void init_ctx(ctx_s* ctx, func_t f, unsigned int stack_size);
