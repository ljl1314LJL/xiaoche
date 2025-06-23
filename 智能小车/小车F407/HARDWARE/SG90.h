#ifndef __SG90_H
#define __SG90_H

#include "stm32f4xx.h"
#include "delay.h"

void SG90_Config(void);
void sg_angle(uint32_t angle);


void SG90_Init(void);
void SG90_SetAngle(uint8_t angle); //0-180


void SG90_Init_PA6(void);
void SG90_SetAngle_PA6(uint8_t angle);


#endif
