#ifndef __SMARTCAR_H
#define __SMARTCAR_H

#include "stm32f10x.h" 
#include "pwm.h"
#include "motor.h"



void Move_Forward(int32_t leftSpeed, int32_t rightSpeed);
void SmartCar_Init(void);





#endif
