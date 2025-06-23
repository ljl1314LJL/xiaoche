#include "SG90.h"


/************************************
引脚说明
用的io口是PG15
************************************/
/**
 * @brief 配置SG90舵机引脚
 *
 * 该函数用于配置SG90舵机所需的GPIO引脚，包括打开GPIO端口时钟、配置IO口为输出模式等。
 */
void SG90_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//1.打开GPIO端口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  
	//2.配置IO口为复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
}


/**
 * @brief 根据给定的角度设置SG角度信号
 *
 * 该函数根据给定的角度值（angle），通过操作GPIOG的ODR寄存器来输出对应的SG角度信号。
 *
 * @param angle 输入的角度值，可能的值包括0, 45, 90, 135, 180。
 */
void sg_angle(uint32_t angle)
{
	if(angle==0)
	{
		GPIOG->ODR |= (1<<15); //输出高电平
		delay_us(500);
		
		GPIOG->ODR &=~(1<<15); //输出低电平
		delay_us(500);
		delay_ms(19);
	}

	if(angle==45)
	{
		GPIOG->ODR |= (1<<15); //输出高电平
		delay_ms(1);
		
		GPIOG->ODR &=~(1<<15); //输出低电平
		delay_ms(19);
	}


	if(angle==90)
	{
		GPIOG->ODR |= (1<<15); //输出高电平
		delay_ms(1);
		delay_us(500);
		
		GPIOG->ODR &=~(1<<15); //输出低电平
		delay_ms(18);
		delay_us(500);
	}
	
	if(angle==135)
	{
		GPIOG->ODR |= (1<<15); //输出高电平
		delay_ms(2);
		
		GPIOG->ODR &=~(1<<15); //输出低电平
		delay_ms(18);
	}	
	
	if(angle==180)
	{
		GPIOG->ODR |= (1<<15); //输出高电平
		delay_ms(2);
		delay_us(500);
		
		GPIOG->ODR &=~(1<<15); //输出低电平
		delay_ms(17);
		delay_us(500);
	}		
}





void SG90_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    // 1. 打开TIM4的时钟（TIM4属于APB1总线）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    // 2. 打开GPIOB的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // 3. 配置PB6为复用模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 4. 将PB6复用为TIM4通道1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    
    // 5. 配置TIM4的时基参数
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;         // 预分频值：84MHz/8400=10kHz（100μs计数1次）
    TIM_TimeBaseStructure.TIM_Period = 200 - 1;             // 周期：20ms（20ms/100μs=200次计数）
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    // 6. 配置TIM4通道1的PWM参数
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        // PWM模式1（递增计数时，CNT<CCR时通道有效）
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_Pulse = 15;                        // 初始脉冲值（对应0.5ms，0°）
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 高电平有效
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);                 // 初始化TIM4通道1
    
    // 7. 使能预装载寄存器
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    
    // 8. 使能自动重载预装载
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    
    // 9. 使能TIM4
    TIM_Cmd(TIM4, ENABLE);
}


void SG90_SetAngle(uint8_t angle)
{
    // 角度范围限制
    if (angle > 180) angle = 180;
    
    // 计算对应的脉冲值（0.5ms-2.5ms对应0°-180°）
    // 公式：pulse = 5 + (angle × 20 / 180)
    uint16_t pulse = 5 + (angle * 20 / 180);
    
    // 更新比较值
    TIM_SetCompare1(TIM4, pulse);
}





void SG90_Init_PA6(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    /* 1. 使能时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);   // TIM13时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   // GPIOA时钟使能
    
    /* 2. 配置PA6为复用推挽输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;               // PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            // 复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      // 高速
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          // 推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 3. 配置PA6的复用功能为TIM13_CH1 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM13);
    
    /* 4. 配置TIM13时基参数（产生20ms周期的PWM） */
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;         // 预分频：84MHz/8400=10kHz（100μs/计数）
    TIM_TimeBaseStructure.TIM_Period = 200 - 1;             // 周期：200*100μs=20ms（50Hz）
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频因子
    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
    
    /* 5. 配置TIM13_CH1为PWM模式 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_Pulse = 15;                      // 初始角度：90°（1.5ms）
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 高电平有效
    TIM_OC1Init(TIM13, &TIM_OCInitStructure);                // 初始化通道1
    
    /* 6. 使能预装载寄存器 */
    TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM13, ENABLE);
    
    /* 7. 启动定时器 */
    TIM_Cmd(TIM13, ENABLE);
}

/**
  * @brief  设置SG90舵机角度（0-180°）
  * @param  angle: 目标角度（0-180）
  * @retval 无
  */
void SG90_SetAngle_PA6(uint8_t angle)
{
    uint16_t pulse;

    
    // 计算脉冲值：0.5ms(0°) ~ 2.5ms(180°)
    // 公式：pulse = 5 + (angle × 20 / 180) （5对应0.5ms，25对应2.5ms）
    pulse = 5 + (angle * 20 / 180);
    
    // 更新TIM13通道1的比较值
    TIM_SetCompare1(TIM13, pulse);
}

