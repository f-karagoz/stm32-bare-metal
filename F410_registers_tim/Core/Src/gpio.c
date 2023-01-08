#include "gpio.h"

void gpio_config (void)
{
	/************ STEPS FOLLOWED ***********
	1. Enable the GPIO CLOCK
	2. Set the Pin as OUTPUT
	3. Configure the OUTPUT MODE
	Note: LED connected to PA5
	***************************************/

	//! 1. Enable the GPIO CLOCK
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;		// Enable GPIO A Clock

	//! 2. Set the Pin as OUTPUT
	GPIOA->MODER |= 1 << GPIO_MODER_MODER5_Pos;	// Set PA5 as General purpose output mode. Bits 10:11 set as (01)

	//! 3. Configure the OUTPUT MODE
	// GPIOB->OTYPER |= ( 0 << 0 );
	GPIOA->OTYPER |= ( 0 << GPIO_OTYPER_OT5_Pos );	// Push-pull type for pin 5.
	//GPIOB->OSPEEDR |= ( 2 << 0 );
	GPIOA->OSPEEDR |= ( 2 << GPIO_OSPEEDR_OSPEED5_Pos );	// High speed setup for pin 0
	//GPIOB->PUPDR &= ~( ( 1 << 1 ) | ( 1 << 0 ) );
	GPIOA->PUPDR &= ~( 3 << GPIO_PUPDR_PUPD5_Pos );	// 10:11 set as (00) for pin 5 to be no pull up or down
}

void led_on (void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS5;
}

void led_off (void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR5;
}
