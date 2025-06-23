/**
  ******************************************************************************
  * @file    main.c 
  * @author  cecelmx@126.com
  * @version 
  * @date    2025/04/08
  * @brief   智能小车程序入口文件

  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h" //必须包含
#include <stdio.h>
#include "delay.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


#include "sr04.h"		//超声波测距头
#include "smartcar.h"	//小车控制头文件
#include "dht11.h"		//温湿度传感器头文件
#include "SG90.h"		//舵机头文件
#include "linewalking.h"//巡线传感器头文件


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

#define BUFFERSIZE 100			 //接收缓冲区容量

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t aRxBuffer [BUFFERSIZE];  //接收缓冲区
__IO uint8_t ubRxIndex = 0x00;	 //接收计数器

uint8_t aRxBuffer1 [BUFFERSIZE];  //接收缓冲区
__IO uint8_t ubRxIndex1 = 0x00;	 //接收计数器

//温湿度数据缓存区
uint8_t dhtbuf[5] ={0}; 

//定义超声波距离的全局变量
int distance=25;
int distance_Count=0;//定时器6中断1毫秒触发一次，这个变量让距离测量的数值每隔100次中断更新一次
int distance_Count1=0;//定时器6中断1毫秒触发一次，这个变量让距离测量的数值每隔1000次中断更新一次


//四路巡线状态指针
int p_iL1;//左前巡线传感器状态
int p_iL2;//左后巡线传感器状态
int p_iR1;//右前巡线传感器状态
int p_iR2;//右后巡线传感器状态

//是否开启避障模式的标志位
int isAvoidance = 0;//0表示不开启，1表示开启
//是否开启循迹模式的标志位
int isLineWalk = 0;//0表示不开启，1表示开启
//前进标志位
int isGo = 0;//0表示不在前进，1表示在前进
//后退标志位
int isBack = 0;//0表示不在后退，1表示在后退

//舵机上下移动度数
int servoAngle = 90;
//舵机左右移动度数
int servoAngle1 = 90;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

//对该函数进行重定向
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch); //每次发1字节
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待字节发送完
	
	return ch;
}
void _sys_exit(int return_code) 
{
	label: goto label; /* endless loop */
}




//和pc端通信的串口配置函数
void UART1_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.打开GPIOA端口的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
    //2.打开UART1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
	//3.选择IO口的复用功能
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
  
	//4.配置GPIOA引脚参数 + 初始化GPIO端口
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		//复用模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    //5.配置UART串口参数 + 初始化UART串口
	USART_InitStructure.USART_BaudRate = baud;						//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //全双工
	USART_Init(USART1, &USART_InitStructure);
  
  
	//6.配置NVIC中断参数
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.选择UART1的中断源  接收到数据则触发中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	//8.使能UART1
	USART_Cmd(USART1, ENABLE);

}

//UART1发送字符串 
void UART1_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART1,*str++); //每次发1字节
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}

}








//和蓝牙通信的串口
void UART3_Config(u32 baud)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. 打开对应GPIO端口的时钟（以PB10/PB11为例，USART3默认引脚为PB10/TX, PB11/RX）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // PB10/PB11属于GPIOB

    // 2. 打开USART3的时钟（USART3属于APB1总线）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // 3. 选择IO口的复用功能（USART3_TX/RX对应PB10/PB11）
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);  // PB10复用为USART3_TX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);  // PB11复用为USART3_RX

    // 4. 配置GPIOB引脚参数
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 高速
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // TX引脚推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // RX引脚上拉（增强抗干扰）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; // 选择PB10和PB11
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 5. 配置USART3串口参数（与USART2相同）
    USART_InitStructure.USART_BaudRate = baud;                  // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  // 8位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      // 1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式
    USART_Init(USART3, &USART_InitStructure); // 注意改为USART3

    // 6. 配置NVIC中断（USART3中断）
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; // 改为USART3_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 7. 使能接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 改为USART3

    // 8. 使能USART3
    USART_Cmd(USART3, ENABLE); // 改为USART3
}



