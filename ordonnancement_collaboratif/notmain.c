#include "dispatcher.h"

//struct ctx_s ctx_init;
/*
void
ping()
{
  while (1) {
    switch_to(&ctx_B);
    switch_to(&ctx_B);
  }
}

void
pong()
{
  while (1) {
    switch_to(&ctx_A);
    switch_to(&ctx_A);
    switch_to(&ctx_A);
  }
}
*/

void
funcA ()
{
	int cptA = 0;
	while( cptA < 3 ) {
		cptA ++;
		yield();
	}
}
void
funcB ()
{
	int cptB = 1;
	while( cptB < 5 ) {
		cptB += 2 ;
		yield();
	}
}

//------------------------------------------------------------------------
int
notmain ( void )
{
 // init_ctx(&ctx_A, funcA, STACK_SIZE);
  //init_ctx(&ctx_B, funcB, STACK_SIZE);

  //current_ctx = &ctx_init;

  //switch_to(&ctx_A);

	create_process(funcA, 0);
	create_process(funcB, 0);

	start_scheduler();

  return 0;
}


