#include "Bluetooth.h"


#define BUFFERSIZE 200			 //接收缓冲区容量

// 函数来确保值在5到180之间
uint16_t limit_to_range(uint16_t value) {
    if (value >180 ) {
        return 180;
    } else if (value < 5|| value == 0) {
        return 0;
    } else {
        return value;
    }
}

uint8_t limit_to_range1(uint8_t value) 
{
    if (value > 180 || value == 180) 
    {
        return (uint8_t)180;
    } else if (value < 5 || value == 0) 
    {
        return (uint8_t)10;

    } 
    else 
    {
        return value;
    }
}


int16_t CAREMA_zuoyou = 90;
int8_t CAREMA_shangxia = 80;





uint8_t avoid_flag = 0;//避障标志位
uint8_t Auto_Line_flag = 0;//自动巡线标志位
uint8_t aRxBuffer [BUFFERSIZE];  //接收缓冲区
__IO uint8_t ubRxIndex = 0x00;	 //接收计数器
__IO uint8_t sendCounter = 0;     //发送计数器

//串口2
/*蓝牙模块通信***********************************************************************************/
void UART2_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.打开GPIOA端口 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
    //2.打开UART2的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  
	//3.配置PA2引脚参数(RXD)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置PA3(TXD)为浮空输入或上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    // 或使用 GPIO_Mode_IPU 上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    //5.配置UART串口参数 + 初始化UART串口
	USART_InitStructure.USART_BaudRate = baud;						//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //全双工
	USART_Init(USART2, &USART_InitStructure);
  
  
	//6.配置NVIC中断参数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.选择UART2的中断源  接收到数据则触发中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	
	//8.使能UART2
	USART_Cmd(USART2, ENABLE);

}


//UART2发送字符串 
void UART2_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART2,*str++); //每次发1字节
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}
}

void UART2_sendfloat(float num)
{
    char str[10];
    sprintf(str, "%.2f", num);
    UART2_SendString(str);
}


void UART3_SendChar(char ch) {
    // 发送字节
    USART_SendData(USART3, ch);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}


// 定义串口2的命令处理函数类型
typedef void (*UART2CommandHandler)(char* value);

// 串口2的命令结构（包含key和处理函数）
typedef struct {
    char key;          // 命令关键字（如 'F', 'B'）
    UART2CommandHandler handler; // 处理函数
} UART2Command_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 串口2命令处理函数
void handle_forward_cmd(char* value) {
    if (strcmp(value, "press") == 0) {
        Move_Forward(100, 100);
    } else if (strcmp(value, "pressup") == 0) {
        UART3_SendChar('j');
        Move_Forward(0, 0);
    }
}

void handle_backward_cmd(char* value) {
    if (strcmp(value, "press") == 0) {
        Move_Forward(-100, -100);
    } else if (strcmp(value, "pressup") == 0) {
        UART3_SendChar('m');
        Move_Forward(0, 0);
    }
}

void handle_left_cmd(char* value) {
    if (strcmp(value, "press") == 0) {
        Move_Forward(-100, 100);
    } else if (strcmp(value, "pressup") == 0) {
        UART3_SendChar('k');
        Move_Forward(0, 0);
    }
}

void handle_right_cmd(char* value) {
    if (strcmp(value, "press") == 0) {
        Move_Forward(100, -100);
    } else if (strcmp(value, "pressup") == 0) {
        UART3_SendChar('l');
        Move_Forward(0, 0);
    }
}

void handle_avoid_cmd(char* value) {
    if (strcmp(value, "on") == 0) {
        UART3_SendChar('f');
        avoid_flag = 1;
    } else if (strcmp(value, "pressup") == 0) {
        UART3_SendChar('g');
        avoid_flag = 0;
    }
}

void handle_line_cmd(char* value) {
    if (strcmp(value, "on") == 0) {
        UART3_SendChar('h');
        Auto_Line_flag = 1;
    } else if (strcmp(value, "pressup") == 0) {
        UART3_SendChar('i');
        Auto_Line_flag = 0;
    }
}

void handle_camera_left_cmd(char* value) {
    //UART3_SendChar('b');
    CAREMA_zuoyou += 20;
    CAREMA_zuoyou = limit_to_range(CAREMA_zuoyou);
    Servo_SetAngle7(CAREMA_zuoyou);
}

void handle_camera_right_cmd(char* value) {
    //444UART3_SendChar('d');
    CAREMA_zuoyou -= 20;
    CAREMA_zuoyou = limit_to_range(CAREMA_zuoyou);
    if (CAREMA_zuoyou == 180) CAREMA_zuoyou = 0;
    Servo_SetAngle7(CAREMA_zuoyou);
}

