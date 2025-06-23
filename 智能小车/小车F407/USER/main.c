/**
  ******************************************************************************
  * @file    main.c 
  * @author  cecelmx@126.com
  * @version 
  * @date    2025/04/08
  * @brief   ����С����������ļ�

  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h" //�������
#include <stdio.h>
#include "delay.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


#include "sr04.h"		//���������ͷ
#include "smartcar.h"	//С������ͷ�ļ�
#include "dht11.h"		//��ʪ�ȴ�����ͷ�ļ�
#include "SG90.h"		//���ͷ�ļ�
#include "linewalking.h"//Ѳ�ߴ�����ͷ�ļ�


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

#define BUFFERSIZE 100			 //���ջ���������

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t aRxBuffer [BUFFERSIZE];  //���ջ�����
__IO uint8_t ubRxIndex = 0x00;	 //���ռ�����

uint8_t aRxBuffer1 [BUFFERSIZE];  //���ջ�����
__IO uint8_t ubRxIndex1 = 0x00;	 //���ռ�����

//��ʪ�����ݻ�����
uint8_t dhtbuf[5] ={0}; 

//���峬���������ȫ�ֱ���
int distance=25;
int distance_Count=0;//��ʱ��6�ж�1���봥��һ�Σ���������þ����������ֵÿ��100���жϸ���һ��
int distance_Count1=0;//��ʱ��6�ж�1���봥��һ�Σ���������þ����������ֵÿ��1000���жϸ���һ��


//��·Ѳ��״ָ̬��
int p_iL1;//��ǰѲ�ߴ�����״̬
int p_iL2;//���Ѳ�ߴ�����״̬
int p_iR1;//��ǰѲ�ߴ�����״̬
int p_iR2;//�Һ�Ѳ�ߴ�����״̬

//�Ƿ�������ģʽ�ı�־λ
int isAvoidance = 0;//0��ʾ��������1��ʾ����
//�Ƿ���ѭ��ģʽ�ı�־λ
int isLineWalk = 0;//0��ʾ��������1��ʾ����
//ǰ����־λ
int isGo = 0;//0��ʾ����ǰ����1��ʾ��ǰ��
//���˱�־λ
int isBack = 0;//0��ʾ���ں��ˣ�1��ʾ�ں���

//��������ƶ�����
int servoAngle = 90;
//��������ƶ�����
int servoAngle1 = 90;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

//�Ըú��������ض���
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch); //ÿ�η�1�ֽ�
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	
	return ch;
}
void _sys_exit(int return_code) 
{
	label: goto label; /* endless loop */
}




//��pc��ͨ�ŵĴ������ú���
void UART1_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.��GPIOA�˿ڵ�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
    //2.��UART1��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
	//3.ѡ��IO�ڵĸ��ù���
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
  
	//4.����GPIOA���Ų��� + ��ʼ��GPIO�˿�
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		//����ģʽ
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    //5.����UART���ڲ��� + ��ʼ��UART����
	USART_InitStructure.USART_BaudRate = baud;						//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ȫ˫��
	USART_Init(USART1, &USART_InitStructure);
  
  
	//6.����NVIC�жϲ���
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.ѡ��UART1���ж�Դ  ���յ������򴥷��ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	//8.ʹ��UART1
	USART_Cmd(USART1, ENABLE);

}

//UART1�����ַ��� 
void UART1_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART1,*str++); //ÿ�η�1�ֽ�
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}

}








//������ͨ�ŵĴ���
void UART3_Config(u32 baud)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. �򿪶�ӦGPIO�˿ڵ�ʱ�ӣ���PB10/PB11Ϊ����USART3Ĭ������ΪPB10/TX, PB11/RX��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // PB10/PB11����GPIOB

    // 2. ��USART3��ʱ�ӣ�USART3����APB1���ߣ�
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // 3. ѡ��IO�ڵĸ��ù��ܣ�USART3_TX/RX��ӦPB10/PB11��
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);  // PB10����ΪUSART3_TX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);  // PB11����ΪUSART3_RX

    // 4. ����GPIOB���Ų���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // ����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // TX�����������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // RX������������ǿ�����ţ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; // ѡ��PB10��PB11
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 5. ����USART3���ڲ�������USART2��ͬ��
    USART_InitStructure.USART_BaudRate = baud;                  // ������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  // 8λ����λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      // 1λֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;         // ��У��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // �շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); // ע���ΪUSART3

    // 6. ����NVIC�жϣ�USART3�жϣ�
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; // ��ΪUSART3_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 7. ʹ�ܽ����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ��ΪUSART3

    // 8. ʹ��USART3
    USART_Cmd(USART3, ENABLE); // ��ΪUSART3
}



