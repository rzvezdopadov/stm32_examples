#include "clear.h"

void clearRAM(uint8_t *addr, uint32_t size) {
	for (uint32_t i=0; i<size; i++) {
		addr[i] = 0;
	}
}

void clearCPU(void) { 						// Разинициализация контроллера
	__disable_irq();

	NVIC->ICER[0] = 0xFFFFFFFF;
	NVIC->ICER[1] = 0xFFFFFFFF;
	NVIC->ICER[2] = 0xFFFFFFFF;

	NVIC->ICPR[0] = 0xFFFFFFFF;
	NVIC->ICPR[1] = 0xFFFFFFFF;
	NVIC->ICPR[2] = 0xFFFFFFFF;

	RCC->AHB1RSTR = 0x206010FF;
	RCC->AHB2RSTR = 0x00000081;
	RCC->AHB3RSTR = 0x00000003;
	RCC->APB1RSTR = 0x3FFFC9FF;
	RCC->APB2RSTR = 0x00C77933;

	RCC->AHB1RSTR = 0;
	RCC->AHB2RSTR = 0;
	RCC->AHB3RSTR = 0;
	RCC->APB1RSTR = 0;
	RCC->APB2RSTR = 0;

	RCC->AHB1ENR 	= 0;
	RCC->AHB2ENR 	= 0;
	RCC->AHB3ENR 	= 0;
	RCC->APB1ENR 	= 0;
	RCC->APB2ENR 	= 0;

	SysTick->CTRL = 0;
	SysTick->VAL 	= 0;
	SysTick->LOAD = 0;
	
	RCC->CR 			= 0;
	RCC->PLLCFGR 	= 0;
	RCC->CFGR 		= 0;
	
	__enable_irq();      //запрещаем прерывания
}
