#ifndef __PWM_
#define __PWM_

#include "stm32f10x.h"  


void PWM_Init(void);
void PWM_SetCompare0(uint32_t Compare);
void PWM_SetCompare1(uint32_t Compare);
void PWM_SetCompare3(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);
void PWM_SetCompare5(uint16_t Compare);
#endif
