#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdint.h>

// Отладка на USART1
// PA10 - RX (Не используется), PA9 - TX

int  isDebugOn (void);
void debugInit(void);
void debugSendStr(uint8_t *addr);
void printfArray(uint8_t *addr, uint32_t count);
void printfArray32(uint32_t *addr, uint32_t count);
void testPinChange(void);
void testPinOn(void);
void testPinOff(void);

#endif 
