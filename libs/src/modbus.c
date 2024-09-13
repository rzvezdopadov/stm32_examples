#include "modbus.h"

/*Table of CRC values for high-order byte*/
static unsigned char auchCRCHi[] = {
	0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
	0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
	0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
	0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
	0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
	0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
	0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
	0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
	0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
	0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
	0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
	0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
	0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
	0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
	0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
	0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40
};
/*Table of CRC values for low–order byte*/
static unsigned char auchCRCLo[] = {
	0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,0x07,0xC7,0x05,0xC5,0xC4,0x04,
	0xCC,0x0C,0x0D,0xCD,0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,0x08,0xC8,
	0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,
	0x14,0xD4,0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,0x11,0xD1,0xD0,0x10,
	0xF0,0x30,0x31,0xF1,0x33,0xF3,0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
	0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,0x3B,0xFB,0x39,0xF9,0xF8,0x38,
	0x28,0xE8,0xE9,0x29,0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,0xEC,0x2C,
	0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,
	0xA0,0x60,0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,0xA5,0x65,0x64,0xA4,
	0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
	0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,
	0xB4,0x74,0x75,0xB5,0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,0x70,0xB0,
	0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,
	0x9C,0x5C,0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,0x99,0x59,0x58,0x98,
	0x88,0x48,0x49,0x89,0x4B,0x8B,0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
	0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,0x43,0x83,0x41,0x81,0x80,0x40
};
 
// Функции расчета контрольных сумм Modbus
#define modbusPolynom 0xA001
// Проверяет контрольные суммы, возвращает 1, если суммы верны
uint8_t modbusTestCRC(uint8_t *addr, uint32_t sizeArray) {
	if (sizeArray < 2) return 0;
	
	uint16_t mbCRC = modbusCalcCRC(addr, sizeArray-2);								// Вызываем расчет контрольной суммы данных из массива
	uint16_t mbCRCrecive = (uint16_t)(addr[sizeArray-1]<<8) + addr[sizeArray-2];	// Копируем контрольную сумму из имеющегося массива
	
	if (mbCRCrecive == mbCRC) {																				// Сравниваем контрольные суммы
		return 1;
	}
	
	return 0;
}

// Добавляет контрольную сумму, в хвост массива
void modbusAddCRC(uint8_t *addr, uint32_t sizeArray) {
	uint16_t mbCRC = modbusCalcCRC(addr, sizeArray); 			 						// Вызываем расчет контрольной суммы данных из массива
// Добавляем контрольную сумму в конец массива	
	addr[sizeArray] 	= (uint8_t)mbCRC; 															// Младший байт
	addr[sizeArray+1] = mbCRC>>8;																		 	// Старший байт
}

// Расчет контрольной суммы табличным методом
uint16_t modbusCalcCRC(uint8_t *addr, uint32_t sizeArray) {
	uint8_t uchCRCHi = 0xFF;  																				// Записываем начальное значение массива
  uint8_t uchCRCLo = 0xFF; 
	uint16_t uIndex; 																									// Значение счетчика
	
	for (uint32_t i=0;i<sizeArray;i++) {															// Цикл обработки данных побайтно
		uIndex = uchCRCHi ^ *addr++;																	
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
    uchCRCLo = auchCRCLo[uIndex];
	}
	
	return ((uint16_t)(uchCRCLo << 8) | uchCRCHi);
}

void modbusAddByteToTxBuffer(t_MB_Buf *buf, uint8_t byte) {
		buf->tx[buf->tx_count] = byte;
		
		buf->tx_count++;
}

void modbusAddBytesToTxBuffer(t_MB_Buf *buf, uint8_t *addr, uint16_t byteCount) {
	for (uint16_t i=0; i<byteCount; i++) {
		buf->tx[buf->tx_count] = addr[i];
		
		buf->tx_count++;
	}
}

