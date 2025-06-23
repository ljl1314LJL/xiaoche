#include "duoji.h"


//void Servo_Init(void)
//{
//    PWM_Init();
//}

// 0       500
// 180     2500
void Servo_SetAngle(float Angle)
{
    PWM_SetCompare3(Angle/180*2000+500);
}


void Servo_SetAngle8(float Angle)
{
    PWM_SetCompare4(Angle/180*2000+500);
}

void Servo_SetAngle7(float Angle)
{
    PWM_SetCompare5(Angle/180*2000+500);
}


//io口模拟
//void SG90_Config(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    
//    // 1. 打开GPIO端口时钟（PB7属于GPIOB）
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  
//    // 2. 配置IO口为普通推挽输出模式
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // 普通推挽输出模式
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // IO翻转速度
//    GPIO_Init(GPIOB, &GPIO_InitStructure); 
//}




//void sg_angle(uint32_t angle)
//{

//    // 根据角度设置PWM脉冲宽度
//    if(angle == 0)
//    {
//        GPIO_SetBits(GPIOB, GPIO_Pin_7);  // 输出高电平
//        Delay_us(500);
//        
//        GPIO_ResetBits(GPIOB, GPIO_Pin_7);  // 输出低电平
//        Delay_us(500);
//        Delay_ms(19);
//    }
//    else if(angle == 45)
//    {
//        GPIO_SetBits(GPIOB, GPIO_Pin_7);  // 输出高电平
//        Delay_ms(1);
//        
//        GPIO_ResetBits(GPIOB, GPIO_Pin_7);  // 输出低电平
//        Delay_ms(19);
//    }
//    else if(angle == 90)
//    {
//        GPIO_SetBits(GPIOB, GPIO_Pin_7);  // 输出高电平
//        Delay_ms(1);
//        Delay_us(500);
//        
//        GPIO_ResetBits(GPIOB, GPIO_Pin_7);  // 输出低电平
//        Delay_ms(18);
//        Delay_us(500);
//    }
//    else if(angle == 135)
//    {
//        GPIO_SetBits(GPIOB, GPIO_Pin_7);  // 输出高电平
//        Delay_ms(2);
//        
//        GPIO_ResetBits(GPIOB, GPIO_Pin_7);  // 输出低电平
//        Delay_ms(18);
//    }    
//    else if(angle == 180)
//    {
//        GPIO_SetBits(GPIOB, GPIO_Pin_7);  // 输出高电平
//        Delay_ms(2);
//        Delay_us(500);
//        
//        GPIO_ResetBits(GPIOB, GPIO_Pin_7);  // 输出低电平
//        Delay_ms(17);
//        Delay_us(500);
//    }        
//}




//void Servo_180(void)
//{

//        sg_angle(180);     // 0度
//		sg_angle(180);     // 0度
//		sg_angle(180);     // 0度
//		sg_angle(180);     // 0度
//	
//} 

//void Servo_90(void)
//{
//    sg_angle(90);    // 90度
//}


//void Servo_0(void)
//{
//    sg_angle(0);    // 0度
//	
//}
