#ifndef  BUZZRER_H
#define  BUZZRER_H


#include "stm32f4xx.h" //�������
#define BEEP_PIN GPIO_Pin_8
#define BEEP_PORT GPIOF

/**
 * @brief ��ʼ��������
 * �ú�����ʼ�����������ţ�׼������������
 * @param ��
 * @return ��
 */
void BEEP_Init(void);



#endif
