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
 *  4. Set the CR1.URS bit to enable one pulse mode
 *  5. Set the EGR.UG to generate an update to transfer the content of the preload registers to buffers
 **************************************************************************************/
void tim6_config ( uint16_t psc, uint16_t arr )
{
	//! 1. Enable the timer clock source
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	//! 2. Set the prescaler
	TIM6->PSC = psc - 1;

	//! 3. Set the auto reload
	TIM6->ARR = arr - 1;

	//! 4. One pulse mode causes timer to stop counting in the next update event
	TIM6->CR1 |= TIM_CR1_OPM;

	//! 5. Transfer the content of the preload registers to buffers
	TIM6->EGR |= TIM_EGR_UG;

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
