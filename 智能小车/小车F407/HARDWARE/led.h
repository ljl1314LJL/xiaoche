#ifndef  LED_H
#define  LED_H


#include "stm32f4xx.h" //必须包含



/**
 * @brief 初始化LED
 *
 * 该函数用于初始化LED的GPIO引脚，设置它们为输出模式，并配置相关的GPIO参数。
 */
void led_init(void);


#endif
