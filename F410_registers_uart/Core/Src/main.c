#include "main.h"

#define BUFFER_SIZE		100

int main ( void )
{
	system_clock_config();
	uart2_config();

	uint8_t rxBuffer[ BUFFER_SIZE ] = { 0 };
	uint8_t testData[ 10 ] = {'H','E','L','L','O','!','\n','\0'};

	uart2_send_data ( testData );

	while ( 1 )
	{

		for ( int i = 0; i < BUFFER_SIZE - 1; i++)						// Last element of the buffer is reserved for null termination
		{
			uint8_t gatheredChar = 0;

			gatheredChar = uart2_get_char();							// Blocking till data received

			rxBuffer[ i ] = gatheredChar;

			if ( gatheredChar == '\r' || gatheredChar == '\n' )			// Received data is completed with CR or LF
			{
				rxBuffer[ i + 1 ] = 0;									// Terminate the package

				break;													// Break from the for loop
			}
		}

		uart2_send_data ( rxBuffer );									// Transmit back the received package

		for ( int j = 0; rxBuffer[ j ] == 0 || j < BUFFER_SIZE ; j++)	// Clear the rx buffer
			rxBuffer[ j ] = 0;

	}
}