//UART3�����ַ��� 
void UART3_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART3,*str++); //ÿ�η�1�ֽ�
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}
}








//��ȡѲ�ߴ�������״̬
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











//DHT11_ReadData(dhtbuf);//��ȡDHT11�����ݣ�������dhtbuf������
//sg_angle(0);	//����Ƕ�
//bsp_GetLineWalking(&p_iL1, &p_iL2, &p_iR1, &p_iR2);	//��ȡѲ�ߴ�������״̬
//Get_Sr04_Value();	//��ȡ������ģ��ľ���ֵ
//USART_ReceiveData(USART3); //�����������͵�����



int main(void)
{
	

	//1.Ӳ����ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�ж����ȼ����� 4bit���ṩ����ռ���ȼ�
	UART1_Config(9600);			//��ʼ��UART1  ��PC��ͨ��
	UART3_Config(9600);				//��ʼ��UART3  ������ͨ��
	Sr04_Init();					//��ʼ��������ģ��
	//TIM6_Config();					//��ʼ����ʱ��6 ������ɲ��� 
	Motor_Init();					//��ʼ�����ģ��
	DHT11_Config();					//DHT11�ĳ�ʼ��
	SG90_Config();					//�����ʼ��
	SG90_Init();					//�����ʼ��
	SG90_Init_PA6();
	LineWalking_GPIO_Init();		//��ʼ��Ѱ��ģ���IO��

	//UART3_SendString("AT\r\n");
	//Move_Forward();	//С����ǰ�ƶ�
	SG90_SetAngle(90);	//����Ƕ�
	SG90_SetAngle_PA6(90);	//����Ƕ�


	
	while(1)
	{

	


		//�ж��Ƿ����ѭ��ģʽ
		if(isLineWalk==1)
		{
			//��ȡѲ�ߴ�������״̬
			bsp_GetLineWalking(&p_iL1, &p_iL2, &p_iR1, &p_iR2);
			//�ж��Ƿ���Ҫת��
			if (p_iL1==0 && p_iL2==1 && p_iR1==1 && p_iR2==0)//ǰ��
			{
				Trace_Forward(-40, -40);//ѭ��ǰ��	
			}
			else if (p_iL1==1 && p_iL2==0 && p_iR1==0 && p_iR2==0)//��ת
			{
				Trace_Forward(30, -100);	//ѭ����ת
			}
			else if (p_iL1==0 && p_iL2==0 && p_iR1==0 && p_iR2==1)//��ת
			{
				Trace_Forward(-100, 30);	//ѭ����ת
			}
			else if (p_iL1==0 && p_iL2==0 && p_iR1==0 && p_iR2==0)//ǰ��
			{
				Trace_Forward(-40, -40);//ѭ��ǰ��
			}
			else if (p_iL1==1 && p_iL2==1 && p_iR1==0 && p_iR2==0)//��ת
			{
				Trace_Forward(0, -100);	//ѭ����ת
			}
			else if (p_iL1==0 && p_iL2==0 && p_iR1==1 && p_iR2==1)//��ת
			{
				Trace_Forward(-100, 0);	//ѭ����ת
			}
			else if (p_iL1==1 && p_iL2==1 && p_iR1==1 && p_iR2==0)//��ת
			{
				Trace_Forward(0, -100);	//ѭ����ת
			}
			else if (p_iL1==0 && p_iL2==1 && p_iR1==1 && p_iR2==1)//��ת
			{
				Trace_Forward(-100, 0);	//ѭ����ת
			}
			if (isLineWalk==0)
			{
				Car_Stop();	//ֹͣ
			}
			
					
		}
		if(isAvoidance==1)//�ж��Ƿ���б���ģʽ
		{
			//�رն�ʱ��6����ֹ����Ѱ��ģʽ
			TIM_Cmd(TIM6, DISABLE);

			Move_Forward(-70);	//С����ǰ�ƶ�
			distance_Count++;	//��ʱ��6�ж�1���봥��һ�Σ���������þ����������ֵÿ��500���жϸ���һ��
			if (distance_Count == 100)
			{
				distance=Get_Sr04_Value();	//��ȡ������ģ��ľ���ֵ
				//printf("distance:%d\n", distance);	//��ӡ����ֵ
				distance_Count=0;	//���ü���������һ���жϵ���ʱ�ٸ��¾���ֵ
			}
			if(distance<20)
			{
				Car_Stop();	//ֹͣ
				//��ʼִ�б��϶���
					sg_angle(180);	//����Ƕ�
					delay_ms(100);	
					distance=Get_Sr04_Value();	//��ȡ������ģ��ľ���ֵ
					delay_ms(500);	//��ʱ500����
					if (distance>20)
					{
						sg_angle(180);	//����Ƕ�
						distance=Get_Sr04_Value();	//��ȡ������ģ��ľ���ֵ
						delay_ms(500);	//��ʱ500����
						if (distance>20)
						{
							//��ת
							Turn_Left();	//С������ת��
							delay_ms(600);
						}
					}
					distance=Get_Sr04_Value();	//��ȡ������ģ��ľ���ֵ
					delay_ms(500);	//��ʱ500����
					if (distance<20)
					{
						sg_angle(90);	//����Ƕ�
						sg_angle(0);	//����Ƕ�
						delay_ms(500);	//��ʱ100����
						sg_angle(0);	//����Ƕ�
						distance=Get_Sr04_Value();	//��ȡ������ģ��ľ���ֵ
						delay_ms(500);	//��ʱ500����
						if (distance>20)
						{
							//��ת
							Turn_Right();	//С������ת��
							delay_ms(600);
						}
						else
						{
							//����
							Move_Backward();	//С������ƶ�
							delay_ms(1000);	//��ʱ500����
							//��ת
							Turn_Right();	//С������ת��
							delay_ms(600);
						}
					}
					
				sg_angle(90);	//����Ƕ�	
				//ִ������϶����󣬼���ǰ��
				Move_Forward(-70);	//С����ǰ�ƶ�
			}
			//���¿�����ʱ��6
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
	//�ж��ж��Ƿ���
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//�жϵ�ǰ���ջ������Ƿ�����
		if (ubRxIndex1 < BUFFERSIZE)
		{
			//�ѽ��յ����ֽڴ洢��������
			aRxBuffer1[ubRxIndex1++] = USART_ReceiveData(USART1);

			if(strstr((char *)aRxBuffer1,"bz")!=NULL) //����յ�"bz"����򿪱���ģʽ
			{
				isLineWalk = 0; //�ر�Ѱ��
				isAvoidance = 1; // ��������
				Move_Forward(-70); // ��������ʱС������ǰ��
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
				
			}
			else if(strstr((char *)aRxBuffer1,"zb")!=NULL) //����յ�"zb"����رձ���ģʽ
			{
				isAvoidance = 0; //�رձ���ģʽ
				Car_Stop(); //�رձ���ʱС��ֹͣ���ȴ���һ��ָ��
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"xj")!=NULL) //����յ�"xl"�����Ѱ��ģʽ
			{
				isLineWalk = 1; //����Ѱ��
				isAvoidance = 0; //�رձ���
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"jx")!=NULL) //����յ�"jx"����ر�Ѱ��ģʽ
			{
				isLineWalk = 0; //�ر�Ѱ��
				Car_Stop(); //�ر�Ѱ��ʱС��ֹͣ���ȴ���һ��ָ��
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"qjl")!=NULL) //����յ�"qjl"����С������
			{
				//С��ǰ��
				Trace_Forward(-70, -70);//ѭ��ǰ��
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"htl")!=NULL) 
			{
				//С������
				Trace_Forward(70, 70);//����
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"tz")!=NULL) //����յ�"z"����С��ֹͣ
			{
				//С��ֹͣ
				Car_Stop(); //ֹͣ
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"yzl")!=NULL) //����յ�"yz"����С��youzouת��
			{
				//С����ת
				Turn_Right(); //��ת
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"zzl")!=NULL) //����յ�"zyl"����С��zuozouת��
			{
				//С����ת
				Turn_Left(); //��ת
				//�������������
				memset(aRxBuffer1,0,BUFFERSIZE);
				ubRxIndex1 = 0;
			}
			else if(strstr((char *)aRxBuffer1,"gwsxt")!=NULL) //
			{
			    SG90_SetAngle(90);	//����Ƕ�
				SG90_SetAngle_PA6(90);	//����Ƕ�
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
//����ң��С����ң��ָ���ʽΪ��{aaa}��{bbb}��{ccc}��{ddd}��{eee}������aaa��ʾǰ����bbb��ʾ���ˣ�ccc��ʾ��ת��ddd��ʾ��ת��eee��ʾֹͣ��
void USART3_IRQHandler(void)
{
	//�ж��ж��Ƿ���
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
			//�ѽ��յ����ֽڴ洢��������
			aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART3);
			//Ϊ�˲����Ƿ��յ����ݣ�����԰ѽ��յ����ݷ��ͳ�ȥ
			//USART_SendData(USART3, aRxBuffer[ubRxIndex-1]); //��������
			//if (USART_ReceiveData(USART3) == '}')
			if(strstr((char*)aRxBuffer,"u")!=NULL)
			{
				Car_Stop();	//ֹͣ
				UART1_SendString("tz");	//ֹͣ
				//���������
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			
			}



			if(strstr((char*)aRxBuffer,"press")!=NULL)
			{
				if(strstr((char*)aRxBuffer,"aaa")!=NULL)//С����ǰ�ƶ�
				{
					Move_Forward(-100);	//С����ǰ�ƶ� 
					UART1_SendString("qj");	//��ǰ
					//ִ����Ӧ�Ĳ���
					//printf("find 111\r\n");
				}
				else if(strstr((char*)aRxBuffer,"eee")!=NULL)//С��ֹͣ�ƶ�
				{
					Car_Stop();	//ֹͣ
					UART1_SendString("tz");	//ֹͣ
					//ִ����Ӧ�Ĳ���
					//printf("find 222\r\n");
				}
				else if(strstr((char*)aRxBuffer,"bbb")!=NULL)//С������ƶ�
				{
					Move_Forward(100);	//С������ƶ� 
					UART1_SendString("ht");	//����

					//printf("find 333\r\n");
				}
				else if(strstr((char*)aRxBuffer,"ccc")!=NULL)//С������ת��
				{
					Turn_Right();	//С������ת
					UART1_SendString("yz");
					//ִ����Ӧ�Ĳ���
					//printf("find 444\r\n");
				}
				else if(strstr((char*)aRxBuffer,"www")!=NULL)//С������ת��
				{
					Turn_Left();	//С������ת
					UART1_SendString("zz");
					//ִ����Ӧ�Ĳ���
					//printf("find 555\r\n");

				}
				//���������
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}






			if (strstr((char*)aRxBuffer,"}")!=NULL)
			{
				if(strstr((char*)aRxBuffer,"aaa")!=NULL)//С����ǰ�ƶ�
				{
					Move_Forward(-70);	//С����ǰ�ƶ� 
					UART1_SendString("qj");	//��ǰ
					//ִ����Ӧ�Ĳ���
					//printf("find 111\r\n");

					//SG90_SetAngle(90);	//����Ƕ�90��

				}
				else if(strstr((char*)aRxBuffer,"bbb")!=NULL)//С�������ƶ�
				{
					Move_Backward();	//С������ƶ� 
					UART1_SendString("ht");	//����
				
				}
				else if(strstr((char*)aRxBuffer,"eee")!=NULL)//С��ֹͣ�ƶ�
				{
					Car_Stop();	//ֹͣ
					UART1_SendString("tz");	//ֹͣ
					//ִ����Ӧ�Ĳ���
					//printf("find 222\r\n");
				}
				else if(strstr((char*)aRxBuffer,"wsd")!=NULL)//������ʪ��
				{
					//��ʪ�Ȳ���
					DHT11_ReadData(dhtbuf);//��ȡDHT11�����ݣ�������dhtbuf������
					char dht_str[100] = {0};//����һ���ַ������飬���ڴ����ʪ������
					sprintf(dht_str, "\r\n�¶ȣ�%d�� ʪ�ȣ�%d%%\r\n", dhtbuf[2], dhtbuf[0]);//����ʪ�����ݸ�ʽ��Ϊ�ַ�����������dht_str������
					UART3_SendString(dht_str);//ͨ������3������ʪ������
					if (dhtbuf[2] == 25)//����¶�Ϊ25�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("25");
					}
					else if (dhtbuf[2] == 26)//����¶�Ϊ26�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("26");
					}
					else if (dhtbuf[2] == 27)//����¶�Ϊ27�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("27");
					}
					else if (dhtbuf[2] == 28)//����¶�Ϊ28�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("28");
					}
					else if (dhtbuf[2] == 29)//����¶�Ϊ29�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("29");
					}
					else if (dhtbuf[2] == 30)//����¶�Ϊ30�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("30");
					}
					else if (dhtbuf[2] == 31)//����¶�Ϊ31�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("31");
					}
					else if (dhtbuf[2] == 32)//����¶�Ϊ32�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("32");
					}
					else if (dhtbuf[2] == 33)//����¶�Ϊ33�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("33");
					}
					else if (dhtbuf[2] == 34)//����¶�Ϊ34�ȣ���ִ����Ӧ�Ĳ���
					{
						UART1_SendString("34");
					}
					
				}
				//���������
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			else if(strstr((char*)aRxBuffer,"qaz")!=NULL)
			{

			    //�������30��
				servoAngle-=30;
				if(servoAngle<30)
				{
					servoAngle=30;
				}
				SG90_SetAngle(servoAngle);	//����Ƕ�servoAngle��
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			else if(strstr((char*)aRxBuffer,"edc")!=NULL)//����½�30��
			{
				servoAngle+=30;
				if(servoAngle>150)
				{
					servoAngle=150;
				}
				SG90_SetAngle(servoAngle);	//����Ƕ�servoAngle��
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			else if(strstr((char*)aRxBuffer,"wsx")!=NULL)//�������30��
			{
				servoAngle1+=30;
				if(servoAngle1>180)
				{
					servoAngle1=180;
				}
				SG90_SetAngle_PA6(servoAngle1);	//����Ƕ�servoAngle��
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			else if(strstr((char*)aRxBuffer,"rfv")!=NULL)//�������30��
			{
				servoAngle1-=30;
				if(servoAngle1<0)
				{
					servoAngle1=0;
				}
				SG90_SetAngle_PA6(servoAngle1);	//����Ƕ�servoAngle��
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}
			


			if(strstr((char*)aRxBuffer,"qqq")!=NULL)//С����������
			{
				isAvoidance = !isAvoidance;  // ��־λ��ת
       			if (isAvoidance) 
				{
					isLineWalk = 0;        // �ر�Ѱ��
					UART1_SendString("bz");    // ��������
					Move_Forward(-70);     // ��������ʱǰ��
				}
				else
				{
				    // �رձ���ʱֹͣ
				    Car_Stop();
					UART1_SendString("zb");    // �رձ���
				}
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}



			if(strstr((char*)aRxBuffer,"ttt")!=NULL)//С������ѭ��
			{
				isLineWalk = !isLineWalk;  // ��־λ��ת
       			if (isLineWalk)
				{
					isAvoidance = 0;        // �رձ���
					UART1_SendString("xj");    // ����ѭ��
				}
				else
				{
				    // �ر�ѭ��ʱֹͣ
				    Car_Stop();
					UART1_SendString("jx");    // �ر�ѭ��
				}
				//���������
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
			}




			if(strstr((char*)aRxBuffer,"jli")!=NULL)//���������
			{
			    distance=Get_Sr04_Value();	//��ȡ������ģ��ľ���ֵ
				//���������ַ���
				char distance_str[10];
				sprintf(distance_str, "����:%d", distance);
				//printf("����:%s\n", distance_str);	//��ӡ����ֵ
				//����/r/n���з�
				strcat(distance_str, "\r\n");
				//���;����ַ���������
				UART3_SendString(distance_str);
				//���������
				memset(aRxBuffer, 0, BUFFERSIZE);
				ubRxIndex = 0;
				
			}





	}

}






/************************ (C) COPYRIGHT Your Name *****END OF FILE*************/
