#include "SG90.h"


/************************************
引脚说明
用的io口是PC6
************************************/
void SG90_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//1.打开GPIO端口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
	//2.配置IO口为复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
}


void sg_angle(uint32_t angle)
{
	if(angle==0)
	{
		GPIOC->ODR |= (1<<6); //输出高电平
		delay_us(500);
		
		GPIOC->ODR &=~(1<<6); //输出低电平
		delay_us(500);
		delay_ms(19);
	}

	if(angle==45)
	{
		GPIOC->ODR |= (1<<6); //输出高电平
		delay_ms(1);
		
		GPIOC->ODR &=~(1<<6); //输出低电平
		delay_ms(19);
	}


	if(angle==90)
	{
		GPIOC->ODR |= (1<<6); //输出高电平
		delay_ms(1);
		delay_us(500);
		
		GPIOC->ODR &=~(1<<6); //输出低电平
		delay_ms(18);
		delay_us(500);
	}
	
	if(angle==135)
	{
		GPIOC->ODR |= (1<<6); //输出高电平
		delay_ms(2);
		
		GPIOC->ODR &=~(1<<6); //输出低电平
		delay_ms(18);
	}	
	
	if(angle==180)
	{
		GPIOC->ODR |= (1<<6); //输出高电平
		delay_ms(2);
		delay_us(500);
		
		GPIOC->ODR &=~(1<<6); //输出低电平
		delay_ms(17);
		delay_us(500);
	}		
}
