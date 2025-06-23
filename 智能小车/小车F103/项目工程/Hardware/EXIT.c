#include "EXIT.h"


void TIM2_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // ʹ��TIM2ʱ��

    // ����TIM2
    TIM_TimeBaseStructure.TIM_Period = arr - 1; // ���ö�ʱ����
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; // ����Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ü�����ģʽΪ���ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // ʹ��TIM2�����ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ʹ��TIM2�ж�
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE); // ʹ��TIM2
}


