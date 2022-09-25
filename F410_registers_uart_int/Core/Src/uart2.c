#include "uart2.h"

/******* STEPS FOLLOWED ********
1. Enable the UART CLOCK and GPIO CLOCK.
2. Configure the UART PINs for Alternate Functions.
3. Enable the UART by writing the UE bit in USART_CR1 register to 1.
4. Program the M bit in USART_CR1 to define the word length.
5. Program the number of stop bits in USART_CR2.
6. Select the desired baud rate using the baud rate register USART_BRR.
7. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
********************************/

void uart2_config (void)
{
	//! 1. Enable the UART CLOCK and GPIO CLOCK
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;							// Enable USART3's clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;							// Enable GPIOA's clock (A port used by USART2's pins PA2:3)

	//! 2. Configure the UART PINs for Alternate Functions
	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1; 		// PA2 and PA3 pins set as Alternate Function
	GPIOA->AFR[0] |= ( 7 << 8 ) | ( 7 << 12 ); 						// AF7 set for pin 2 and 3. Those pins' AFR is in the AFRH register therefore AFR[1].
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3; 	// Pins setup as high speed.


	//! [1] 3. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	USART2->CR1 |= USART_CR1_UE;									// Enable USART

	//! [2] 4. Program the M bit in USART_CR1 to define the word length.
	//USART3->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);
	USART2->CR1 &= ~USART_CR1_M; 									// We set the M as 0 for 8 bit message format.

	//! [3] 5. Program the number of stop bits in USART_CR2.
	USART2->CR2 &= ~USART_CR2_STOP; 								// For 1 stop bit (13:12) set as 0:0

	//! [4] 6. Select the desired baud rate using the baud rate register USART_BRR.
	USART2->CR1 &= ~USART_CR1_OVER8;								// Set the over-sampling by 16 to be sure.
	// For 115200 baud rate the BRR calculation via the RM0401 s.24.4.4 formula.
	// USART2 is connected to APB1 and we previously set 50MHz to that bus line.
	// Calculated mantissa id 0d27 := 0x1B
	// Calculated fraction is 0d2 := 0x2
	USART2->BRR |= ( (0x1B << USART_BRR_DIV_Mantissa_Pos ) | ( 0x2 << USART_BRR_DIV_Fraction_Pos ) );

	//! [5] 7. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;						// Enable Transmit and Receive

}

/*********** STEPS FOLLOWED *************
1. Write the data to send in the USART_TDR register (this clears the TXE bit). Repeat this
	 for each data to be transmitted in case of single buffer.
2. After writing the last data into the USART_TDR register, wait until TC=1. This indicates
	 that the transmission of the last frame is complete. This is required for instance when
	 the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.
****************************************/
void uart2_send_char ( uint8_t c )
{

	if ( USART2->SR & USART_SR_TXE ) 								// First check if the Transmit Data Register is empty
	{
		USART2->DR = c;												// Load the char to be transmitted into DR
																	// Reading from SR and writing to DR cleared the TC bit
		while ( !( USART2->SR & USART_SR_TC ) );					// Wait for Transmit Complete flag to be set in the SR
	}
}

void uart2_send_data ( uint8_t* data )								// The input of the function is the address of the char array
{
	while ( *data != '\0' )											// We will be looping until we find NULL char
	{
		uart2_send_char ( *data );									// We send the value pointed by the current address
		(data)++;													// We increment the address by one for the next 8 bit
	}
}

/*********** STEPS FOLLOWED *************
1. Wait for the RXNE bit to set. It indicates that the data has been received and can be read.
2. Read the data from USART_DR  Register. This also clears the RXNE bit
****************************************/
uint8_t uart2_get_char ( void )
{
	uint8_t data;
	while ( !( USART2->SR & USART_SR_RXNE ) ); 						// Wait for the RX Not Empty Flag to set. Which indicates that data can be read.
	data = USART2->DR;
	return data;
}
