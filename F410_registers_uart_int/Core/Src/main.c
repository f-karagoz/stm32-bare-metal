#include "main.h"

#define BUFFER_SIZE		100

#define APP_3

#if ( defined APP_1 ) || ( defined APP_2 )
uint8_t rxBuffer[ BUFFER_SIZE ] = { 0 };
uint8_t idx = 0;
#endif /* ( defined APP_1 ) || ( defined APP_2 ) */

#ifdef APP_2
uint8_t rxBuffer_2[ BUFFER_SIZE ] = { 0 };
uint8_t idx_2 = 0;
#endif /* APP_2 */

int main ( void )
{
	system_clock_config();
	uart1_config();
	uart2_config();

	while ( 1 )
	{
	}
}

#if ( defined  APP_1 ) || ( defined APP_2 )

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
#endif /* ( defined  APP_1 ) || ( defined APP_2 ) */

#ifdef APP_2
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
#endif /* APP_2 */

#ifdef APP_3

void USART1_IRQHandler ( void )
{
	if ( USART1->SR & USART_SR_RXNE )	// Incoming data to UART1
	{
		USART2->DR = USART1->DR;		// Read from UART1->DR and write to UART2->DR
	}
}

void USART2_IRQHandler ( void )
{
	if ( USART2->SR & USART_SR_RXNE )	// Incoming data to UART2
	{
		USART1->DR = USART2->DR;		// Read from UART2->DR and write to UART1->DR
	}
}

#endif
