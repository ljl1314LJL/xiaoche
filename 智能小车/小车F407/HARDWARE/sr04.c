#include "sr04.h"


/************************************
引脚说明


PC7– TRIG（触发信号）  
PA4 – ECHO（回响信号）
************************************/

/**
 * @brief 初始化SR04模块的TRIG和ECHO引脚
 * 该函数用于初始化SR04模块的TRIG和ECHO引脚，包括配置GPIO和定时器。
 * @details
 * 1. 使能定时器时钟
 * 2. 配置GPIO引脚
 *   - TRIG(PC7) 作为触发信号输出端口，配置为输出模式
 *   - ECHO(PA4) 作为输入端口，配置为输入模式
 * 3. 初始化定时器，配置自动重装载寄存器和预分频器
 * @note
 * - 在初始化完成后，定时器被禁用
 */
void Sr04_Init(void)//初始化SR04模块的TRIG和ECHO引脚
{
	
	GPIO_InitTypeDef	GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	
    //1、能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	//时钟初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
	//打开PA5端口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	

	
	//TRIG(PC7触发信号输出端口初始化
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_7; 
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//ECHO(PA4输入端口初始化
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_4; 	
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	
    TIM_TimeBaseInitStruct.TIM_Prescaler    = 84-1;                  // 84分频 84MHZ/84 = 1MHZ 计1个数用1us
    TIM_TimeBaseInitStruct.TIM_Period    	= 50000-1; //（其实计不到这么多数）                       
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;    // 向上计数
    TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;            // 分频因子
    //2、初始化定时器，配置ARR,PSC
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
            
    //5,不使能定时器 没有计数
     TIM_Cmd(TIM3, DISABLE);	
	
}




/**
 * @brief 获取SR04超声波传感器的测量值
 *
 * 此函数通过SR04超声波传感器获取测量距离，并以厘米为单位返回结果。
 *
 * @return 测量的距离值（以厘米为单位），如果等待超时则返回错误码
 */
int Get_Sr04_Value(void)
{
	int dis, t = 0;
	u32 temp;
	
	//触发信号
	//PC7输出低电平
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);	
	delay_us(8);
	//PC7输出高电平
	GPIO_SetBits(GPIOC, GPIO_Pin_7);//PC7输出高电平，至少10us
	delay_us(15); //至少10us
	//PC7输出低电平
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);	
	
	
	//设置定时器的CNT为0  
	TIM3->CNT = 0;

	//PA4等待高电平到来，参考按键松开代码 
	//添加超时等待
	while( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)  != Bit_SET )
	{
		t++;
		delay_us(10);
		
		if(t == 300) //等待3000us，未有高电平到来
			return 1000;
	}

	//使能定时器开始计数
	TIM_Cmd(TIM3, ENABLE);	
	
	
	t = 0;
	//PA3等待低电平到来， 
	//手册说明测量最大距离:400cm, 等待最长低电平的时间:400*58 =  23200us
	while( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_SET)
	{
		t++;
		delay_us(10);
		
		if(t == 2320) //等待23200us，未有低电平到来
			return 2000;	
	}


	//获取定时器CNT值，从而得到高电平持续时间 
	//TIM3->CNT值为多少，表示高电平持续时间为多少us
	temp = TIM3->CNT;

	//关闭定时器
	TIM_Cmd(TIM3, DISABLE);
	
	
	//通过公式计算出超声波测量距离	
	
	dis = temp/58;
	
	return dis;

}

