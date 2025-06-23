#include "buzzer.h"



/**
 * @brief 初始化蜂鸣器
 * 该函数初始化蜂鸣器引脚，准备播放声音。
 * @param 无
 * @return 无
 */
void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//1.打开GPIOF端口的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//2.设置PF8引脚四个参数
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
}












