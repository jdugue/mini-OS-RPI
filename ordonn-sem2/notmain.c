#include "dispatcher.h"
#include "sem.h"
#include "hw.h"

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
	while( 1 ) {
		led_on();
	}
}
void
funcB ()
{
	int cptB = 1;
	while( 1 ) {
		led_off();
	}
}

//-----------------------------------------------------------------------------
//------------------------ PHILOSOPHERS ---------------------------------------
static unsigned long long mange[5];
static mtx_s* chopsticks[5];

void philosopher(int val)
{
	if ( val % 2 )
	//Odd philoshoper
	{
		while ( 1 )
		{
			mtx_lock( chopsticks[ (val+1) % 5 ] );
			mtx_lock( chopsticks[val]   );

			mange[val]++;

			mtx_unlock( chopsticks[val] );
			mtx_unlock( chopsticks[ (val+1) % 5 ] );
		}
	}
	else
	//Even philosopher
	{
		while ( 1 )
		{
			mtx_lock( chopsticks[val] );
			mtx_lock( chopsticks[ (val+1) % 5 ] );

			mange[val]++;

			mtx_unlock( chopsticks[ (val+1) % 5 ] );
			mtx_unlock( chopsticks[val]   );
		}
	}
}



//-----------------------------------------------------------------------------
int
notmain ( void )
{
  //init_ctx(&ctx_A, funcA, STACK_SIZE);
  //init_ctx(&ctx_B, funcB, STACK_SIZE);
  //current_ctx = &ctx_init;
  //switch_to(&ctx_A);

	create_process(funcA, 0,2,1);
	create_process(funcB, 0,2,1);
	//start_scheduler();

/*
	int i;
	for( i=0; i<5; i++)
	{
		chopsticks[i] = mtx_init();
		create_process( philosopher, i );
	}
 */

	start_scheduler();

 	return 0;
}


