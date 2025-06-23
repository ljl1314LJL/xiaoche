/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         bsp_linewalking.h
* @author       liusen
* @version      V1.0
* @date         2015.01.03
* @brief        Ѳ������ͷ�ļ�
* @details      
* @par History  ������˵��
*                 
* version:	liusen_20170717
*/

/*

	�ӳ��������ǰ���� ��ൽ�ұ�Ѳ�ߴ�����˳��Ϊ  L1 L2 |����| R1  R2	  ��Ӧԭ��ͼ IN2 IN1 |����| IN3 IN4

*/

#ifndef __LINEWALKING_H__
#define __LINEWALKING_H__	


void LineWalking_GPIO_Init(void);	//Ѳ�ߴ�������ʼ��

void bsp_GetLineWalking(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2);//��ȡѲ�ߴ�����״̬

void app_LineWalking(void);//Ѳ�ߺ���




#endif
