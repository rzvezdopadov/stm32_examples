#include "delay.h"

void _delay_us(uint64_t temp) { // Задержка в микросекундах
	temp = (F_CPU/30000000)*temp;
	
	while(temp--){
		__ASM("NOP");
	}
}

void _delay_ms(uint64_t temp) { // Задержка в миллисекундах
	temp = (F_CPU/30000)*temp;
	
	while(temp--){
		__ASM("NOP");
	}
}
