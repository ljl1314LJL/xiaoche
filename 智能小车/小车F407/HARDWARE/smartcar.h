#ifndef __SMARTCAR_H
#define __SMARTCAR_H

#include "stm32f4xx.h"


void PWM_Init(void);
void PWM_SetCompare3(uint16_t Compare);//设置通道3的比较值
void PWM_SetCompare4(uint16_t Compare);//设置通道4的比较值
void Motor_Init(void);//电机初始化函数
void LeftMotor_Speed(int8_t Speed);//左电机速度控制函数
void RightMotor_Speed(int8_t Speed);//右电机速度控制函数
void SmartCar_Init(void);//智能车初始化函数
void Move_Forward(int8_t Speed);//前进函数
void Move_Backward(void);//后退函数
void Car_Stop(void);//停止函数
void Turn_Left(void);//右转函数
void Turn_Right(void);//左转函数





void Trace_Forward(int8_t Speed1,int8_t Speed2); //循迹函数


#endif