void modbusAddWordToTxBuffer(t_MB_Buf *buf, uint16_t *addr, uint16_t wordCount) {
	for (uint16_t i=0; i<wordCount; i++) {
		buf->tx[buf->tx_count] = addr[i] >> 8;
		buf->tx[buf->tx_count + 1] = addr[i];
		buf->tx_count += 2;
	}
}

void modbusCalcCRCandAddToBuf(t_MB_Buf *buf) {
	uint16_t crc = modbusCalcCRC((uint8_t *)&buf->tx[0], buf->tx_count);
	uint8_t crcHigh = (uint8_t)(crc >> 8);
	uint8_t crcLow  = (uint8_t)crc;
	
	modbusAddByteToTxBuffer(buf, crcLow);
	modbusAddByteToTxBuffer(buf, crcHigh);
}

void modbusAddWordToCore(uint16_t *addrCore, uint16_t *addrModbus, uint16_t wordCount) {
	for (uint16_t i=0; i<wordCount; i++) {
		addrCore[i] = (addrModbus[i] << 8) | (addrModbus[i] >> 8);
	}
}

t_MB_head_character modbusGetHeadCharacter(uint8_t *buf, uint16_t minAddrMB) {
	t_MB_head_character head;
	
	head.startAddr = (buf[0] << 8) + buf[1];																	// Вытаскиваем запрашиваемые адрес начала данных 
	head.sizeData	 = (buf[2] << 8) + buf[3];																	// и количество
	head.biasStartAddr = head.startAddr - minAddrMB; 													// Приводим к базовому смещению
	
	return head;
}

enum MB_ERR modbusRMR( // Read multiple register
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, void clbk(void)
) {	// Функция отдачи данных Modbus
	t_MB_head_character head = modbusGetHeadCharacter(&buf->rx[2], minAddrMB);
	
	if (!((head.startAddr >= minAddrMB) && (head.startAddr <= maxAddrMB))) return MB_ERR_IDA;// Если адрес вне диапазона выход
	if (head.biasStartAddr + head.sizeData > (maxAddrMB - minAddrMB) + 1) return MB_ERR_IDA; // Если количество запрашиваемых данных выходит за пределы, выход
	
	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + head.biasStartAddr;
	uint8_t sizeDataNew = head.sizeData * 2;															 										// Количество регистров * 2

	modbusAddByteToTxBuffer(buf, sizeDataNew); 		 				 																		// Добавляем количество данных сколько будет в ответе
	modbusAddWordToTxBuffer(buf, startAddrOnCore, head.sizeData); 						 								// Добавляем данные
	modbusCalcCRCandAddToBuf(buf);																														// Считаем и добавляем контрольную сумму
	
	if (clbk) clbk();
	
	return MB_OK;
}

enum MB_ERR modbusWSR( // Write single register
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, 
		t_MB_HoldingAcceptRange *acceptRange, void clbk(void)
) {	// Функция отдачи данных Modbus	
	uint16_t startAddr = (buf->rx[2] << 8) + buf->rx[3];																	// Вытаскиваем желаемый адрес начала данных 
	uint16_t biasStartAddr = startAddr - minAddrMB; 																		// Приводим к базовому смещению
	uint16_t data = (buf->rx[4] << 8) | buf->rx[5]; 	// Пришедший байт данных	
	t_MB_HoldingAcceptRange *acceptRangeLocal = acceptRange + biasStartAddr;
	
	if (!((startAddr >= minAddrMB) && (startAddr <= maxAddrMB))) return MB_ERR_IDA; 		// Если адрес вне диапазона выход
	if ((acceptRangeLocal->lowValue > data) || (data > acceptRangeLocal->highValue)) return MB_ERR_IDV; // Проверка допустимых величин

	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + biasStartAddr;
	uint16_t *value = (uint16_t *)&buf->rx[4];
	
	modbusAddWordToCore(startAddrOnCore, value, 1);
	modbusAddBytesToTxBuffer(buf, &buf->rx[2], 4); 																			// Копируем данные, команда, адрес первого регистра, значение
	modbusCalcCRCandAddToBuf(buf);																											// Считаем и добавляем контрольную сумму
	
	if (clbk) clbk();
		
	return MB_OK;									
}