//UART3发送字符串 
void UART3_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART3,*str++); //每次发1字节
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}
}








//获取巡线传感器的状态
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











//DHT11_ReadData(dhtbuf);//读取DHT11的数据，并存入dhtbuf数组中
//sg_angle(0);	//舵机角度
//bsp_GetLineWalking(&p_iL1, &p_iL2, &p_iR1, &p_iR2);	//获取巡线传感器的状态
//Get_Sr04_Value();	//获取超声波模块的距离值
//USART_ReceiveData(USART3); //接收蓝牙发送的数据



int main(void)
{
	

	//1.硬件初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //中断优先级分组 4bit都提供给抢占优先级
	UART1_Config(9600);			//初始化UART1  和PC端通信
	UART3_Config(9600);				//初始化UART3  和蓝牙通信
	Sr04_Init();					//初始化超声波模块
	//TIM6_Config();					//初始化定时器6 用来完成操作 
	Motor_Init();					//初始化电机模块
	DHT11_Config();					//DHT11的初始化
	SG90_Config();					//舵机初始化
	SG90_Init();					//舵机初始化
	SG90_Init_PA6();
	LineWalking_GPIO_Init();		//初始化寻线模块的IO口

	//UART3_SendString("AT\r\n");
	//Move_Forward();	//小车向前移动
	SG90_SetAngle(90);	//舵机角度
	SG90_SetAngle_PA6(90);	//舵机角度


	
	while(1)
	{

	


		//判断是否进行循迹模式
		if(isLineWalk==1)
		{
			//获取巡线传感器的状态
			bsp_GetLineWalking(&p_iL1, &p_iL2, &p_iR1, &p_iR2);
			//判断是否需要转弯
			if (p_iL1==0 && p_iL2==1 && p_iR1==1 && p_iR2==0)//前进
			{
				Trace_Forward(-40, -40);//循迹前进	
			}
			else if (p_iL1==1 && p_iL2==0 && p_iR1==0 && p_iR2==0)//左转
			{
				Trace_Forward(30, -100);	//循迹左转
			}
			else if (p_iL1==0 && p_iL2==0 && p_iR1==0 && p_iR2==1)//右转
			{
				Trace_Forward(-100, 30);	//循迹右转
			}
			else if (p_iL1==0 && p_iL2==0 && p_iR1==0 && p_iR2==0)//前进
			{
				Trace_Forward(-40, -40);//循迹前进
			}
			else if (p_iL1==1 && p_iL2==1 && p_iR1==0 && p_iR2==0)//左转
			{
				Trace_Forward(0, -100);	//循迹左转
			}
			else if (p_iL1==0 && p_iL2==0 && p_iR1==1 && p_iR2==1)//右转
			{
				Trace_Forward(-100, 0);	//循迹右转
			}
			else if (p_iL1==1 && p_iL2==1 && p_iR1==1 && p_iR2==0)//左转
			{
				Trace_Forward(0, -100);	//循迹左转
			}
			else if (p_iL1==0 && p_iL2==1 && p_iR1==1 && p_iR2==1)//右转
			{
				Trace_Forward(-100, 0);	//循迹右转
			}
			if (isLineWalk==0)
			{
				Car_Stop();	//停止
			}
			
					
		}
		if(isAvoidance==1)//判断是否进行避障模式
		{
			//关闭定时器6，防止干扰寻线模式
			TIM_Cmd(TIM6, DISABLE);

			Move_Forward(-70);	//小车向前移动
			distance_Count++;	//定时器6中断1毫秒触发一次，这个变量让距离测量的数值每隔500次中断更新一次
			if (distance_Count == 100)
			{
				distance=Get_Sr04_Value();	//获取超声波模块的距离值
				//printf("distance:%d\n", distance);	//打印距离值
				distance_Count=0;	//重置计数器，下一次中断到来时再更新距离值
			}
			if(distance<20)
			{
				Car_Stop();	//停止
				//开始执行避障动作
					sg_angle(180);	//舵机角度
					delay_ms(100);	
					distance=Get_Sr04_Value();	//获取超声波模块的距离值
					delay_ms(500);	//延时500毫秒
					if (distance>20)
					{
						sg_angle(180);	//舵机角度
						distance=Get_Sr04_Value();	//获取超声波模块的距离值
						delay_ms(500);	//延时500毫秒
						if (distance>20)
						{
							//左转
							Turn_Left();	//小车向左转弯
							delay_ms(600);
						}
					}
					distance=Get_Sr04_Value();	//获取超声波模块的距离值
					delay_ms(500);	//延时500毫秒
					if (distance<20)
					{
						sg_angle(90);	//舵机角度
						sg_angle(0);	//舵机角度
						delay_ms(500);	//延时100毫秒
						sg_angle(0);	//舵机角度
						distance=Get_Sr04_Value();	//获取超声波模块的距离值
						delay_ms(500);	//延时500毫秒
						if (distance>20)
						{
							//右转
							Turn_Right();	//小车向右转弯
							delay_ms(600);
						}
						else
						{
							//后退
							Move_Backward();	//小车向后移动
							delay_ms(1000);	//延时500毫秒
							//右转
							Turn_Right();	//小车向右转弯
							delay_ms(600);
						}
					}
					
				sg_angle(90);	//舵机角度	
				//执行完避障动作后，继续前进
				Move_Forward(-70);	//小车向前移动
			}
			//重新开启定时器6
			TIM_Cmd(TIM6, ENABLE);
		}
		
	}
}

























