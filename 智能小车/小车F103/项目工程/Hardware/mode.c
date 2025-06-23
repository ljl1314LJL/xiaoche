#include "mode.h"

extern uint8_t avoid_flag; // 避障功能标志位
extern uint8_t Auto_Line_flag; // 自动循迹功能标志位
extern float Distance; //障碍物距离

#define OBSTACLE_THRESHOLD 20  // 障碍物阈值（cm）


// 舵机转向并获取对应方向距离
float Servo_TurnAndDetect(uint16_t angle) 
{
    Servo_SetAngle(angle); // 转向
    Delay_ms(1000);        // 等待舵机稳定（需根据舵机速度调整）
    return HCSR04_Distance(); // 获取距离
}

//自动避障模式
void auto_avoidance_mode(void)
{
    
    for(;;)
    {
        Distance = HCSR04_Distance(); //获取距离
        //利用蓝牙发送距离信息在手机上显示
        UART2_SendString("Distance:");
        UART2_sendfloat(Distance);
        UART2_SendString("cm\r\n");
        if (Distance > OBSTACLE_THRESHOLD)
        {
           Move_Forward(100,100); //前进
        }
        
        
        else if(Distance < OBSTACLE_THRESHOLD) //如果障碍物距离小于15cm
        {
            //停止
            Move_Forward(0,0);
            float rightDist = Servo_TurnAndDetect(0);    // 右转90°检测
            float leftDist = Servo_TurnAndDetect(180); // 左转90°检测
            printf("leftDist:%.2f,rightDist:%.2f\r\n",leftDist,rightDist);
            
            Servo_SetAngle(90); // 归位
 

            if (leftDist > OBSTACLE_THRESHOLD) 
            {
              Move_Forward(-100,100); // 左转
              Delay_ms(1300); // 等待舵机稳定
              Move_Forward(100, 100); 
            } 
            else if (rightDist > OBSTACLE_THRESHOLD) 
            {
                Move_Forward(100, -100); // 右转
                Delay_ms(1300); // 
                Move_Forward(100, 100); 
            }
            else 
            {
                // 后退+转向（无可行方向）
                Move_Forward(-100, -100); // 后退
                Delay_ms(1300);
                Move_Forward(100, -100); // 左转
				Delay_ms(1300);
				Move_Forward(100, 100);
            }
            Delay_ms(500); // 等待舵机稳定

        //避免重复进入自动避障模式
        if (avoid_flag == 0)
        {
			Move_Forward(0,0); //停止
            break; //退出自动避障模式
        }
        
       
        
    }	

}
}


//循迹模式
void tracking_mode(void)
{
    for(;;)
    {
        app_LineWalking(); 

        if (Auto_Line_flag == 0)
        {
            Move_Forward(0,0); //停止
            break; //退出循迹模式
        }
        
    }
    

}
