#include "Bluetooth.h"


#define BUFFERSIZE 200			 //���ջ���������

// ������ȷ��ֵ��5��180֮��
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





uint8_t avoid_flag = 0;//���ϱ�־λ
uint8_t Auto_Line_flag = 0;//�Զ�Ѳ�߱�־λ
uint8_t aRxBuffer [BUFFERSIZE];  //���ջ�����
__IO uint8_t ubRxIndex = 0x00;	 //���ռ�����
__IO uint8_t sendCounter = 0;     //���ͼ�����

//����2
/*����ģ��ͨ��***********************************************************************************/
void UART2_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.��GPIOA�˿� ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
    //2.��UART2��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  
	//3.����PA2���Ų���(RXD)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//��������      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. ����PA3(TXD)Ϊ�����������������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
    // ��ʹ�� GPIO_Mode_IPU ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    //5.����UART���ڲ��� + ��ʼ��UART����
	USART_InitStructure.USART_BaudRate = baud;						//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ȫ˫��
	USART_Init(USART2, &USART_InitStructure);
  
  
	//6.����NVIC�жϲ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.ѡ��UART2���ж�Դ  ���յ������򴥷��ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	
	//8.ʹ��UART2
	USART_Cmd(USART2, ENABLE);

}


//UART2�����ַ��� 
void UART2_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART2,*str++); //ÿ�η�1�ֽ�
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}
}

void UART2_sendfloat(float num)
{
    char str[10];
    sprintf(str, "%.2f", num);
    UART2_SendString(str);
}


void UART3_SendChar(char ch) {
    // �����ֽ�
    USART_SendData(USART3, ch);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}


// ���崮��2�������������
typedef void (*UART2CommandHandler)(char* value);

// ����2������ṹ������key�ʹ�������
typedef struct {
    char key;          // ����ؼ��֣��� 'F', 'B'��
    UART2CommandHandler handler; // ������
} UART2Command_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����2�������
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




// ����2�����key�봦����ӳ�䣩
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
    {'2', handle_camera_down_cmd}, // ע�⣺'2' ��Ϊkey��ȷ��Э��һ��
    {'u', handle_camera_reset_cmd},
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����2�жϺ���
void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
        if (ubRxIndex < BUFFERSIZE) {
            aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART2);
            
            // ����Ƿ��յ��������ݣ��� '}' ��β��
            if (aRxBuffer[ubRxIndex-1] == '}') {
                aRxBuffer[ubRxIndex] = '\0'; // ��ֹ�ַ���
                
                // ����key��value���������ݸ�ʽΪ "key":"value"��
                char key = 0;
                char value[10] = {0};
                int quote_count = 0;
                int value_start = 0;
                
                // ������������λ��
                for (int i = 0; i < ubRxIndex; i++) {
                    if (aRxBuffer[i] == '"') {
                        quote_count++;
                        if (quote_count == 1 && i+1 < ubRxIndex) {
                            key = aRxBuffer[i+1]; // ��ȡkey���� 'F'��
                        } else if (quote_count == 3 && i+1 < ubRxIndex) {
                            value_start = i+1; // value��ʼλ��
                        } else if (quote_count == 4 && value_start > 0) {
                            // ��ȡvalue���� "press" �� "pressup"��
                            int len = i - value_start;
                            if (len > 0 && len < sizeof(value)) {
                                strncpy(value, &aRxBuffer[value_start], len);
                                value[len] = '\0';
                            }
                            break;
                        }
                    }
                }
                
                // ����key���������ִ�д�����
                for (uint8_t i = 0; i < sizeof(uart2_commands)/sizeof(UART2Command_t); i++) {
                    if (uart2_commands[i].key == key) {
                        uart2_commands[i].handler(value); // ִ�������
                        break;
                    }
                }
                
                ubRxIndex = 0; // ��ջ�����
            }
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // ����жϱ�־
    }
}




/*�����ͨ��***********************************************************************************/
//����1
#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

//�Ըú��������ض���
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch); //ÿ�η�1�ֽ�
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����

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

	//1.��GPIOA�˿ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
    //2.��UART1��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
	//3.ѡ��IO�ڵĸ��ù���
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //F103���֧��50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	// RX���ţ�PA10������Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 
    //4.����UART���ڲ��� + ��ʼ��UART����
	USART_InitStructure.USART_BaudRate = baud;						//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ȫ˫��
	USART_Init(USART1, &USART_InitStructure);
  
  
	//5.����NVIC�жϲ��� 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�������ȼ�����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //F103��Ҫ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //6.ѡ��UART1���ж�Դ  ���յ������򴥷��ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    //7.ʹ��UART1
    USART_Cmd(USART1, ENABLE);

}