void handle_camera_up_cmd(char* value) {
    //UART3_SendChar('a');
    CAREMA_shangxia -= 20;
    CAREMA_shangxia = limit_to_range1(CAREMA_shangxia);
    if (CAREMA_shangxia < 0) CAREMA_shangxia = 10;
    Servo_SetAngle8(CAREMA_shangxia);
}

void handle_camera_down_cmd(char* value) {
    //UART3_SendChar('e');
    CAREMA_shangxia += 20;
    CAREMA_shangxia = limit_to_range1(CAREMA_shangxia);
    if (CAREMA_shangxia < 0) CAREMA_shangxia = 120;
    Servo_SetAngle8(CAREMA_shangxia);
}

void handle_camera_reset_cmd(char* value) {
    UART3_SendChar('c');
    CAREMA_shangxia = 80;
    CAREMA_zuoyou = 90;
    Servo_SetAngle8(CAREMA_shangxia);
    Servo_SetAngle7(CAREMA_zuoyou);
}




// 串口2命令表（key与处理函数映射）
const UART2Command_t uart2_commands[] = 
{
    {'F', handle_forward_cmd},
    {'B', handle_backward_cmd},
    {'L', handle_left_cmd},
    {'R', handle_right_cmd},
    {'b', handle_avoid_cmd},
    {'x', handle_line_cmd},
    {'z', handle_camera_left_cmd},
    {'y', handle_camera_right_cmd},
    {'s', handle_camera_up_cmd},
    {'2', handle_camera_down_cmd}, // 注意：'2' 作为key需确保协议一致
    {'u', handle_camera_reset_cmd},
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 串口2中断函数
void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
        if (ubRxIndex < BUFFERSIZE) {
            aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART2);
            
            // 检测是否收到完整数据（以 '}' 结尾）
            if (aRxBuffer[ubRxIndex-1] == '}') {
                aRxBuffer[ubRxIndex] = '\0'; // 终止字符串
                
                // 解析key和value（假设数据格式为 "key":"value"）
                char key = 0;
                char value[10] = {0};
                int quote_count = 0;
                int value_start = 0;
                
                // 遍历查找引号位置
                for (int i = 0; i < ubRxIndex; i++) {
                    if (aRxBuffer[i] == '"') {
                        quote_count++;
                        if (quote_count == 1 && i+1 < ubRxIndex) {
                            key = aRxBuffer[i+1]; // 提取key（如 'F'）
                        } else if (quote_count == 3 && i+1 < ubRxIndex) {
                            value_start = i+1; // value起始位置
                        } else if (quote_count == 4 && value_start > 0) {
                            // 提取value（如 "press" 或 "pressup"）
                            int len = i - value_start;
                            if (len > 0 && len < sizeof(value)) {
                                strncpy(value, &aRxBuffer[value_start], len);
                                value[len] = '\0';
                            }
                            break;
                        }
                    }
                }
                
                // 根据key查找命令表并执行处理函数
                for (uint8_t i = 0; i < sizeof(uart2_commands)/sizeof(UART2Command_t); i++) {
                    if (uart2_commands[i].key == key) {
                        uart2_commands[i].handler(value); // 执行命令处理
                        break;
                    }
                }
                
                ubRxIndex = 0; // 清空缓冲区
            }
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除中断标志
    }
}




/*与电脑通信***********************************************************************************/
//串口1
#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

//对该函数进行重定向
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch); //每次发1字节
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待字节发送完

    //
	
	return ch;
}


void _sys_exit(int return_code) 
{
	label: goto label; /* endless loop */
}





void UART1_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.打开GPIOA端口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
    //2.打开UART1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
	//3.选择IO口的复用功能
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //F103最高支持50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	// RX引脚（PA10）配置为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 
    //4.配置UART串口参数 + 初始化UART串口
	USART_InitStructure.USART_BaudRate = baud;						//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //全双工
	USART_Init(USART1, &USART_InitStructure);
  
  
	//5.配置NVIC中断参数 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置优先级分组
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //F103需要设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //6.选择UART1的中断源  接收到数据则触发中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    //7.使能UART1
    USART_Cmd(USART1, ENABLE);

}


//串口1中断函数
void USART1_IRQHandler(void)
{
	//判断中断是否发生
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//判断当前接收缓冲区是否已满
		if (ubRxIndex < BUFFERSIZE)
		{
			//把接收到的字节存储到缓冲区
			aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART1);
			
			//为了测试是否收到数据，则可以把接收的数据发送出去
//			USART_SendData(USART1, aRxBuffer[ubRxIndex-1]);
		}
	}

}