enum MB_ERR modbusWMR( // Write multiple register
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, 
		t_MB_HoldingAcceptRange *acceptRange, void clbk(void)
) {	// Функция отдачи данных Modbus
	t_MB_head_character head = modbusGetHeadCharacter(&buf->rx[2], minAddrMB);

	if (!((head.startAddr >= minAddrMB) && (head.startAddr <= maxAddrMB))) return MB_ERR_IDA; 		// Если адрес вне диапазона выход
	if (head.biasStartAddr + head.sizeData > ((maxAddrMB - minAddrMB) + 1)) return MB_ERR_IDA; 		// Если количество запрашиваемых данных выходит за пределы, выход
	
	for (uint16_t i=0; i<head.sizeData; i++) { 																							// Проверка допустимых величин
		uint16_t data = (buf->rx[7 + i * 2] << 8) | buf->rx[8 + i * 2]; 										// Пришедший байт данных
		t_MB_HoldingAcceptRange *acceptRangeLocal = acceptRange + head.biasStartAddr + i;
		
		if ((acceptRangeLocal->lowValue > data) || (data > acceptRangeLocal->highValue)) return MB_ERR_IDV; // Проверка допустимых величин
	}
			
	uint16_t *data = (uint16_t *)&buf->rx[7];
	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + head.biasStartAddr;
	
	modbusAddWordToCore(startAddrOnCore, data, head.sizeData);
	modbusAddBytesToTxBuffer(buf, &buf->rx[2], 2); 																			// Копируем данные, адрес первого регистра
	modbusAddBytesToTxBuffer(buf, &buf->rx[4], 2); 																			// Копируем количество регистров
	modbusCalcCRCandAddToBuf(buf);																											// Считаем и добавляем контрольную сумму

	if (clbk) clbk();
	
	return MB_OK;
}

//////////////////// С ключом управления
enum MB_ERR modbusRMRandKey( // Read multiple register
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, uint32_t key, void clbk(void), void clbkKeyErr(void)
) {	// Функция отдачи данных Modbus
	t_MB_head_character head = modbusGetHeadCharacter(&buf->rx[2], minAddrMB);
	uint8_t  mbKeyBias = 6;
	uint32_t mbKey = (buf->rx[mbKeyBias] << 24) | (buf->rx[mbKeyBias + 1] << 16) | (buf->rx[mbKeyBias + 2] << 8) | buf->rx[mbKeyBias + 3];  
	
	if (!((head.startAddr >= minAddrMB) && (head.startAddr <= maxAddrMB))) return MB_ERR_IDA;	// Если адрес вне диапазона выход
	if (head.biasStartAddr + head.sizeData > (maxAddrMB - minAddrMB) + 1) return MB_ERR_IDA; 	// Если количество запрашиваемых данных выходит за пределы, выход
	if (mbKey != key) {																																				// Если ключи управления не подошли, выход
		if (clbkKeyErr) clbkKeyErr();
		
		return MB_ERR_IF; 																											
	}
	
	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + head.biasStartAddr;
	uint8_t sizeDataNew = head.sizeData * 2;															 										// Количество регистров * 2

	modbusAddByteToTxBuffer(buf, sizeDataNew); 		 				 																		// Добавляем количество данных сколько будет в ответе
	modbusAddWordToTxBuffer(buf, startAddrOnCore, head.sizeData); 						 								// Добавляем данные
	modbusCalcCRCandAddToBuf(buf);																														// Считаем и добавляем контрольную сумму
	
	if (clbk) clbk();
		
	return MB_OK;
}

