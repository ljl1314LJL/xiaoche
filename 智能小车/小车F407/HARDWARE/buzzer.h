#ifndef  BUZZRER_H
#define  BUZZRER_H


#include "stm32f4xx.h" //必须包含
#define BEEP_PIN GPIO_Pin_8
#define BEEP_PORT GPIOF

/**
 * @brief 初始化蜂鸣器
 * 该函数初始化蜂鸣器引脚，准备播放声音。
 * @param 无
 * @return 无
 */
void BEEP_Init(void);



#endif
