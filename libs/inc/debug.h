#ifndef  __DEBUG_H
#define __DEBUG_H

#include "main.h"

// Отладка на USART1
// PA10 - RX (Не используется), PA9 - TX

#define testPinOff  GPIOC->BSRR = GPIO_BSRR_BR9
#define testPinOn   GPIOC->BSRR = GPIO_BSRR_BS9 

int  isDebugOn (void);															// Проверка настроен ли Debug
void debugInit(void); 															// Инициализация тактирования
void debugSendStr(uint8_t *addr);										// Отправка строки
void printfArray(uint8_t *addr, uint16_t count);		// Вывод данных в шестнадцатеричной форме
void printfArray32(uint32_t *addr, uint16_t count);
void testPinChange(void);														// Тестовый пин PC9

#endif 