//����1�жϺ���
void USART1_IRQHandler(void)
{
	//�ж��ж��Ƿ���
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//�жϵ�ǰ���ջ������Ƿ�����
		if (ubRxIndex < BUFFERSIZE)
		{
			//�ѽ��յ����ֽڴ洢��������
			aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART1);
			
			//Ϊ�˲����Ƿ��յ����ݣ�����԰ѽ��յ����ݷ��ͳ�ȥ
//			USART_SendData(USART1, aRxBuffer[ubRxIndex-1]);
		}
	}

}


//����3
//����������ģ��ͨ��
void UART3_Config(u32 baud)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. ��GPIOB�˿ڵ�ʱ�ӣ�����3��TX��RX������GPIOB�� ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // 2. ��UART3��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // 3. ����TX���ţ�PB10��Ϊ�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    // �����������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // F103���֧��50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ����RX���ţ�PB11��Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 4. ����UART���ڲ��� + ��ʼ��UART����
    USART_InitStructure.USART_BaudRate = baud;                     // ������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    // ����λ 8bit
    USART_InitStructure.USART_StopBits = USART_StopBits_1;         // ֹͣλ 1bit
    USART_InitStructure.USART_Parity = USART_Parity_No;            // ��У��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // ȫ˫��
    USART_Init(USART3, &USART_InitStructure);

    // 5. ����NVIC�жϲ���
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // �������ȼ�����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  // F103��Ҫ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 6. ѡ��UART3���ж�Դ  ���յ������򴥷��ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    // 7. ʹ��UART3
    USART_Cmd(USART3, ENABLE);
}


// ���������������
typedef void (*CommandHandler)(void);

// ����ṹ����
typedef struct {
    const char* command;
    uint8_t length;
    CommandHandler handler;
} Command_t;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������
void handle_avoid_on(void) {
    avoid_flag = 1; // �Զ�����ģʽ����
}

void handle_avoid_off(void) {
    avoid_flag = 0; // �Զ�����ģʽ�ر�
}

void handle_line_on(void) {
    Auto_Line_flag = 1; // �Զ�Ѳ��ģʽ����
}

void handle_line_off(void) {
    Auto_Line_flag = 0; // �Զ�Ѳ��ģʽ�ر�
}

void handle_forward(void) {
    Move_Forward(100, 100); // ǰ��
}

void handle_backward(void) {
    Move_Forward(-100, -100); // ����
}

void handle_left(void) {
    Move_Forward(-100, 100); // ��ת
}

void handle_right(void) {
    Move_Forward(100, -100); // ��ת
}

void handle_stop(void) {
    Move_Forward(0, 0); // ֹͣ
}

void handle_camera_left(void) {
    
    CAREMA_zuoyou += 20;
    CAREMA_zuoyou = limit_to_range(CAREMA_zuoyou); // �޷�����0��180
    Servo_SetAngle7(CAREMA_zuoyou);
    
}

void handle_camera_right(void) {
    
    CAREMA_zuoyou -= 20;
    CAREMA_zuoyou = limit_to_range(CAREMA_zuoyou); // �޷�����0��180
    Servo_SetAngle7(CAREMA_zuoyou);
    
}

void handle_camera_up(void) {
    
    CAREMA_shangxia -= 20;
    CAREMA_shangxia = limit_to_range1(CAREMA_shangxia); // �޷�����
    Servo_SetAngle8(CAREMA_shangxia);
   
}

void handle_camera_down(void) {
    
    CAREMA_shangxia += 20;
    CAREMA_shangxia = limit_to_range1(CAREMA_shangxia); // �޷�����
    Servo_SetAngle8(CAREMA_shangxia);
}

void handle_camera_reset(void) {
    
    CAREMA_shangxia = 80;
    CAREMA_zuoyou = 90;
    Servo_SetAngle8(CAREMA_shangxia);
    Servo_SetAngle7(CAREMA_zuoyou);
    
}



// �����
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
// ����3�жϺ���
void USART3_IRQHandler(void)
{
    // �ж��ж��Ƿ���
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        // �жϵ�ǰ���ջ������Ƿ�����
        if (ubRxIndex < BUFFERSIZE)
        {
            // �ѽ��յ����ֽڴ洢��������
            aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART3);
            
            // ����Ƿ�ƥ���κ�����
            for (uint8_t i = 0; i < sizeof(commands)/sizeof(Command_t); i++) {
                const Command_t* cmd = &commands[i];
                if (ubRxIndex >= cmd->length && 
                    strncmp((char*)&aRxBuffer[ubRxIndex - cmd->length], cmd->command, cmd->length) == 0) {
                    // �ҵ�ƥ������
                    if (cmd->handler) {
                        cmd->handler(); // ִ���������
                    }
                    ubRxIndex = 0; // ��ջ�����
                    break;
                }
            }
            
            // �������������
            if (ubRxIndex >= BUFFERSIZE) {
                ubRxIndex = 0; // ��ֹ���������
            }
        }
        
        // ����жϱ�־
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}



