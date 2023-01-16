/*
* Copyright (c) 2022, 2023 by
* Furkan Karagoz, Istanbul/Turkey. All rights reserved.
*
* Permission to use, copy, modify, and distribute this software
* is freely granted, provided that this notice is preserved.
*/

#include "main.h"

uint8_t ledState = 0;

int main ( void )
{
	system_clock_config ();
	// 100Mhz APB1 timer clock is divided by 1000. Timer counter is counting up every 10us
	// Timer is reloaded after 10us * 50k = 500ms
	tim6_config ( 1000, 50000 );
	gpio_config ();

	tim6_enable ( 1 );

	while (1)
	{
		if ( TIM6->CNT == TIM6->ARR)
		{
			if (ledState)
			{
				led_off();
				ledState = 0;
			}
			else
			{
				led_on();
				ledState = 1;
			}

			TIM6->CNT = 0;
		}

	}

	return 0;
}