enum MB_ERR modbusWSRandKey( // Write single register
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, 
		t_MB_HoldingAcceptRange *acceptRange, uint32_t key, void clbk(void), void clbkKeyErr(void)
) {	// Функция отдачи данных Modbus	
	uint16_t startAddr = (buf->rx[2] << 8) + buf->rx[3];																// Вытаскиваем желаемый адрес начала данных 
	uint16_t biasStartAddr = startAddr - minAddrMB; 																		// Приводим к базовому смещению
	t_MB_HoldingAcceptRange *acceptRangeLocal = acceptRange + biasStartAddr;
	uint8_t  mbKeyBias = 4;
	uint32_t mbKey = (buf->rx[mbKeyBias] << 24) | (buf->rx[mbKeyBias + 1] << 16) | (buf->rx[mbKeyBias + 2] << 8) | buf->rx[mbKeyBias + 3];  
	uint16_t data = (buf->rx[8] << 8) | buf->rx[9]; 	// Пришедший байт данных	
	
	if (!((startAddr >= minAddrMB) && (startAddr <= maxAddrMB))) return MB_ERR_IDA; 		// Если адрес вне диапазона выход
	if ((acceptRangeLocal->lowValue > data) || (data > acceptRangeLocal->highValue)) return MB_ERR_IDV; // Проверка допустимых величин
	if (mbKey != key) {																																	// Если ключи управления не подошли, выход
		if (clbkKeyErr) clbkKeyErr();
		
		return MB_ERR_IF; 																								
	}
	
	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + biasStartAddr;
	uint16_t *value = (uint16_t *)&buf->rx[8];
	
	modbusAddWordToCore(startAddrOnCore, value, 1);
	modbusAddBytesToTxBuffer(buf, &buf->rx[2], 2); 																			// Копируем данные, адрес первого регистра, 
	modbusAddBytesToTxBuffer(buf, &buf->rx[8], 2); 																			// значение
	modbusCalcCRCandAddToBuf(buf);																											// Считаем и добавляем контрольную сумму
	
	if (clbk) clbk();
	
	return MB_OK;									
}

enum MB_ERR modbusWMRandKey( // Write multiple register and key
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, 
		t_MB_HoldingAcceptRange *acceptRange, uint32_t key, void clbk(void), void clbkKeyErr(void)
) {	// Функция отдачи данных Modbus
	t_MB_head_character head = modbusGetHeadCharacter(&buf->rx[2], minAddrMB);
	uint8_t  mbKeyBias = 7;
	uint32_t mbKey = (buf->rx[mbKeyBias] << 24) | (buf->rx[mbKeyBias + 1] << 16) | (buf->rx[mbKeyBias + 2] << 8) | buf->rx[mbKeyBias + 3];  
	
	if (!((head.startAddr >= minAddrMB) && (head.startAddr <= maxAddrMB))) return MB_ERR_IDA; 			// Если адрес вне диапазона выход
	if (head.biasStartAddr + head.sizeData > ((maxAddrMB - minAddrMB) + 1)) return MB_ERR_IDA; 			// Если количество запрашиваемых данных выходит за пределы, выход
	if (mbKey != key) {																																							// Если ключи управления не подошли, выход
		if (clbkKeyErr) clbkKeyErr();
		
		return MB_ERR_IF; 																														
	}
		
	for (uint16_t i=0; i<head.sizeData; i++) { 																											// Проверка допустимых величин
		uint16_t data = (buf->rx[11 + i * 2] << 8) | buf->rx[12 + i * 2]; 														// Пришедший байт данных
		t_MB_HoldingAcceptRange *acceptRangeLocal = acceptRange + head.biasStartAddr + i;
		
		if ((acceptRangeLocal->lowValue > data) || (data > acceptRangeLocal->highValue)) return MB_ERR_IDV; // Проверка допустимых величин
	}
			
	uint16_t *data = (uint16_t *)&buf->rx[11];
	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + head.biasStartAddr;
	
	modbusAddWordToCore(startAddrOnCore, data, head.sizeData);
	modbusAddBytesToTxBuffer(buf, &buf->rx[2], 2); 																			// Копируем данные, адрес первого регистра
	modbusAddBytesToTxBuffer(buf, &buf->rx[4], 2); 																			// Копируем количество регистров
	modbusCalcCRCandAddToBuf(buf);																											// Считаем и добавляем контрольную сумму

	if (clbk) clbk();
	
	return MB_OK;
}

