#ifndef UART2_H
#define UART2_H

#include "stm32f410rx.h"

/**
 * @brief Configures UART.
 *
 */
void uart2_config (void);

/**
 * @brief Sends one char through USART3;
 *
 * @param c Actual char.
 */
void uart2_send_char (uint8_t c);

/**
 * @brief Sends char array.
 *
 * @param data Pointer to char array.
 */
void uart2_send_data (uint8_t * data);

/**
 * @brief Gets the char from UART.
 *
 * @return Read char data.
 */
uint8_t uart2_get_char (void);

#endif /* UART2_H */
