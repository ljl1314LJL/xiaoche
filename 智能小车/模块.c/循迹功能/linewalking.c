/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         bsp_linewalking.c	
* @author       liusen
* @version      V1.0
* @date         2017.07.17
* @brief        巡线驱动源文件
* @details      
* @par History  见如下说明
*                 
* version:		liusen_20170717
*/

#include "linewalking.h"
#include "stm32f10x.h"




//初始化巡线GPIO端口PC9 PC11 PE5 PE6都为输入模式，上拉
/**
 * @brief 初始化用于行走的GPIO引脚
 * 该函数用于初始化STM32微控制器上的特定GPIO引脚，以支持行走功能。
 * 具体步骤如下：
 * 1. 使能GPIOC和GPIOE的时钟（STM32F4使用AHB1总线）。
 * 2. 配置PC9和PC10为上拉输入模式，用于检测行走传感器的信号。
 * 3. 配置PE5和PE6为上拉输入模式，同样用于检测行走传感器的信号。
 * @return 无返回值
 */
void LineWalking_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOC和GPIOE的时钟（STM32F4使用AHB1总线）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
    
    // 配置PC9和PC10为上拉输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;          // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          // 上拉电阻
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // I/O速率
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // 配置PE5和PE6为上拉输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}





/**
* @function       bsp_GetLineWalking
* @author         liusen  
* @brief          获取巡线传感器状态（PC9, PC11, PE5, PE6）
* @param[in]      int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2  四路巡线状态指针
* @param[out]     void
* @retval         void
* @par History    适配STM32F4，使用标准库GPIO读取接口
*/
void bsp_GetLineWalking(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2)
{
    // 假设传感器连接如下：
    // L1 -> PC9, L2 -> PC11, R1 -> PE5, R2 -> PE6

    // 读取PC9（L1）状态：上拉输入时，检测到黑线输出低电平（0）
    *p_iL1 = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
    
    // 读取PC11（L2）状态
    *p_iL2 = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
    
    // 读取PE5（R1）状态
    *p_iR1 = !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5);
    
    // 读取PE6（R2）状态
    *p_iR2 = !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6);
}








*/
/**
 * @brief 直线行走函数
 *
 * 该函数根据黑线传感器的检测结果来控制小车的行驶方向。
 *
 * @param 无
 *
 * @return 无
 */
void app_LineWalking(void)











