#ifndef  __TRANSFORM_H
#define __TRANSFORM_H

#include "stdint.h"

// Библиотека трансформирования данных
void transformUint16ToBuf(uint8_t *buf, uint16_t data);
void transformUint32ToBuf(uint8_t *buf, uint32_t data);

void transformUint16ToBufReverse(uint8_t *buf, uint16_t data);
void transformUint32ToBufReverse(uint8_t *buf, uint32_t data);

uint16_t transformBufToUint16(uint8_t *buf);
uint32_t transformBufToUint32(uint8_t *buf);
int16_t  transformBufToInt16(uint8_t *buf);
int32_t  transformBufToInt32(uint8_t *buf);

uint16_t transformBufToUint16Reverse(uint8_t *buf);
uint32_t transformBufToUint32Reverse(uint8_t *buf);
int16_t  transformBufToInt16Reverse(uint8_t *buf);
int32_t  transformBufToInt32Reverse(uint8_t *buf);

#endif 
