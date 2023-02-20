/*
* Copyright (c) 2023 by
* Furkan Karagoz, Istanbul/Turkey. All rights reserved.
*
* Permission to use, copy, modify, and distribute this software
* is freely granted, provided that this notice is preserved.
*/

#include "tim.h"

/**************************** >>> STEPS FOLLOWED <<< **********************************
 *  1. Enable the timer clock source
 *  2. Set the prescaler
 *  3. Set the auto reload
 *  4. Set the counter mode
 *  5. Enable auto reload preload
 *  6. ! Set CR1.URS to make only counter overflow generate an interrupt
 *  7. ! Set the EGR.UG to generate an update to transfer the content of the preload registers to buffers
 *  // Now an event occurred but UIF bit is not changed due to state of the URS bit (also interrupts are not enabled)
 *  8. Enable the update interrupt DIER.UIE
 *  9. Set the NVIC to get callback from interrupt events
 **************************************************************************************/
void tim6_config ( uint16_t psc, uint16_t arr )
{
	//! 1. Enable the timer clock source
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	//! 2. Set the prescaler
	TIM6->PSC = psc - 1;

	//! 3. Set the auto reload
	TIM6->ARR = arr;

	//! 4. Set the counter mode
	// TIM6 only offers up-counter

	//! 5. Enable auto reload buffering
	// The content of the preload register is transfered to the shadow register at each update event
	TIM6->CR1 |= TIM_CR1_ARPE;

	//! 6. Only counter overflow generate an interrupt
	TIM6->CR1 |= TIM_CR1_URS;

	//! 7. Transfer the content of the preload registers to buffers
	TIM6->EGR |= TIM_EGR_UG;

	//! 8. Enable the update interrupt
	TIM6->DIER |= TIM_DIER_UIE;
	TIM6->SR &= ~TIM_SR_UIF;			// Interrupt should not be triggered by now, but we clear it anyways

	//! 9. Configure the NVIC to run a callback function when interrupt occur
	/* Set interrupt priority */
	IRQn_Type IRQn = TIM6_DAC_IRQn;
	uint32_t prioritygroup = 0x00U;
	uint32_t PreemptPriority = 0;
	uint32_t SubPriority = 0;
	prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));

	/* Enable interrupt */
	NVIC_EnableIRQ(IRQn);

	//! Optional: Stops the timer when debug is halted
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM6_STOP;
}

void tim6_enable ( uint8_t state )
{
	if (state == 0)
		TIM6->CR1 &= ~TIM_CR1_CEN;
	else
		TIM6->CR1 |= TIM_CR1_CEN;

	return;
}

/**************************** >>> STEPS FOLLOWED <<< **********************************
 *  1. Enable peripheral clock sources (GPIOx, and TIMx clock sources)
 *  2. Set mode of the GPIO pin, and set the alternate function
 *  3. Set the counter mode
 *  4. Set the timing parameters
 *  5. Set PWM mode and enable output compare preload register
 *  6. Enable auto-reload preload register
 *  7. Configure signal polarity, and enable the CC output
 *  8. Set interrupt source
 *  9. Load the register data to the hardware (preload-->shadow)
 *  10. Configure and enable interrupt
 *  11. Set the NVIC to handle interrupts
 **************************************************************************************/
void tim11_config ( uint16_t psc, uint16_t arr )
{
	//! 1. Enable peripheral clock sources
	RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;						// TIM11 is connected to APB2
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;						// TIM11 channel is PC12

	//! 2. Set mode of the GPIO pin, and set the alternate function
	GPIOC->MODER |= ( 0x2UL << GPIO_MODER_MODER12_Pos );		// PC12 Alternate function
	GPIOC->OSPEEDR |= ( 0x2UL << GPIO_OSPEEDR_OSPEED12_Pos );	// PC12 High speed
	GPIOC->AFR[1] |= ( 0x3UL << 16);							// AF3 for TIM11

	//! 3. Set the counter mode
	// TIM11 only offers up-counter

	//! 4. Set the timing parameters
	TIM11->PSC = psc - 1;										// Set prescaler
	TIM11->ARR = arr;											// Set auto-reload
	TIM11->CCR1 = arr / 3;										// 50% duty cycle

	//! 5. Set PWM mode and enable output compare preload register
	TIM11->CCMR1 |= ( 0x6UL << TIM_CCMR1_OC1M_Pos );			// PWM mode 1
	TIM11->CCMR1 |= TIM_CCMR1_OC1PE;							// Enable preload of CCR1

	//! 6. Enable auto-reload preload register
	TIM11->CR1 |= TIM_CR1_ARPE;									// Enable preload of ARR

	//! 7. Configure signal polarity, and enable the CC output
	TIM11->CCER &= ~TIM_CCER_CC1P;			// Set signal polarity
	TIM11->CCER |= TIM_CCER_CC1E;			// Enable CC

	//! 8. Set interrupt source
	TIM11->CR1 |= TIM_CR1_URS;									// Setting UG bit does not gen. int.

	//! 9. Load the register data to the hardware (preload-->shadow)
	TIM11->EGR |= TIM_EGR_UG;									// Generate an update event

	//! 10. Configure and enable interrupt
	TIM11->DIER |= ( TIM_DIER_CC1IE | TIM_DIER_UIE );			// Update and CC interrupt enabled
	TIM11->SR &= ~( TIM_SR_CC1IF | TIM_SR_UIF );				// Clear the interrupts

	//! 11. Set the NVIC to handle interrupts
	IRQn_Type IRQn = TIM1_TRG_COM_TIM11_IRQn;
	uint32_t priorityGroup = NVIC_GetPriorityGrouping();
	uint32_t preemptPriority = 0;
	uint32_t subPriority = 0;
	NVIC_SetPriority ( IRQn, NVIC_EncodePriority(priorityGroup, preemptPriority, subPriority) );
	NVIC_EnableIRQ ( IRQn );									// Enable interrupt in the NVIC

	//! Optional: Stops the timer when debug is halted
	DBGMCU->APB2FZ |= DBGMCU_APB2_FZ_DBG_TIM11_STOP;
}

void tim11_enable ( uint8_t state )
{
	if (state == 0)
		TIM11->CR1 &= ~TIM_CR1_CEN;
	else
		TIM11->CR1 |= TIM_CR1_CEN;

	return;
}

void tim11_set_pwm_duty(uint8_t duty)
{
	// TIM11->ARR + 1 is the period
	TIM11->CCR1 = ( (TIM11->ARR + 1) * duty / 100 );
}
