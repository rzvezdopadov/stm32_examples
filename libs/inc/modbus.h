#ifndef  __MODBUS_H
#define __MODBUS_H

#include "stdio.h"
#include "stdint.h"

#define MB_Size_Buffer 300

// Нумератор функциональных кодов MODBUS
enum {
	MB_FC_RC   = 0x01, //		Read Coils Status(0x01)
	MB_FC_RHR  = 0x03, //		Read Holding Registers (0x03)
	MB_FC_RIR  = 0x04, //		Read Input Registers (0x04)
	MB_FC_WSC  = 0x05, //		Write Single Coil (0x05)
	MB_FC_WSR  = 0x06, //		Write Single Register (0x06)
	MB_FC_WMC  = 0x0F, //		Write Multiple Coils (0x0F)
	MB_FC_WMR  = 0x10, //		Write Multiple Registers (0x10)
	MB_FC_EM   = 0x80, //		Error Mask (0x80)
};

// Стандартные MODBUS коды ошибок
enum MB_ERR {
	MB_OK   		= 0x00, // Запрос прошел
	MB_ERR_IF   = 0x01, // Illegal Function — Принятый код функции не может быть обработан.
	MB_ERR_IDA  = 0x02, // Illegal Data Address — Адрес данных, указанный в запросе, недоступен.
	MB_ERR_IDV  = 0x03, // Illegal Data Value — Значение, содержащееся в поле данных запроса, является недопустимой величиной.
	MB_ERR_SDF  = 0x04, // Slave Device Failure — Невосстанавливаемая ошибка имела место, пока ведомое устройство пыталось выполнить затребованное действие.
	MB_ERR_ACK  = 0x05, // Acknowledge — Ведомое устройство приняло запрос и обрабатывает его, но это требует много времени.
	MB_ERR_SDB  = 0x06, // Slave Device Busy — Ведомое устройство занято обработкой команды. Ведущее устройство должно повторить сообщение позже, когда ведомое освободится.
	MB_ERR_NACK = 0x07, // Negative Acknowledge — Ведомое устройство не может выполнить программную функцию, заданную в запросе.
	MB_ERR_MPE  = 0x08  // Memory Parity Error — Ведомое устройство при чтении расширенной памяти обнаружило ошибку паритета.
};

typedef struct {								// Диапапзоны для MODBUS Write
	int32_t			lowValue;					// Нижний предел
	int32_t			highValue;				// Высокий предел
} t_MB_HoldingAcceptRange;

typedef struct {								// Буферы передачи данных
	uint8_t  rx[MB_Size_Buffer];
	uint8_t	 *rx_addr;
	uint16_t rx_count;
	uint8_t  tx[MB_Size_Buffer];
	uint8_t	 *tx_addr;
	uint16_t tx_count;
} t_MB_Buf;

typedef struct { 								// Структура для заголовка
	uint16_t startAddr;
	uint16_t sizeData;
	uint16_t biasStartAddr;
} t_MB_head_character;

uint8_t modbusTestCRC(uint8_t *addr, uint32_t sizeArray);
uint16_t modbusCalcCRC(uint8_t *addr, uint32_t sizeArray);
void modbusAddCRC(uint8_t *addr, uint32_t sizeArray);
void modbusAddByteToTxBuffer(t_MB_Buf *buf, uint8_t byte);
void modbusAddBytesToTxBuffer(t_MB_Buf *buf, uint8_t *addr, uint16_t byteCount);
void modbusAddWordToTxBuffer(t_MB_Buf *buf, uint16_t *addr, uint16_t wordCount);
void modbusCalcCRCandAddToBuf(t_MB_Buf *buf);
void modbusAddWordToCore(uint16_t *addrCore, uint16_t *addrModbus, uint16_t wordCount);
enum MB_ERR modbusRMR(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, void clbk(void));
enum MB_ERR modbusWSR(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, t_MB_HoldingAcceptRange *acceptRange, void clbk(void));
enum MB_ERR modbusWMR(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, t_MB_HoldingAcceptRange *acceptRange, void clbk(void));
enum MB_ERR modbusRMRandKey(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, uint32_t key, void clbk(void), void clbkKeyErr(void));
enum MB_ERR modbusWSRandKey(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, t_MB_HoldingAcceptRange *acceptRange, uint32_t key, void clbk(void), void clbkKeyErr(void));
enum MB_ERR modbusWMRandKey(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, t_MB_HoldingAcceptRange *acceptRange, uint32_t key, void clbk(void), void clbkKeyErr(void));
enum MB_ERR modbusRMRandDKey(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, uint32_t key1, uint32_t key2, void clbk(void), void clbkKeyErr(void));
enum MB_ERR modbusWSRandDKey(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, t_MB_HoldingAcceptRange *acceptRange, uint32_t key1, uint32_t key2, void clbk(void), void clbkKeyErr(void));
enum MB_ERR modbusWMRandDKey(t_MB_Buf *buf, uint16_t minAddrMB, uint16_t maxAddrMB, uint8_t *addrDataOnCore, t_MB_HoldingAcceptRange *acceptRange, uint32_t key1, uint32_t key2, void clbk(void), void clbkKeyErr(void));

#endif 