//串口3
//与天问语音模块通信
void UART3_Config(u32 baud)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. 打开GPIOB端口的时钟（串口3的TX和RX引脚在GPIOB上 ）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // 2. 打开UART3的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // 3. 配置TX引脚（PB10）为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    // 复用推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // F103最高支持50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置RX引脚（PB11）为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 4. 配置UART串口参数 + 初始化UART串口
    USART_InitStructure.USART_BaudRate = baud;                     // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    // 数据位 8bit
    USART_InitStructure.USART_StopBits = USART_StopBits_1;         // 停止位 1bit
    USART_InitStructure.USART_Parity = USART_Parity_No;            // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 全双工
    USART_Init(USART3, &USART_InitStructure);

    // 5. 配置NVIC中断参数
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 设置优先级分组
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  // F103需要设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 6. 选择UART3的中断源  接收到数据则触发中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    // 7. 使能UART3
    USART_Cmd(USART3, ENABLE);
}


// 定义命令处理函数类型
typedef void (*CommandHandler)(void);

// 命令结构定义
typedef struct {
    const char* command;
    uint8_t length;
    CommandHandler handler;
} Command_t;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 命令处理函数
void handle_avoid_on(void) {
    avoid_flag = 1; // 自动避障模式开启
}

void handle_avoid_off(void) {
    avoid_flag = 0; // 自动避障模式关闭
}

void handle_line_on(void) {
    Auto_Line_flag = 1; // 自动巡线模式开启
}

void handle_line_off(void) {
    Auto_Line_flag = 0; // 自动巡线模式关闭
}

void handle_forward(void) {
    Move_Forward(100, 100); // 前进
}

void handle_backward(void) {
    Move_Forward(-100, -100); // 后退
}

void handle_left(void) {
    Move_Forward(-100, 100); // 左转
}

void handle_right(void) {
    Move_Forward(100, -100); // 右转
}

void handle_stop(void) {
    Move_Forward(0, 0); // 停止
}

void handle_camera_left(void) {
    
    CAREMA_zuoyou += 20;
    CAREMA_zuoyou = limit_to_range(CAREMA_zuoyou); // 限幅左右0到180
    Servo_SetAngle7(CAREMA_zuoyou);
    
}

void handle_camera_right(void) {
    
    CAREMA_zuoyou -= 20;
    CAREMA_zuoyou = limit_to_range(CAREMA_zuoyou); // 限幅左右0到180
    Servo_SetAngle7(CAREMA_zuoyou);
    
}

void handle_camera_up(void) {
    
    CAREMA_shangxia -= 20;
    CAREMA_shangxia = limit_to_range1(CAREMA_shangxia); // 限幅上下
    Servo_SetAngle8(CAREMA_shangxia);
   
}

void handle_camera_down(void) {
    
    CAREMA_shangxia += 20;
    CAREMA_shangxia = limit_to_range1(CAREMA_shangxia); // 限幅上下
    Servo_SetAngle8(CAREMA_shangxia);
}

void handle_camera_reset(void) {
    
    CAREMA_shangxia = 80;
    CAREMA_zuoyou = 90;
    Servo_SetAngle8(CAREMA_shangxia);
    Servo_SetAngle7(CAREMA_zuoyou);
    
}



// 命令表
const Command_t commands[] = 
{
    {"bzon", 4, handle_avoid_on},
    {"bzoff", 5, handle_avoid_off},
    {"xjon", 4, handle_line_on},
    {"xjoff", 5, handle_line_off},
    {"Q", 1, handle_forward},
    {"H", 1, handle_backward},
    {"Z", 1, handle_left},
    {"Y", 1, handle_right},
    {"T", 1, handle_stop},
    {"zy", 2, handle_camera_left},
    {"yy", 2, handle_camera_right},
    {"sy", 2, handle_camera_up},
    {"xy", 2, handle_camera_down},
    {"gwsxt", 5, handle_camera_reset},
    
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 串口3中断函数
void USART3_IRQHandler(void)
{
    // 判断中断是否发生
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        // 判断当前接收缓冲区是否已满
        if (ubRxIndex < BUFFERSIZE)
        {
            // 把接收到的字节存储到缓冲区
            aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART3);
            
            // 检查是否匹配任何命令
            for (uint8_t i = 0; i < sizeof(commands)/sizeof(Command_t); i++) {
                const Command_t* cmd = &commands[i];
                if (ubRxIndex >= cmd->length && 
                    strncmp((char*)&aRxBuffer[ubRxIndex - cmd->length], cmd->command, cmd->length) == 0) {
                    // 找到匹配命令
                    if (cmd->handler) {
                        cmd->handler(); // 执行命令处理函数
                    }
                    ubRxIndex = 0; // 清空缓冲区
                    break;
                }
            }
            
            // 缓冲区溢出处理
            if (ubRxIndex >= BUFFERSIZE) {
                ubRxIndex = 0; // 防止缓冲区溢出
            }
        }
        
        // 清除中断标志
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}



