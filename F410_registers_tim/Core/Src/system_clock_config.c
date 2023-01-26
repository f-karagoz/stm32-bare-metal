/*
* Copyright (c) 2023 by
* Furkan Karagoz, Istanbul/Turkey. All rights reserved.
*
* Permission to use, copy, modify, and distribute this software
* is freely granted, provided that this notice is preserved.
*/

#include "system_clock_config.h"

/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
1. ENABLE HSI and wait for the HSI to become Ready
2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
3. Configure the FLASH PREFETCH and the LATENCY Related Settings
4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
5. Configure the MAIN PLL
6. Enable the PLL and wait for it to become ready
7. Select the Clock Source and wait for it to be set
********************************************************/
void system_clock_config(void)
{

	//! 1. ENABLE HSI and wait for the HSI to become Ready
	// RCC->CR |= (1<<0);
	RCC->CR |= RCC_CR_HSION;  					// Set RCC's CR register's 16th bit to 1
	while (!(RCC->CR & RCC_CR_HSIRDY));

	//! 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	//! 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR |= FLASH_ACR_DCEN;				// Data cache enable
	FLASH->ACR |= FLASH_ACR_ICEN;				// Instruction cache enable
	FLASH->ACR |= FLASH_ACR_PRFTEN;				// Prefetch buffer enable
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS; 		// Flash latency is 3 wait states

	//! 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; 			// AHB PS Div 1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; 			// APB1 PS Div 2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; 			// APB2 PS Div 1

	//! 5. Configure the MAIN PLL
	// PLLM=8	PLLN=100	PLLP=2(!)	PLLQ=3(did not use)		PLLR=2(did not use)
	RCC->PLLCFGR = 0x00000000; 					// Reset the register. Default values prevent us from setting correct values.
	RCC->PLLCFGR |= 8 << RCC_PLLCFGR_PLLM_Pos;
	RCC->PLLCFGR |= 100 << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR |= 0 << RCC_PLLCFGR_PLLP_Pos;	// Actually it does nothing
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; 	// High speed internal clock selected as clock source
	// This setting will multiply the 16 MHz oscillator frequency source to 100 MHz

	//! 6. Enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;
	while ( !( RCC->CR & RCC_CR_PLLRDY ) ); 	// Wait for the PLL to get ready

	//! 7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ( ( RCC->CFGR & RCC_CFGR_SWS_Msk ) != RCC_CFGR_SWS_PLL );
}
