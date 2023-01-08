#include "main.h"

int main ( void )
{
	system_clock_config ();
	tim_config ();				// TODO implement the function
	gpio_config ();

	led_on();					// Used for testing

	return 0;
}
