/*
* Copyright (c) 2023 by
* Furkan Karagoz, Istanbul/Turkey. All rights reserved.
*
* Permission to use, copy, modify, and distribute this software
* is freely granted, provided that this notice is preserved.
*/

#include "main.h"

#define DUTY_STEP		200

static uint8_t rampingUp = 1;

int main ( void )
{
	system_clock_config ();
	tim11_config ( 1000, 5000 );

	TIM11->CCR1 = 0;

	tim11_enable ( 1 );

	while (1)
		;

	return 0;
}


void TIM1_TRG_COM_TIM11_IRQHandler ( void )
{
	if ( TIM11->SR & TIM_SR_UIF )
	{
		if ( rampingUp )	// Ramping up
		{
			if ( TIM11->CCR1 <= TIM11->ARR )
			{
				TIM11->CCR1 += DUTY_STEP;
			}
			else					// Reached top, go down
			{
				rampingUp = 0;
				TIM11->CCR1 -= DUTY_STEP;
			}
		}
		else	// Ramping down
		{
			if ( TIM11->CCR1 > 0 )
			{
				TIM11->CCR1 -= DUTY_STEP;
			}
			else					// Reached bottom, go up
			{
				rampingUp = 1;
				TIM11->CCR1 += DUTY_STEP;
			}
		}

		TIM11->SR &= ~TIM_SR_UIF;	// Clear update interrupt flag
	}
//	else if ( TIM11->SR & TIM_SR_CC1IF )
//	{
//		TIM11->SR &= ~TIM_SR_CC1IF;	// Clear CC interrupt flag
//	}
}
