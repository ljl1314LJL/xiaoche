/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         bsp_linewalking.h
* @author       liusen
* @version      V1.0
* @date         2015.01.03
* @brief        巡线驱动头文件
* @details      
* @par History  见如下说明
*                 
* version:	liusen_20170717
*/

/*

	从车身后面往前看： 左侧到右边巡线传感器顺序为  L1 L2 |黑线| R1  R2	  对应原理图 IN2 IN1 |黑线| IN3 IN4

*/

#ifndef __LINEWALKING_H__
#define __LINEWALKING_H__	


void LineWalking_GPIO_Init(void);	//巡线传感器初始化

void bsp_GetLineWalking(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2);//获取巡线传感器状态

void app_LineWalking(void);//巡线函数




#endif
