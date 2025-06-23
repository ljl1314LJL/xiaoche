 #include "motor.h"


void Motor_Init(void)
{
    // 必须先使能AFIO时钟！
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    // 然后才能进行引脚重映射
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    
    // 初始化GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 初始化后强制设置PB3状态
   // GPIO_ResetBits(GPIOB, GPIO_Pin_3);  // 立即拉低PB3
    
    PWM_Init();
}
//设置左侧电机的速度
void LeftMotor_Speed(int32_t Speed)
{
    if(Speed>0)
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_5);
        GPIO_SetBits(GPIOB,GPIO_Pin_4);
        //printf("left%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5));
       // printf("left%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4));
        PWM_SetCompare1(Speed);
    }
    else if(Speed==0)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_4);
        GPIO_SetBits(GPIOB,GPIO_Pin_5);
        // printf("left%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4));
        // printf("left%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3));
        PWM_SetCompare1(Speed);
    }
    else if (Speed<0)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_5);    // 反转方向
        GPIO_ResetBits(GPIOB,GPIO_Pin_4);
       
        
        //printf("left%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5));
        //printf("left%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4));
        
        PWM_SetCompare1(-Speed);// CCR的值为正，所以Speed前面加负号
		//printf("3333333");
    }
        
}

//设置右侧电机的速度
void RightMotor_Speed(int32_t Speed)
{
    if(Speed>0)
    {  
        GPIO_SetBits(GPIOB,GPIO_Pin_0);
        GPIO_ResetBits(GPIOB,GPIO_Pin_1);
        //printf("right%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));
        //printf("right%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1));
        PWM_SetCompare0(Speed);
    }
    else if(Speed==0)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_0);
        GPIO_SetBits(GPIOB,GPIO_Pin_1);
        // printf("right%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));
        // printf("right%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1));
        PWM_SetCompare0(Speed);
    }
    else if (Speed<0)
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);
        GPIO_SetBits(GPIOB,GPIO_Pin_1);
        //printf("right%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));
        //printf("right%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1));
        PWM_SetCompare0(-Speed);// CCR的值为正，所以Speed前面加负号
    }
}



       
        
