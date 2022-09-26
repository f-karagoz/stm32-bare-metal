#include "main.h"

#define BUFFER_SIZE		100

uint8_t rxBuffer[ BUFFER_SIZE ] = { 0 };
uint8_t idx = 0;

int main ( void )
{
	system_clock_config();
	uart2_config();

	while ( 1 )
	{
	}
}


void USART2_IRQHandler ( void )
{
	if ( USART2->SR & USART_SR_RXNE )			// Read from DR clears the flag
	{
		if ( idx < BUFFER_SIZE )				// Preventing overflow of the buffer
		{
			rxBuffer[ idx ] = USART2->DR;

			if ( rxBuffer[ idx ] == '\n' )	// If LF gathered send the received data
			{
				uart2_send_data ( rxBuffer );

				idx = 0;

				// Clear the rx buffer. Might be bad method to run inside the int handler though.
				for ( int j = 0; rxBuffer[ j ] == 0 || j < BUFFER_SIZE ; j++)
					rxBuffer[ j ] = 0;

				return;
			}

			idx++;
		}
		else
		{
			idx = 0;
		}

	}
	else
	{
		while ( 1 );							// Unexpected, wait for debug
	}
}
