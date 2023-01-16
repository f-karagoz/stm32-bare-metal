/*
* Copyright (c) 2023 by
* Furkan Karagoz, Istanbul/Turkey. All rights reserved.
*
* Permission to use, copy, modify, and distribute this software
* is freely granted, provided that this notice is preserved.
*/

#ifndef __INC_GPIO_H
#define __INC_GPIO_H

#include "stm32f410rx.h"

/*!
 * @brief Configures on-board LED's GPIO.
 *
 */
void gpio_config (void);

/*!
 * @brief Turns LED ON
 *
 */
void led_on (void);

/*!
 * @brief Turns LED OFF
 *
 */
void led_off (void);

#endif /* __INC_GPIO_H */
