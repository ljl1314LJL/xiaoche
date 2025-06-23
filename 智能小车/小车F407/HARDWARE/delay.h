#ifndef  DELAY_H
#define  DELAY_H


#include "stm32f4xx.h" //必须包含



/**
  * @brief  微秒延时函数
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note   函数参数不能超过798915us
  */
void delay_us(__IO uint32_t nTime);









/**
  * @brief  毫秒延时函数
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note
  */
void delay_ms(__IO uint32_t nTime);




#endif
