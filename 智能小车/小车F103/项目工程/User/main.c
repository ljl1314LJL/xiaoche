#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "pwm.h"
#include "motor.h"
#include "smartcar.h"
#include "HCSR04.h"
#include "duoji.h"
#include "EXIT.h"
#include "Bluetooth.h"
#include "mode.h"
#include "linewalking.h"


extern uint8_t avoid_flag; // 避障功能标志位
extern uint8_t Auto_Line_flag; // 循迹功能标志位

float Distance; //障碍物距离



int main(void)
{
 
 	SmartCar_Init();//动力模块初始化
 	HCSR04_Init();//超声波模块初始化
 	LineWalking_GPIO_Init(); // 线走模式GPIO初始化
 	UART2_Config(9600);//蓝牙初始化用于连接蓝牙，其中已有中断函数
 	UART1_Config(115200);//串口1初始化用于调试
 	UART3_Config(9600);//串口3用于和语音助手通信
 	UART2_SendString("AT\r\n"); //测试指令  响应"OK\r\n"
	printf("AT\r\n"); //测试指令  响应"OK\r\n"
	Servo_SetAngle(90);//居中超声波舵机
	Servo_SetAngle7(90);//居中摄像头左右舵机
	Servo_SetAngle8(80);//居中摄像头上下舵机



	while(1)
	{


		// 避障模式
		 if (avoid_flag  == 1)
		 {
			
		 	auto_avoidance_mode(); 
			
		 }
		 // 循迹模式
		 else if (Auto_Line_flag  == 1)
		 {
			linewalking_task();
		 }
		

		
	}
	

	
}





