#ifndef __SMARTCAR_H
#define __SMARTCAR_H

#include "stm32f4xx.h"


void PWM_Init(void);
void PWM_SetCompare3(uint16_t Compare);//设置通道3的比较值
void PWM_SetCompare4(uint16_t Compare);//设置通道4的比较值
void Motor_Init(void);
void LeftMotor_Speed(int8_t Speed);
void RightMotor_Speed(int8_t Speed);
void SmartCar_Init(void);
void Move_Forward(void);
void Move_Backward(void);
void Car_Stop(void);
void Turn_Left(void);
void Turn_Right(void);
void Clockwise_Rotation(void);
void CounterClockwise_Rotation(void);


#endif