#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"
#include "delay.h"
#include <stdbool.h>


void DHT11_PinModeSet(GPIOMode_TypeDef Mode);
void DHT11_Config(void);
void DHT11_SendStart(void);
bool DHT11_IsACK(void);
uint8_t DHT11_ReadBit(void);
uint8_t  DHT11_ReadByte(void);
bool DHT11_ReadData(uint8_t *dhtbuf);







#endif
