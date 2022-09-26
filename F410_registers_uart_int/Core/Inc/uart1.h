#ifndef UART1_H
#define UART1_H

#include "stm32f410rx.h"

/**
 * @brief Configures UART.
 *
 */
void uart1_config (void);

/**
 * @brief Sends one char through USART1;
 *
 * @param c Actual char.
 */
void uart1_send_char (uint8_t c);

/**
 * @brief Sends char array.
 *
 * @param data Pointer to char array.
 */
void uart1_send_data (uint8_t * data);

/**
 * @brief Gets the char from UART.
 *
 * @return Read char data.
 */
uint8_t uart1_get_char (void);

#endif /* UART2_H */