//////////////////// С ключом управления и ключом регулировщика
enum MB_ERR modbusRMRandDKey( // Read multiple register
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, uint32_t key1, uint32_t key2, void clbk(void), void clbkKeyErr(void)
) {	// Функция отдачи данных Modbus
	t_MB_head_character head = modbusGetHeadCharacter(&buf->rx[2], minAddrMB);
	uint8_t  mbKey1Bias = 6;
	uint32_t mbKey1 = (buf->rx[mbKey1Bias] << 24) | (buf->rx[mbKey1Bias + 1] << 16) | (buf->rx[mbKey1Bias + 2] << 8) | buf->rx[mbKey1Bias + 3];
	uint8_t  mbKey2Bias = 10;
	uint32_t mbKey2 = (buf->rx[mbKey2Bias] << 24) | (buf->rx[mbKey2Bias + 1] << 16) | (buf->rx[mbKey2Bias + 2] << 8) | buf->rx[mbKey2Bias + 3];  
	
	if (!((head.startAddr >= minAddrMB) && (head.startAddr <= maxAddrMB))) return MB_ERR_IDA;	// Если адрес вне диапазона выход
	if (head.biasStartAddr + head.sizeData > (maxAddrMB - minAddrMB) + 1) return MB_ERR_IDA; 	// Если количество запрашиваемых данных выходит за пределы, выход
	if ((mbKey1 != key1) || (mbKey2 != key2)) {																								// Если ключи управления не подошли, выход
		if (clbkKeyErr) clbkKeyErr();
		
		return MB_ERR_IF;
	} 																										
		
	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + head.biasStartAddr;
	uint8_t sizeDataNew = head.sizeData * 2;															 										// Количество регистров * 2

	modbusAddByteToTxBuffer(buf, sizeDataNew); 		 				 																		// Добавляем количество данных сколько будет в ответе
	modbusAddWordToTxBuffer(buf, startAddrOnCore, head.sizeData); 						 								// Добавляем данные
	modbusCalcCRCandAddToBuf(buf);																														// Считаем и добавляем контрольную сумму
	
	if (clbk) clbk();
	
	return MB_OK;
}

enum MB_ERR modbusWSRandDKey( // Write single register
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, 
		t_MB_HoldingAcceptRange *acceptRange, uint32_t key1, uint32_t key2, void clbk(void), void clbkKeyErr(void)
) {	// Функция отдачи данных Modbus	
	uint16_t startAddr = (buf->rx[2] << 8) + buf->rx[3];																// Вытаскиваем желаемый адрес начала данных 
	uint16_t biasStartAddr = startAddr - minAddrMB; 																		// Приводим к базовому смещению
	t_MB_HoldingAcceptRange *acceptRangeLocal = acceptRange + biasStartAddr;
	uint8_t mbKey1Bias = 4;
	uint32_t mbKey1 = (buf->rx[mbKey1Bias] << 24) | (buf->rx[mbKey1Bias + 1] << 16) | (buf->rx[mbKey1Bias + 2] << 8) | buf->rx[mbKey1Bias + 3];
	uint8_t mbKey2Bias = 8;
	uint32_t mbKey2 = (buf->rx[mbKey2Bias] << 24) | (buf->rx[mbKey2Bias + 1] << 16) | (buf->rx[mbKey2Bias + 2] << 8) | buf->rx[mbKey2Bias + 3];  
	uint16_t data = (buf->rx[8] << 8) | buf->rx[9]; 																		// Пришедший байт данных	
	
	if (!((startAddr >= minAddrMB) && (startAddr <= maxAddrMB))) return MB_ERR_IDA; 		// Если адрес вне диапазона выход
	if ((acceptRangeLocal->lowValue > data) || (data > acceptRangeLocal->highValue)) return MB_ERR_IDV; // Проверка допустимых величин
	if ((mbKey1 != key1) || (mbKey2 != key2)) {																					// Если ключи управления не подошли, выход
		if (clbkKeyErr) clbkKeyErr();
		
		return MB_ERR_IF;
	} 	

	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + biasStartAddr;
	uint16_t *value = (uint16_t *)&buf->rx[12];
	
	modbusAddWordToCore(startAddrOnCore, value, 1);
	modbusAddBytesToTxBuffer(buf, &buf->rx[2], 2); 																			// Копируем данные, адрес первого регистра, 
	modbusAddBytesToTxBuffer(buf, &buf->rx[12], 2); 																		// значение
	modbusCalcCRCandAddToBuf(buf);																											// Считаем и добавляем контрольную сумму
	
	if (clbk) clbk();
		
	return MB_OK;									
}

