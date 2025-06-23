#include "pwm.h"

//宏定义
#define TIM        TIM2
#define TIM_tim    RCC_APB1Periph_TIM2
#define TIM_gpio   RCC_APB2Periph_GPIOA


#define GPIO  GPIO_Pin_0|GPIO_Pin_1

//初始化PW

void PWM_Init(void)
{
    //1.使能GPIO时钟和TIM时钟
  RCC_APB1PeriphClockCmd(TIM_tim,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(TIM_gpio,ENABLE);                                                               
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;// 
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;// 
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_8 |GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //2.配置TIM
		TIM_InternalClockConfig(TIM);//72MHz
		TIM_InternalClockConfig(TIM4);//72MHz



    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1 ;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=100-1;//ARR=100;
    TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;// 
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
		
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1 ;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=20000-1;//ARR=100;
    TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;// 
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	//3.配置输出比较模式
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);//
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse=0;//
    TIM_OC1Init(TIM2,&TIM_OCInitStructure);//
    TIM_OC2Init(TIM2,&TIM_OCInitStructure);//
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);//
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);//
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);//
    TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}


void PWM_SetCompare0(uint32_t Compare)
{
    TIM_SetCompare1(TIM2,Compare);// 
}

void PWM_SetCompare1(uint32_t Compare)

{
    TIM_SetCompare2(TIM2,Compare);// 
}


void PWM_SetCompare3(uint16_t Compare)

{
    TIM_SetCompare1(TIM4,Compare);// 
}

//上下舵机

void PWM_SetCompare4(uint16_t Compare)

{
    TIM_SetCompare3(TIM4,Compare);// 
}

//左右舵机
void PWM_SetCompare5(uint16_t Compare)

{
    TIM_SetCompare2(TIM4,Compare);// 
}



