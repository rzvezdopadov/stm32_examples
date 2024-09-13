#include "flash.h"

void flashUnlock(void) {																		// Разблокировка Flash памяти для записи
	while ((FLASH->SR & FLASH_SR_BSY) != 0);	
	
	// Проверяем память на предмет занятости
	if ((FLASH->CR & FLASH_CR_LOCK) != 0){										// Если она заблокирована
		FLASH->KEYR = 0x45670123;																// Пишем первый ключ разблокировки
		FLASH->KEYR = 0xCDEF89AB; 															// Пишем второй ключ разблокировки
	}
}

void flashEraseSector(uint8_t sectNum) {										// Очистка сектора памяти
	__disable_irq();
	flashUnlock();																						// Разблокировка памяти

	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR &= ~FLASH_CR_SNB_Msk;  													// Записываем номер сектора, который будем стирать
	FLASH->CR |= (uint32_t)(sectNum<<FLASH_CR_SNB_Pos);  			// Записываем номер сектора, который будем стирать
	FLASH->CR |= FLASH_CR_STRT;																// Запускаем старт стирания сектора

	while ((FLASH->SR & FLASH_SR_BSY) != 0); 									// Проверяем память на предмет занятости
		
	if ((FLASH->SR & FLASH_SR_EOP) != 0) { 										// Если произошла ошибка, скидываем флаг
			FLASH->SR = FLASH_SR_EOP; 
	}
	FLASH->CR = FLASH_CR_LOCK; 																// Блокируем память для записи
	
	__enable_irq();
}

void flashWrite(uint32_t addr, uint8_t *data, uint32_t size) {
	flashUnlock();

	for (uint32_t i=0;i<size;i++) {
		while ((FLASH->SR & FLASH_SR_BSY) != 0){};							// Проверяем память на предмет занятости

		FLASH->CR |= FLASH_CR_PG;																// Устанавливаем флаг записи памяти
		*(__IO uint8_t*)(addr+i) = data[i]; 										// Записываем байт

		while ((FLASH->SR & FLASH_SR_BSY) != 0); 								// Проверяем память на предмет занятости

		if ((FLASH->SR & FLASH_SR_EOP) != 0) {									// Если произошла ошибка, скидываем флаг
				FLASH->SR = FLASH_SR_EOP;
		}
		
		FLASH->CR &= ~FLASH_CR_PG; 															// Сбрасываем флаг записи памяти
	}
	
	FLASH->CR |= FLASH_CR_LOCK; 															// Блокируем память от записи
}
