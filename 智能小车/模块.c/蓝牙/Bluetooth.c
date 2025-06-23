#include "Bluetooth.h"



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






