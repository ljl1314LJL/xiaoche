#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h" 
#include "pwm.h"
#include "Bluetooth.h"



void Motor_Init(void);
void LeftMotor_Speed(int32_t Speed);
void RightMotor_Speed(int32_t Speed);
#endif
