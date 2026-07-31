#include "rcc.h"
#include "stm32f4xx.h"
#include "config.h"
#include "delay.h"
#include "clear.h"

// #define INTERNAL_CLOCK // Если внутренняя RC цепочка

void rccInit(void) {
	_delay_ms(100);
	clearCPU();  // Отключение всей периферии
	_delay_ms(100);

	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY;

#ifndef INTERNAL_CLOCK
	// Включение HSE
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Настройка PLL для внешнего кварца
	RCC->PLLCFGR = (4 << RCC_PLLCFGR_PLLM_Pos) |
								 ((F_CPU / 1000000) << RCC_PLLCFGR_PLLN_Pos) |
								 (0 << RCC_PLLCFGR_PLLP_Pos) |  // PLLP = 2
								 (7 << RCC_PLLCFGR_PLLQ_Pos) |
								 RCC_PLLCFGR_PLLSRC_HSE;
#else
	// Включение HSI
	RCC->CR |= RCC_CR_HSION;
	while (!(RCC->CR & RCC_CR_HSIRDY));

	// Настройка PLL для внутреннего RC-генератора
	RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos) |
								 (F_CPU / 1000000 << RCC_PLLCFGR_PLLN_Pos) |
								 (0 << RCC_PLLCFGR_PLLP_Pos) |  // PLLP = 2
								 (7 << RCC_PLLCFGR_PLLQ_Pos) |
								 RCC_PLLCFGR_PLLSRC_HSI;
#endif

	// Включение PLL
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Переключение SYSCLK на PLL
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	// Настройка делителей для периферийных шин
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2)) |
							RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
}