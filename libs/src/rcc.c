#include "rcc.h"

// #define INTERNAL_CLOCK // Если внутренняя RC цепочка

void rccInit(void) { // Инициализация тактирования
	clearCPU();												// Отключение всей периферии
	_delay_ms(100);
	// Включение акселератора памяти Flash(максимальные циклы чтения памяти)
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY;
		
#ifndef INTERNAL_CLOCK
///////// Для внешнего кварца v
// Изменение веток тактирования на внешний кварц
	RCC->CR = RCC_CR_HSEON; // Переключаем на внешний	 кварц
	while(!(RCC->CR & RCC_CR_HSERDY)){} // Ожидаем, пока не переключится

// Раскачка умножителей PLL
	RCC->PLLCFGR = (4 << RCC_PLLCFGR_PLLM_Pos)
								| (180 << RCC_PLLCFGR_PLLN_Pos)
								 | (0 << RCC_PLLCFGR_PLLP_Pos)
									| (7 << RCC_PLLCFGR_PLLQ_Pos)
										| (2 << RCC_PLLCFGR_PLLR_Pos)
											| RCC_PLLCFGR_PLLSRC_HSE
	;
///////// Для внешнего кварца ^
#else
/////// Для внутренней RC цепочки (16 Мгц) v		
	RCC->CR = RCC_CR_HSION; // Переключаем на внешний	 кварц
	while(!(RCC->CR & RCC_CR_HSIRDY)){} // Ожидаем, пока не переключится	
	
// Раскачка умножителей PLL
	RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos)
								| (180 << RCC_PLLCFGR_PLLN_Pos)
								 | (0 << RCC_PLLCFGR_PLLP_Pos)
									| (7 << RCC_PLLCFGR_PLLQ_Pos)
									 | (2 << RCC_PLLCFGR_PLLR_Pos)
										| RCC_PLLCFGR_PLLSRC_HSI
	;	
/////// Для внутренней RC цепочки ^
#endif		
		
// Поднимаем до 168 МГц
	RCC->CR |= RCC_CR_PLLON; // Включаем PLL
	while(!(RCC->CR & RCC_CR_PLLRDY)){} // Ожидаем, пока не включится
	
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2; // Включаем делители частоты периферийных шин
	
	RCC->CFGR |= RCC_CFGR_SW_PLL;							
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)){}
}
