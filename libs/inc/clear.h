#ifndef  __CLEAR_H
#define __CLEAR_H

#include "main.h"

void clearRAM(uint8_t *addr, uint32_t size);			// Очистка памяти
void clearCPU(void); 															// Разинициализация контроллера

#endif 

