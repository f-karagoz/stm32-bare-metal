#ifndef __INC_TIM_H
#define __INC_TIM_H

#include "stm32f410rx.h"

/**
 * @brief Configures the TIM peripheral
 *
 * @param psc	Prescaler value that divides the counter clock
 * @param arr	Auto reload value
 */
void tim6_config ( uint16_t psc, uint16_t arr );

/**
 * @brief Enables and disables the timer 6
 *
 * @param state 1 to enable, 0 to disable
 */
void tim6_enable ( uint8_t state );

#endif /* __INC_TIM_H */
