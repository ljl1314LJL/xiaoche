#include "delay.h"



/**
  * @brief  微秒延时函数
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note   函数参数不能超过798915us
  */
void delay_us(__IO uint32_t nTime)
{ 
	SysTick->CTRL = 0; 					// 关闭定时器
	SysTick->LOAD = 21 * nTime - 1; 	// 设置重载值
	SysTick->VAL = 0; 					// 清空计数器
	SysTick->CTRL = 1; 					//打开定时器，并且使用21MHZ时钟源
	while ((SysTick->CTRL & 0x00010000)==0);// 等待延时时间到达
	SysTick->CTRL = 0; 					// 关闭定时器
}

/**
  * @brief  毫秒延时函数
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note
  */
void delay_ms(__IO uint32_t nTime)
{ 
	while(nTime--)
    {
        delay_us(1000);
    }
}












