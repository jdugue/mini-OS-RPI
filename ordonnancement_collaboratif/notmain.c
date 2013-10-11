#include "dispatcher.h"

#define STACK_SIZE 128

struct ctx_s ctx_A;
struct ctx_s ctx_B;
struct ctx_s ctx_init;

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

void
funcA ()
{
	int cptA = 0;
	while( 1 ) {
		cptA ++;
		switch_to(&ctx_B ) ;
	}
}
void
funcB ()
{
	int cptB = 1;
	while( 1 ) {
		cptB += 2 ;
		switch_to(&ctx_A ) ;
	}
}

//------------------------------------------------------------------------
int
notmain ( void )
{
  init_ctx(&ctx_A, funcA, STACK_SIZE);
  init_ctx(&ctx_B, funcB, STACK_SIZE);

  current_ctx = &ctx_init;

  switch_to(&ctx_A);

  return 0;
}


