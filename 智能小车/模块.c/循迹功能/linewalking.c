/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         bsp_linewalking.c	
* @author       liusen
* @version      V1.0
* @date         2017.07.17
* @brief        Ѳ������Դ�ļ�
* @details      
* @par History  ������˵��
*                 
* version:		liusen_20170717
*/

#include "linewalking.h"
#include "stm32f10x.h"




//��ʼ��Ѳ��GPIO�˿�PC9 PC11 PE5 PE6��Ϊ����ģʽ������
/**
 * @brief ��ʼ���������ߵ�GPIO����
 * �ú������ڳ�ʼ��STM32΢�������ϵ��ض�GPIO���ţ���֧�����߹��ܡ�
 * ���岽�����£�
 * 1. ʹ��GPIOC��GPIOE��ʱ�ӣ�STM32F4ʹ��AHB1���ߣ���
 * 2. ����PC9��PC10Ϊ��������ģʽ�����ڼ�����ߴ��������źš�
 * 3. ����PE5��PE6Ϊ��������ģʽ��ͬ�����ڼ�����ߴ��������źš�
 * @return �޷���ֵ
 */
void LineWalking_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��GPIOC��GPIOE��ʱ�ӣ�STM32F4ʹ��AHB1���ߣ�
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
    
    // ����PC9��PC10Ϊ��������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;          // ����ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // I/O����
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // ����PE5��PE6Ϊ��������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}





/**
* @function       bsp_GetLineWalking
* @author         liusen  
* @brief          ��ȡѲ�ߴ�����״̬��PC9, PC11, PE5, PE6��
* @param[in]      int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2  ��·Ѳ��״ָ̬��
* @param[out]     void
* @retval         void
* @par History    ����STM32F4��ʹ�ñ�׼��GPIO��ȡ�ӿ�
*/
void bsp_GetLineWalking(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2)
{
    // ���贫�����������£�
    // L1 -> PC9, L2 -> PC11, R1 -> PE5, R2 -> PE6

    // ��ȡPC9��L1��״̬����������ʱ����⵽��������͵�ƽ��0��
    *p_iL1 = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
    
    // ��ȡPC11��L2��״̬
    *p_iL2 = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
    
    // ��ȡPE5��R1��״̬
    *p_iR1 = !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5);
    
    // ��ȡPE6��R2��״̬
    *p_iR2 = !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6);
}








*/
/**
 * @brief ֱ�����ߺ���
 *
 * �ú������ݺ��ߴ������ļ����������С������ʻ����
 *
 * @param ��
 *
 * @return ��
 */
void app_LineWalking(void)











