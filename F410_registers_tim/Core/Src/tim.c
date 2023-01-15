#include "tim.h"

/*
 * Features of TIM6:
 * 16-bit auto-reload upcounter
 * 16-bit programmable prescaler (canbe set on the fly)
 * Interrupt / DMA generation
 *
 * Counter Register (TIMx_CNT)
 * Prescaler Register (TIMx_PSC)
 * Auto-Reload Register (TIMx_ARR)
 *
 * The counter is clocked by the prescaler output CK_CNT, which is enabled only when the
 * counter enable bit (CEN) in the TIMx_CR1 register is set.
 *
 * Note that the actual enable signal CNR_EN is set 1 clock cycle after CEN.
 *
 * The prescaler can divide the counter clock by any factor between 1 and 65646.
 * When prescaler is changed on-the-fly, the new prescaler is buffered on the next update event.
 *
 * The counter counts from 0 to the auto-reload value, then restarts from 0 and generates an counter
 * overflow event.
 *
 * An update event can be generated at each counter overflow or by setting the UG bit in
 * the TIMx_EGR register.
 *
 * The UEV event can be disabled by software by setting the UDIS bit in the TIMx_CR1 register.
 * This avoids updating the shadow registers while writing new values into the preload registers.
 * In this way no update event occurs until the UDIS bit has been written to 0, however, the counter
 * and the prescaler counter both restart from 0 (but the prescaler rate does not change).
 *
 * When an update event occurs, all registers are updated and the update flag (UIF bit in the
 * TIMx_SR register) is set (depending on the URS bit):
 *  - The buffer of the prescaler is reloaded with the preload value (contents of the TIMx_PSC register)
 *  - The auto-reload shadow register is updated with the preload value (TIMx_ARR)
 *
 * The counter clock is provided by the Internal clock (CLK_INT) source.
 *
 * When the microcontroller enters the debug mode (Cortex-M4 with FPU core - halted),
 * the TIMx counter either continues to work normally or stops, depending on the
 * DBG_TIMx_STOP configuration bit in the DBGMCU module.
 *
 */


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
