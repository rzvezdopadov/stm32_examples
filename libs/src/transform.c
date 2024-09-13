#include "transform.h"

// Библиотека трансформирования данных
void transformUint16ToBuf(uint8_t *buf, uint16_t data) {
	buf[0] = (uint8_t)(data >> 8);
	buf[1] = (uint8_t)data;
}

void transformUint32ToBuf(uint8_t *buf, uint32_t data) {
	buf[0] = (uint8_t)(data >> 24);
	buf[1] = (uint8_t)(data >> 16);
	buf[2] = (uint8_t)(data >> 8);
	buf[3] = (uint8_t)data;
}

void transformUint16ToBufReverse(uint8_t *buf, uint16_t data) {
	buf[1] = (uint8_t)(data >> 8);
	buf[0] = (uint8_t)data;
}

void transformUint32ToBufReverse(uint8_t *buf, uint32_t data) {
	buf[3] = (uint8_t)(data >> 24);
	buf[2] = (uint8_t)(data >> 16);
	buf[1] = (uint8_t)(data >> 8);
	buf[0] = (uint8_t)data;
}

uint16_t transformBufToUint16(uint8_t *buf) {
	uint16_t result = (buf[0] << 8) | buf[1];
	
	return result;
}

uint32_t transformBufToUint32(uint8_t *buf) {
	uint32_t result = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	
	return result;
}

int16_t transformBufToInt16(uint8_t *buf) {
	int16_t result = (buf[0] << 8) | buf[1];
	
	return result;
}

int32_t transformBufToInt32(uint8_t *buf) {
	int32_t result = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	
	return result;
}

uint16_t transformBufToUint16Reverse(uint8_t *buf) {
	uint16_t result = (buf[1] << 8) | buf[0];
	
	return result;
}

uint32_t transformBufToUint32Reverse(uint8_t *buf) {
	uint32_t result = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
	
	return result;
}

int16_t transformBufToInt16Reverse(uint8_t *buf) {
	int16_t result = (buf[1] << 8) | buf[0];
	
	return result;
}

int32_t transformBufToInt32Reverse(uint8_t *buf) {
	int32_t result = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
	
	return result;
}
