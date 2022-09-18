#include "main.h"

int main ( void )
{
	system_clock_config();
	uart2_config();

	uint8_t testData[ 10 ] = {'H','E','L','L','O','!','\n','\0'};

	uart2_send_data ( testData );

	while ( 1 )
	{
		// Code here
	}
}


