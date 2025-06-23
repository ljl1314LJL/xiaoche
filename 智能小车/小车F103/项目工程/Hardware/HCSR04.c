 #include "HCSR04.h"


int16_t Num;


void HCSR04_Init(void)
{
    // 1. ��ʼ��TIM3��GPIOA ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2.��ʼ��GPIOA6,7
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; // �������
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6; // tiger
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD; // ��������
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7; // echo
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //3.ѡ��ʱ����Դ
    TIM_InternalClockConfig(TIM3); //72MHz

    //4.����ʱ����Ԫ
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1 ;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=60000-1; //ARR;
    TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
}


#define HCSR04_TIMEOUT_US  30000  // 30ms超时（对应约5米距离）
#define SOUND_SPEED_CM_US  0.034f // 声速（cm/μs）

float HCSR04_Distance(void) {
    uint32_t start_time, timeout;
    uint16_t pulse_width;
    float distance = -1.0f;  // 默认-1表示测量失败

    // 1. 发送20us触发脉冲（Trig: PA6）
    GPIO_ResetBits(GPIOA, GPIO_Pin_6);
    Delay_us(2);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    Delay_us(20);
    GPIO_ResetBits(GPIOA, GPIO_Pin_6);

    // 2. 等待Echo引脚（PA7）变高，带超时检测
    timeout = 0;
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == RESET) {
        if (timeout++ > HCSR04_TIMEOUT_US) {
            return -1.0f;  // 超时返回错误
        }
        Delay_us(1);
    }

    // 3. 启动定时器TIM3（APB1总线，需预配置为1MHz计数）
    TIM3->CNT = 0;
    TIM_Cmd(TIM3, ENABLE);

    // 4. 测量高电平脉冲宽度，带超时检测
    timeout = 0;
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == SET) {
        if (timeout++ > HCSR04_TIMEOUT_US) {
            TIM_Cmd(TIM3, DISABLE);
            return -1.0f;  // 超时返回错误
        }
        Delay_us(1);
    }

    // 5. 停止定时器并计算距离
    TIM_Cmd(TIM3, DISABLE);
    pulse_width = TIM_GetCounter(TIM3);
    distance = (pulse_width * SOUND_SPEED_CM_US) / 2.0f;

    // 6. 最小测量间隔（防干扰）
    Delay_ms(60);  // 超声波模块推荐≥60ms
    return distance;
}

