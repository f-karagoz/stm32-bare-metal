#include "uart1.h"

/******* STEPS FOLLOWED ********
1. Enable the UART CLOCK and GPIO CLOCK.
2. Configure the UART PINs for Alternate Functions.
3. Enable the UART by writing the UE bit in USART_CR1 register to 1.
4. Program the M bit in USART_CR1 to define the word length.
5. Program the number of stop bits in USART_CR2.
6. Select the desired baud rate using the baud rate register USART_BRR.
7. Enable the interrupt(s).
8. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
********************************/

// UART2 was connected to the APB1 which was set as 50Mhz, pins were at PA2 PA3, AF7
// UART1 is at APB2 100MHz, pins at PA9 PA10, AF7
void uart1_config (void)
{
	//! 1. Enable the UART CLOCK and GPIO CLOCK
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;							// Enable USART1's clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;							// Enable GPIOA's clock (A port used by USART1's pins PA9:10)

	//! 2. Configure the UART PINs for Alternate Functions
	GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; 		// PA9 and PA10 pins set as Alternate Function
	GPIOA->AFR[1] |= ( 7 << 4 ) | ( 7 << 8 ); 						// AF7 set for pin 9 and 10. Those pins' AFR is in the AFRH register therefore AFR[1].
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9 | GPIO_OSPEEDR_OSPEED10; 	// Pins setup as high speed.


	//! [1] 3. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	USART1->CR1 |= USART_CR1_UE;									// Enable USART

	//! [2] 4. Program the M bit in USART_CR1 to define the word length.
	USART1->CR1 &= ~USART_CR1_M; 									// We set the M as 0 for 8 bit message format.

	//! [3] 5. Program the number of stop bits in USART_CR2.
	USART1->CR2 &= ~USART_CR2_STOP; 								// For 1 stop bit (13:12) set as 0:0

	//! [4] 6. Select the desired baud rate using the baud rate register USART_BRR.
	USART1->CR1 &= ~USART_CR1_OVER8;								// Set the over-sampling by 16 to be sure.
	// For 115200 baud rate the BRR calculation via the RM0401 s.24.4.4 formula.
	// USART1 is connected to APB2 and we previously set 100MHz to that bus line.
	// Calculated USARTDIV is 54.2534
	// Calculated mantissa is 0d54 := 0x36
	// Calculated fraction is 0d4 := 0x4
	USART1->BRR |= ( (0x36 << USART_BRR_DIV_Mantissa_Pos ) | ( 0x4 << USART_BRR_DIV_Fraction_Pos ) );

	//! 7. Enable the interrupt(s).
	USART1->CR1 |= USART_CR1_RXNEIE;								// RXNE interrupt enabled.
	// NVIC_SetPriority ( USART1_IRQn, 0 );
	NVIC_EnableIRQ ( USART1_IRQn );									// Enable IRQ

	//! [5] 8. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;						// Enable Transmit and Receive

}

/*********** STEPS FOLLOWED *************
1. Write the data to send in the USART_TDR register (this clears the TXE bit). Repeat this
	 for each data to be transmitted in case of single buffer.
2. After writing the last data into the USART_TDR register, wait until TC=1. This indicates
	 that the transmission of the last frame is complete. This is required for instance when
	 the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.
****************************************/
void uart1_send_char ( uint8_t c )
{

	if ( USART1->SR & USART_SR_TXE ) 								// First check if the Transmit Data Register is empty
	{
		USART1->DR = c;												// Load the char to be transmitted into DR
																	// Reading from SR and writing to DR cleared the TC bit
		while ( !( USART1->SR & USART_SR_TC ) );					// Wait for Transmit Complete flag to be set in the SR
	}
}

void uart1_send_data ( uint8_t* data )								// The input of the function is the address of the char array
{
	while ( *data != '\0' )											// We will be looping until we find NULL char
	{
		uart1_send_char ( *data );									// We send the value pointed by the current address
		(data)++;													// We increment the address by one for the next 8 bit
	}
}

/*********** STEPS FOLLOWED *************
1. Wait for the RXNE bit to set. It indicates that the data has been received and can be read.
2. Read the data from USART_DR  Register. This also clears the RXNE bit
****************************************/
uint8_t uart1_get_char ( void )
{
	uint8_t data;
	while ( !( USART1->SR & USART_SR_RXNE ) ); 						// Wait for the RX Not Empty Flag to set. Which indicates that data can be read.
	data = USART1->DR;
	return data;
}
