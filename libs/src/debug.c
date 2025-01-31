#include "debug.h"

// Отладка на USART1
// PA10 - RX (Не используется), PA9 - TX
#define debugBaud 1000000 // Битрейт отладки
#define debugBRR (((F_CPU / 2) + (debugBaud / 2)) / debugBaud)

int isDebugOn (void) {	// Проверка настроен ли Debug
	if (
		(RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN) 
	&& (RCC->APB2ENR & RCC_APB2ENR_USART1EN)
	) return 1;
		
	return 0;
}

void debugInit(void) { // Инициализация отладки
	// Альтернативная функция порта
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2 
//									|	GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2
	;
	GPIOA->MODER |= GPIO_MODER_MODE9_1
//								| GPIO_MODER_MODE10_1
	;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_Msk
//										| GPIO_OSPEEDR_OSPEED10_Msk
	;
// Настройки
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR = debugBRR;
	USART1->CR1 |= USART_CR1_UE | USART_CR1_TE;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->MODER 	 |= GPIO_MODER_MODE6_0;
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_Msk;
	printf("\n\n---------- Debug bootloader init Ok! Freeq = %dMHz, Brr = %d ----------\n", F_CPU / 1000000, debugBRR);
}

int fputc(int ch, FILE *f) {
	debugSendStr((uint8_t *)&ch);
	return ch;
}

void debugSendStr(uint8_t *addr) {					// Функция передачи данных побайтно до символа 0x00
	if (!isDebugOn()) return;
	
	uint32_t count = 0; 
	
	while (*addr) {
		USART1->DR = *addr++;										// Отправляем очередной байт
		while(!(USART1->SR & USART_SR_TXE)) {
			if (count++ > 100000) return;
		}	// Ожидаем сброса флага окончания передачи данных
	}
}

void printfArray(uint8_t *addr, uint32_t count) {	// Функция вывода данных в HEX форме
	if (count < 1) return;
	
	for (uint32_t i=0; i<count; i++) {
		if (!(i%16) & (i != 0)) printf("\n");
		printf("%02X ", addr[i]);
	}
	printf("\n--------------------------\n");
}

void printfArray32(uint32_t *addr, uint32_t count) {	// Функция вывода данных в HEX форме 32 битных чисел
	if (count < 1) return;
	
	for (uint32_t i=0; i<count; i++) {
		if (!(i%4) & (i != 0)) printf("\n");
		printf("%08X ", addr[i]);
	}
	printf("\n-------------------------------\n");
}

void testPinChange(void) {
	if (GPIOE->ODR & GPIO_ODR_OD6) { 
		testPinOff;
	} else {
		testPinOn; 
	}
}
