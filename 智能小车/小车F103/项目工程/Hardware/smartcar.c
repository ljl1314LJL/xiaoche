#include "smartcar.h"


//初始化智能小车
void SmartCar_Init(void)
{
    Motor_Init();
}
//向前移动
void Move_Forward(int32_t leftSpeed, int32_t rightSpeed)
{
    LeftMotor_Speed(leftSpeed);
    RightMotor_Speed(rightSpeed);
}


