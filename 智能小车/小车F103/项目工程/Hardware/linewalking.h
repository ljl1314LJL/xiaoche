#ifndef __LINEWALKING_
#define __LINEWALKING_

#include "stm32f10x.h"  
#include "Delay.h"
#include "Bluetooth.h"
#include "smartcar.h"

#define USE_LINE_L1
#define USE_LINE_L2
#define USE_LINE_R1
#define USE_LINE_R2

#define LineWalk_L1_RCC		RCC_APB2Periph_GPIOC
#define LineWalk_L2_RCC		RCC_APB2Periph_GPIOC
#define LineWalk_R1_RCC    	RCC_APB2Periph_GPIOC
#define LineWalk_R2_RCC		RCC_APB2Periph_GPIOA

#define LineWalk_L1_PIN		GPIO_Pin_13
#define LineWalk_L2_PIN		GPIO_Pin_14
#define LineWalk_R1_PIN		GPIO_Pin_15
#define LineWalk_R2_PIN		GPIO_Pin_4

#define LineWalk_L1_PORT	GPIOC
#define LineWalk_L2_PORT	GPIOC
#define LineWalk_R1_PORT	GPIOC
#define LineWalk_R2_PORT	GPIOA

#define LOW		0
#define HIGH	1

#endif




void LineWalking_GPIO_Init(void);
void bsp_GetLineWalking(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2);
void app_LineWalking(void);


