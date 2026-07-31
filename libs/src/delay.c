#include "delay.h"
#include "config.h"
#include <stdio.h>
#include <stm32f4xx.h>

void _delay_us(uint32_t temp) { // Задержка в микросекундах
	temp = (F_CPU/30000000)*temp;
	
	while(temp--) {
		__ASM("NOP");
	}
}

void _delay_ms(uint32_t temp) { // Задержка в миллисекундах
	temp = (F_CPU/30000)*temp;

	while(temp--) {
		__ASM("NOP");
	}
}
