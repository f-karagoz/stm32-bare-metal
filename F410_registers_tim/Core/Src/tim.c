/*
* Copyright (c) 2022, 2023 by
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
 *  9. SET THE NVIC to get callback from interrupt events
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
