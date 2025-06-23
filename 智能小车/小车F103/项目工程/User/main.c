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


extern uint8_t avoid_flag; // ���Ϲ��ܱ�־λ
extern uint8_t Auto_Line_flag; // ѭ�����ܱ�־λ

float Distance; //�ϰ������



int main(void)
{
 
 	SmartCar_Init();//����ģ���ʼ��
 	HCSR04_Init();//������ģ���ʼ��
 	LineWalking_GPIO_Init(); // ����ģʽGPIO��ʼ��
 	UART2_Config(9600);//������ʼ�������������������������жϺ���
 	UART1_Config(115200);//����1��ʼ�����ڵ���
 	UART3_Config(9600);//����3���ں���������ͨ��
 	UART2_SendString("AT\r\n"); //����ָ��  ��Ӧ"OK\r\n"
	printf("AT\r\n"); //����ָ��  ��Ӧ"OK\r\n"
	Servo_SetAngle(90);//���г��������
	Servo_SetAngle7(90);//��������ͷ���Ҷ��
	Servo_SetAngle8(80);//��������ͷ���¶��



	while(1)
	{


		// ����ģʽ
		 if (avoid_flag  == 1)
		 {
			
		 	auto_avoidance_mode(); 
			
		 }
		 // ѭ��ģʽ
		 else if (Auto_Line_flag  == 1)
		 {
			linewalking_task();
		 }
		

		
	}
	

	
}





