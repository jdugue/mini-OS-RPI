#include "sem.h"
#include "hw.h"
#include "music.h"

extern void play_music();

void
turn_led_on ()
{

	while( 1 ) {
		led_on();
	}
}
void
turn_led_off ()
{
	int i = 0;
	while( 1 ) {
		if ( i >0 ){ led_on(); }
		if ( i <0 ){ led_off(); }
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

	create_process( turn_led_off );
	create_process( play_music );

	start_scheduler();

 	return 0;
}


