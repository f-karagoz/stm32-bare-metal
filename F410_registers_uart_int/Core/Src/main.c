#include "main.h"

#define BUFFER_SIZE		100

uint8_t rxBuffer[ BUFFER_SIZE ] = { 0 };
uint8_t idx = 0;
uint8_t rxBuffer_2[ BUFFER_SIZE ] = { 0 };
uint8_t idx_2 = 0;

int main ( void )
{
	system_clock_config();
	uart1_config();
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

			if ( rxBuffer[ idx ] == '\n' )		// If LF gathered send the received data
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


void USART1_IRQHandler ( void )
{
	if ( USART1->SR & USART_SR_RXNE )			// Read from DR clears the flag
	{
		if ( idx_2 < BUFFER_SIZE )				// Preventing overflow of the buffer
		{
			rxBuffer_2[ idx_2 ] = USART1->DR;

			if ( rxBuffer_2[ idx_2 ] == '\n' )		// If LF gathered send the received data
			{
				uart1_send_data ( rxBuffer_2 );

				idx_2 = 0;

				// Clear the rx buffer. Might be bad method to run inside the int handler though.
				for ( int j = 0; rxBuffer_2[ j ] == 0 || j < BUFFER_SIZE ; j++)
					rxBuffer_2[ j ] = 0;

				return;
			}

			idx_2++;
		}
		else
		{
			idx_2 = 0;
		}

	}
	else
	{
		while ( 1 );							// Unexpected, wait for debug
	}
}