/**
* @brief  This function handles USRAT1 interrupt request.
* @param  None
* @retval None
*/
void USART1_IRQHandler(void)
{
	//判断中断是否发生
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//判断当前接收缓冲区是否已满
		if (ubRxIndex1 < BUFFERSIZE)
		{
			//把接收到的字节存储到缓冲区
			aRxBuffer1[ubRxIndex1++] = USART_ReceiveData(USART1);

			if(strstr((char *)aRxBuffer1,"bz")!=NULL) //如果收到"bz"，则打开避障模式
			{
				isLineWalk = 0; //关闭寻线
				isAvoidance = 1; // 开启避障
				Move_Forward(-70); // 开启避障时小车继续前进
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
				
			}
			else if(strstr((char *)aRxBuffer1,"zb")!=NULL) //如果收到"zb"，则关闭避障模式
			{
				isAvoidance = 0; //关闭避障模式
				Car_Stop(); //关闭避障时小车停止，等待下一步指令
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"xj")!=NULL) //如果收到"xl"，则打开寻线模式
			{
				isLineWalk = 1; //开启寻线
				isAvoidance = 0; //关闭避障
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"jx")!=NULL) //如果收到"jx"，则关闭寻线模式
			{
				isLineWalk = 0; //关闭寻线
				Car_Stop(); //关闭寻线时小车停止，等待下一步指令
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"qjl")!=NULL) //如果收到"qjl"，则小车后退
			{
				//小车前进
				Trace_Forward(-70, -70);//循迹前进
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"htl")!=NULL) 
			{
				//小车后退
				Trace_Forward(70, 70);//后退
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"tz")!=NULL) //如果收到"z"，则小车停止
			{
				//小车停止
				Car_Stop(); //停止
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"yzl")!=NULL) //如果收到"yz"，则小车youzou转弯
			{
				//小车右转
				Turn_Right(); //右转
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"zzl")!=NULL) //如果收到"zyl"，则小车zuozou转弯
			{
				//小车左转
				Turn_Left(); //左转
				//清楚缓冲区数据
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"gwsxt")!=NULL) //
			{
			    SG90_SetAngle(90);	//舵机角度
				SG90_SetAngle_PA6(90);	//舵机角度
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}


			
		}
	}

}