enum MB_ERR modbusWMRandDKey( // Write multiple register and key
	t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, 
		t_MB_HoldingAcceptRange *acceptRange, uint32_t key1, uint32_t key2, void clbk(void), void clbkKeyErr(void)
) {	// Функция отдачи данных Modbus
	t_MB_head_character head = modbusGetHeadCharacter(&buf->rx[2], minAddrMB);
	uint8_t mbKey1Bias = 7;
	uint32_t mbKey1 = (buf->rx[mbKey1Bias] << 24) | (buf->rx[mbKey1Bias + 1] << 16) | (buf->rx[mbKey1Bias + 2] << 8) | buf->rx[mbKey1Bias + 3];
	uint8_t mbKey2Bias = 11;
	uint32_t mbKey2 = (buf->rx[mbKey2Bias] << 24) | (buf->rx[mbKey2Bias + 1] << 16) | (buf->rx[mbKey2Bias + 2] << 8) | buf->rx[mbKey2Bias + 3];  
	
	if (!((head.startAddr >= minAddrMB) && (head.startAddr <= maxAddrMB))) return MB_ERR_IDA; 			// Если адрес вне диапазона выход
	if (head.biasStartAddr + head.sizeData > ((maxAddrMB - minAddrMB) + 1)) return MB_ERR_IDA; 			// Если количество запрашиваемых данных выходит за пределы, выход
	if ((mbKey1 != key1) || (mbKey2 != key2)) {																					// Если ключи управления не подошли, выход
		if (clbkKeyErr) clbkKeyErr();
		
		return MB_ERR_IF;
	} 	
	
	for (uint16_t i=0; i<head.sizeData; i++) { 																											// Проверка допустимых величин
		uint16_t data = (buf->rx[15 + i * 2] << 8) | buf->rx[16 + i * 2]; 														// Пришедший байт данных
		t_MB_HoldingAcceptRange *acceptRangeLocal = acceptRange + head.biasStartAddr + i;
		
		if ((acceptRangeLocal->lowValue > data) || (data > acceptRangeLocal->highValue)) return MB_ERR_IDV; // Проверка допустимых величин
	}
			
	uint16_t *data = (uint16_t *)&buf->rx[15];
	uint16_t *startAddrOnCore = (uint16_t *)addrDataOnCore + head.biasStartAddr;
	
	modbusAddWordToCore(startAddrOnCore, data, head.sizeData);
	modbusAddBytesToTxBuffer(buf, &buf->rx[2], 2); 																			// Копируем данные, адрес первого регистра
	modbusAddBytesToTxBuffer(buf, &buf->rx[4], 2); 																			// Копируем количество регистров
	modbusCalcCRCandAddToBuf(buf);																											// Считаем и добавляем контрольную сумму

	if (clbk) clbk();
	
	return MB_OK;
}
