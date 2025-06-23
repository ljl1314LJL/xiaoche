#include "led.h"



/**
 * @brief 初始化LED
 *
 * 该函数用于初始化LED的GPIO引脚，设置它们为输出模式，并配置相关的GPIO参数。
 */
void led_init(void)
{
  //1.定义GPIO外设的结构体变量
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//2.打开GPIOF+GPIOE端口的外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//3.配置GPIO引脚为输出状态
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	
	//4.调用GPIO_Init()初始化外设
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  
}











