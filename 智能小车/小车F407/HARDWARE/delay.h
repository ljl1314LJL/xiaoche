#ifndef  DELAY_H
#define  DELAY_H


#include "stm32f4xx.h" //�������



/**
  * @brief  ΢����ʱ����
  * @param  nTime: ����ʱ��΢��ʱ��
  * @retval None
  * @note   �����������ܳ���798915us
  */
void delay_us(__IO uint32_t nTime);









/**
  * @brief  ������ʱ����
  * @param  nTime: ����ʱ��΢��ʱ��
  * @retval None
  * @note
  */
void delay_ms(__IO uint32_t nTime);




#endif
