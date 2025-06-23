#ifndef __DUOJI_H
#define __DUOJI_H

#include "stm32f10x.h"
#include "Delay.h"
#include "pwm.h"


void Servo_Init(void);
void Servo_SetAngle(float Angle) ;
void SG90_Config(void);
void Servo_180(void);
void Servo_90(void);
void Servo_0(void);
void Servo_SetAngle8(float Angle);
void Servo_SetAngle7(float Angle);





#endif
