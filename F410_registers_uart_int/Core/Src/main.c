#include "main.h"

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
	if ( USART2->SR & USART_SR_RXNE )
	{
		uart2_send_char( USART2->DR );			// Read from DR clears the flag
	}
}
