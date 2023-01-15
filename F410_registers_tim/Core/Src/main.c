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
		;

	return 0;
}


void TIM6_DAC_IRQHandler ( void )
{
	if ( TIM6->SR & TIM_SR_UIF )
	{
		TIM6->SR &= ~TIM_SR_UIF;

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
	}
}