/**
* @brief  This function handles USRAT3 interrupt request.
* @param  None
* @retval None
*/
//蓝牙遥控小车的遥控指令格式为：{aaa}、{bbb}、{ccc}、{ddd}、{eee}，其中aaa表示前进，bbb表示后退，ccc表示右转，ddd表示左转，eee表示停止。
void USART3_IRQHandler(void)
{
	//判断中断是否发生
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
			//把接收到的字节存储到缓冲区
			aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART3);
			//为了测试是否收到数据，则可以把接收的数据发送出去
			//USART_SendData(USART3, aRxBuffer[ubRxIndex-1]); //发送数据
			//if (USART_ReceiveData(USART3) == '}')
			if(strstr((char*)aRxBuffer,"u")!=NULL)
			{
				Car_Stop();	//停止
				UART1_SendString("tz");	//停止
				//清除缓冲区
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			
			}



			if(strstr((char*)aRxBuffer,"press")!=NULL)
			{
				if(strstr((char*)aRxBuffer,"aaa")!=NULL)//小车向前移动
				{
					Move_Forward(-100);	//小车向前移动 
					UART1_SendString("qj");	//向前
					//执行相应的操作
					//printf("find 111\r\n");
				}
				else if(strstr((char*)aRxBuffer,"eee")!=NULL)//小车停止移动
				{
					Car_Stop();	//停止
					UART1_SendString("tz");	//停止
					//执行相应的操作
					//printf("find 222\r\n");
				}
				else if(strstr((char*)aRxBuffer,"bbb")!=NULL)//小车向后移动
				{
					Move_Forward(100);	//小车向后移动 
					UART1_SendString("ht");	//后退

					//printf("find 333\r\n");
				}
				else if(strstr((char*)aRxBuffer,"ccc")!=NULL)//小车向右转弯
				{
					Turn_Right();	//小车向右转
					UART1_SendString("yz");
					//执行相应的操作
					//printf("find 444\r\n");
				}
				else if(strstr((char*)aRxBuffer,"www")!=NULL)//小车向左转弯
				{
					Turn_Left();	//小车向左转
					UART1_SendString("zz");
					//执行相应的操作
					//printf("find 555\r\n");

				}
				//清除缓冲区
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}






			if (strstr((char*)aRxBuffer,"}")!=NULL)
			{
				if(strstr((char*)aRxBuffer,"aaa")!=NULL)//小车向前移动
				{
					Move_Forward(-70);	//小车向前移动 
					UART1_SendString("qj");	//向前
					//执行相应的操作
					//printf("find 111\r\n");

					//SG90_SetAngle(90);	//舵机角度90°

				}
				else if(strstr((char*)aRxBuffer,"bbb")!=NULL)//小车后退移动
				{
					Move_Backward();	//小车向后移动 
					UART1_SendString("ht");	//后退
				
				}
				else if(strstr((char*)aRxBuffer,"eee")!=NULL)//小车停止移动
				{
					Car_Stop();	//停止
					UART1_SendString("tz");	//停止
					//执行相应的操作
					//printf("find 222\r\n");
				}
				else if(strstr((char*)aRxBuffer,"wsd")!=NULL)//测量温湿度
				{
					//温湿度测量
					DHT11_ReadData(dhtbuf);//读取DHT11的数据，并存入dhtbuf数组中
					char dht_str[100] = {0};//定义一个字符串数组，用于存放温湿度数据
					sprintf(dht_str, "\r\n温度：%d℃ 湿度：%d%%\r\n", dhtbuf[2], dhtbuf[0]);//将温湿度数据格式化为字符串，并存入dht_str数组中
					UART3_SendString(dht_str);//通过串口3发送温湿度数据
					if (dhtbuf[2] == 25)//如果温度为25度，则执行相应的操作
					{
						UART1_SendString("25");
					}
					else if (dhtbuf[2] == 26)//如果温度为26度，则执行相应的操作
					{
						UART1_SendString("26");
					}
					else if (dhtbuf[2] == 27)//如果温度为27度，则执行相应的操作
					{
						UART1_SendString("27");
					}
					else if (dhtbuf[2] == 28)//如果温度为28度，则执行相应的操作
					{
						UART1_SendString("28");
					}
					else if (dhtbuf[2] == 29)//如果温度为29度，则执行相应的操作
					{
						UART1_SendString("29");
					}
					else if (dhtbuf[2] == 30)//如果温度为30度，则执行相应的操作
					{
						UART1_SendString("30");
					}
					else if (dhtbuf[2] == 31)//如果温度为31度，则执行相应的操作
					{
						UART1_SendString("31");
					}
					else if (dhtbuf[2] == 32)//如果温度为32度，则执行相应的操作
					{
						UART1_SendString("32");
					}
					else if (dhtbuf[2] == 33)//如果温度为33度，则执行相应的操作
					{
						UART1_SendString("33");
					}
					else if (dhtbuf[2] == 34)//如果温度为34度，则执行相应的操作
					{
						UART1_SendString("34");
					}
					
				}
				//清除缓冲区
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			else if(strstr((char*)aRxBuffer,"qaz")!=NULL)
			{

			    //舵机上升30度
				servoAngle-=30;
				if(servoAngle<30)
				{
					servoAngle=30;
				}
				SG90_SetAngle(servoAngle);	//舵机角度servoAngle°
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			else if(strstr((char*)aRxBuffer,"edc")!=NULL)//舵机下降30度
			{
				servoAngle+=30;
				if(servoAngle>150)
				{
					servoAngle=150;
				}
				SG90_SetAngle(servoAngle);	//舵机角度servoAngle°
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			else if(strstr((char*)aRxBuffer,"wsx")!=NULL)//舵机左移30度
			{
				servoAngle1+=30;
				if(servoAngle1>180)
				{
					servoAngle1=180;
				}
				SG90_SetAngle_PA6(servoAngle1);	//舵机角度servoAngle°
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			else if(strstr((char*)aRxBuffer,"rfv")!=NULL)//舵机右移30度
			{
				servoAngle1-=30;
				if(servoAngle1<0)
				{
					servoAngle1=0;
				}
				SG90_SetAngle_PA6(servoAngle1);	//舵机角度servoAngle°
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			


			if(strstr((char*)aRxBuffer,"qqq")!=NULL)//小车开启避障
			{
				isAvoidance = !isAvoidance;  // 标志位反转
       			if (isAvoidance) 
				{
					isLineWalk = 0;        // 关闭寻线
					UART1_SendString("bz");    // 开启避障
					Move_Forward(-70);     // 开启避障时前进
				}
				else
				{
				    // 关闭避障时停止
				    Car_Stop();
					UART1_SendString("zb");    // 关闭避障
				}
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}



			if(strstr((char*)aRxBuffer,"ttt")!=NULL)//小车开启循迹
			{
				isLineWalk = !isLineWalk;  // 标志位反转
       			if (isLineWalk)
				{
					isAvoidance = 0;        // 关闭避障
					UART1_SendString("xj");    // 开启循迹
				}
				else
				{
				    // 关闭循迹时停止
				    Car_Stop();
					UART1_SendString("jx");    // 关闭循迹
				}
				//清除缓冲区
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}




			if(strstr((char*)aRxBuffer,"jli")!=NULL)//超声波测距
			{
			    distance=Get_Sr04_Value();	//获取超声波模块的距离值
				//构建距离字符串
				char distance_str[10];
				sprintf(distance_str, "距离:%d", distance);
				//printf("距离:%s\n", distance_str);	//打印距离值
				//加上/r/n换行符
				strcat(distance_str, "\r\n");
				//发送距离字符串给蓝牙
				UART3_SendString(distance_str);
				//清除缓冲区
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
				
			}





	}

}






/************************ (C) COPYRIGHT Your Name *****END OF FILE*************/
