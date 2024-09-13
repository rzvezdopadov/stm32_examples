#ifndef  __FLASH_H
#define __FLASH_H

#include "main.h"
												
void flashEraseSector(uint8_t sectNum);												// Очистка сектора памяти
void flashWrite(uint32_t addr, uint8_t *data, uint32_t size);	// Запись данных

#endif 



