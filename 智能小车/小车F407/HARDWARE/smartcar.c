#include "smartcar.h"


//使用PA2和PA3作为PWM的输出
/**
 * @brief 初始化PWM模块
 *
 * 该函数初始化PWM模块，配置TIM2定时器以及对应的GPIO引脚，使其能够生成PWM信号。
 */
void PWM_Init(void)
{
    // 声明所有变量（移至函数开头以避免C89/C90错误）
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    //1.开启TIM2的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM2
    //打开GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //GPIOA

    // 2. 配置GPIO为复用功能
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // PA2和PA3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 将IO口复用为定时器功能
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2); // PA2复用为TIM2_CH3
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2); // PA3复用为TIM2_CH4

    // 4. 选择时基单元的时钟（内部时钟）
    TIM_InternalClockConfig(TIM2); // 84MHz

    // 5. 配置时基单元
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;       // ARR=99
    TIM_TimeBaseInitStructure.TIM_Prescaler = 42 - 1;     // 84MHz / 42 = 2MHz -> PWM频率 = 2MHz / 100 = 20kHz
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    // 6. 初始化输出比较单元（OC） - 通道3 (PA2)
    TIM_OCStructInit(&TIM_OCInitStructure); // 设置默认值
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比为0%
    TIM_OC3Init(TIM2, &TIM_OCInitStructure); // 通道3 (PA2)

    // 7. 初始化输出比较单元（OC） - 通道4 (PA3)
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比为0%
    TIM_OC4Init(TIM2, &TIM_OCInitStructure); // 通道4 (PA3)

    // 8. 使能预装载寄存器
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // 9. 使能自动重载预装载寄存器
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    // 10. 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief 设置通道3的比较值
 *
 * 设置TIM2定时器通道3的比较值，用于生成PWM信号。
 *
 * @param Compare 要设置的比较值
 */
void PWM_SetCompare3(uint16_t Compare)//设置通道3的比较值
{
    TIM_SetCompare3(TIM2, Compare); 
}

/**
 * @brief 设置通道4的比较值
 *
 * 该函数用于设置PWM定时器（TIM2）通道4的比较值。
 *
 * @param Compare 要设置的比较值
 */
void PWM_SetCompare4(uint16_t Compare)//设置通道4的比较值
{
    TIM_SetCompare4(TIM2, Compare); 
}

/**
 * @brief 初始化电机控制
 *
 * 该函数用于初始化电机控制引脚及PWM输出，配置GPIOC和GPIOD的相关引脚为输出模式，并初始化PWM输出。
 *
 * @return 无返回值
 */
void Motor_Init(void)
{
    // 使能GPIOC时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // 配置电机控制引脚 (IN1, IN2)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     // 输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // 推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 设置初始状态为停止
    GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_8);


	// 初始化PD6和PD7为，
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     // 输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // 推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD, GPIO_Pin_6 | GPIO_Pin_7);


    // 初始化PWM输出
    PWM_Init();
}

//设置左侧电机的速度
void LeftMotor_Speed(int8_t Speed)
{
    if(Speed>0)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_6);
        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
        PWM_SetCompare3(Speed);
    }
    else if(Speed==0)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_6);
        GPIO_SetBits(GPIOC,GPIO_Pin_8);
        PWM_SetCompare3(Speed);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_6);
        GPIO_SetBits(GPIOC,GPIO_Pin_8);
        PWM_SetCompare3(-Speed);// CCR的值为正，所以Speed前面加负号
    }
}

//设置右侧电机的速度
void RightMotor_Speed(int8_t Speed)
{
    if(Speed>0)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_6);
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);
        PWM_SetCompare4(Speed);
    }
    else if(Speed==0)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_6);
        GPIO_SetBits(GPIOD,GPIO_Pin_7);
        PWM_SetCompare4(Speed);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_6);
        GPIO_SetBits(GPIOD,GPIO_Pin_7);
        PWM_SetCompare4(-Speed);// CCR的值为正，所以Speed前面加负号
    }
}
             
//初始化智能小车
void SmartCar_Init(void)
{
    Motor_Init();
}
//向前移动
void Move_Forward(int8_t Speed)
{
    LeftMotor_Speed(Speed);
    RightMotor_Speed(Speed);
}
//向后移动
void Move_Backward(void)
{
    LeftMotor_Speed(70);
    RightMotor_Speed(70);
}
//停止
void Car_Stop(void)
{
    LeftMotor_Speed(0);
    RightMotor_Speed(0);
}
//向左转
void Turn_Left(void)
{
    LeftMotor_Speed(0);
    RightMotor_Speed(100);
}
//向右转
void Turn_Right(void)
{
    LeftMotor_Speed(100);
    RightMotor_Speed(0);
}





////////////////////////////////////////////////////////
/**
 * @brief 控制小车函数
 *
 * 该函数用于控制小车以指定速度前进或者后退，正为后退，负为前进。
 *
 * @param Speed 前进速度，取值范围为 [-100, 100]。正数表示前进，负数表示后退，0表示停止。
 */
void Trace_Forward(int8_t Speed1,int8_t Speed2)
{
    LeftMotor_Speed(Speed1);
    RightMotor_Speed(Speed2);
}
