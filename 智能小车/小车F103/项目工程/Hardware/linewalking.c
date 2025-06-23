#include "linewalking.h"

extern uint8_t Auto_Line_flag; // ѭ�����ܱ�־λ


void LineWalking_GPIO_Init(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/*��������ʱ��*/
	RCC_APB2PeriphClockCmd(LineWalk_L1_RCC, ENABLE); 
	/*ѡ��Ҫ���Ƶ�����*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_L1_PIN;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��PORT*/
  	GPIO_Init(LineWalk_L1_PORT, &GPIO_InitStructure);		 



	/*��������ʱ��*/
	RCC_APB2PeriphClockCmd(LineWalk_L2_RCC, ENABLE); 
	/*ѡ��Ҫ���Ƶ�����*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_L2_PIN;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��PORT*/
  	GPIO_Init(LineWalk_L2_PORT, &GPIO_InitStructure);	



	/*��������ʱ��*/
	RCC_APB2PeriphClockCmd(LineWalk_R1_RCC, ENABLE); 
	/*ѡ��Ҫ���Ƶ�����*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_R1_PIN;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��PORT*/
  	GPIO_Init(LineWalk_R1_PORT, &GPIO_InitStructure);	

	

	// /*��������ʱ��*/
	// RCC_APB2PeriphClockCmd(LineWalk_R2_RCC, ENABLE); 
	// /*ѡ��Ҫ���Ƶ�����*/															   
  	// GPIO_InitStructure.GPIO_Pin = LineWalk_R2_PIN;	
	// /*��������ģʽΪͨ���������*/
  	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	// /*������������Ϊ50MHz */   
  	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	// /*���ÿ⺯������ʼ��PORT*/
  	// GPIO_Init(LineWalk_R2_PORT, &GPIO_InitStructure);	


	 

}


/*
* Function       bsp_GetLineWalking
* @author        liusen
* @date          2017.07.20    
* @brief         ��ȡѲ��״̬
* @param[in]     int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2  ��·Ѳ��λָ��
* @param[out]    void
* @retval        void
* @par History   ��
*/
void bsp_GetLineWalking(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2)
{
	*p_iL1 = GPIO_ReadInputDataBit(LineWalk_L1_PORT, LineWalk_L1_PIN);
	*p_iL2 = GPIO_ReadInputDataBit(LineWalk_L2_PORT, LineWalk_L2_PIN);
	*p_iR1 = GPIO_ReadInputDataBit(LineWalk_R1_PORT, LineWalk_R1_PIN);
	*p_iR2 = GPIO_ReadInputDataBit(LineWalk_R2_PORT, LineWalk_R2_PIN);		
}


void app_LineWalking(void)
{
	int LineL1 , LineL2 , LineR1, LineR2;

	bsp_GetLineWalking(&LineL1, &LineL2, &LineR1, &LineR2);	//��ȡ���߼��״̬	

	if( LineL2 == 0 && LineR1 == 0 &&LineL1 == 1 && LineR2 == 1 )  //L2��R1˫��
    {
      	Move_Forward(55,55);
		//printf("1ֱ��\r\n");
		
    }

    else if ( LineL1 == 0 && LineL2 == 1 && LineR1 == 1 && LineR2 == 1 ) // L1����
	{ 
      	Move_Forward(100,0);
		//printf("2��С��\r\n");
		//Delay_ms(500);
    }  

    else if( LineL1 == 1 && LineL2 == 0 && LineR1 == 1 && LineR2 == 1 ) //L2����
    {  
		Move_Forward(100,0); 
		//printf("ֱ����ת��\r\n");
		//Delay_ms(100);
	}

    else if ( LineL1 == 1 && LineL2 == 1 && LineR1 == 0 && LineR2 == 1) //R1����
    {  
		Move_Forward(0,100);
		//printf("��С��\r\n");
		//Delay_ms(300);
	}

    else if (LineL1 == 1 && LineL2 == 1 && LineR1 == 1 && LineR2 == 0) //R2����
    {   
		//printf("5\r\n");
		Move_Forward(0,100);   
	}

	else if (LineL1 == 0 && LineL2 == 0 && LineR1 == 1 && LineR2 == 1) // L1��L2��
	{  
		Move_Forward(100,0);
	}

	else if (LineL1 == 0 && LineL2 == 0 && LineR1 == 0 && LineR2 == 1) // L1��L2 R1��
	{  
		Move_Forward(100,0);
	}

	else if (LineL1 == 1 && LineL2 == 1 && LineR1 == 0 && LineR2 == 0) // R1��R2��
	{  
		Move_Forward(0,100);
	}

	else if (LineL1 == 1 && LineL2 == 0 && LineR1 == 0 && LineR2 == 0) // L2��R1 R2��
	{  
		Move_Forward(0,100);
	}
	
    else if(LineL1 == 1 && LineL2 == 1 && LineR1 == 1 && LineR2 == 1) // û����ɫ
    {  
		// printf("7\r\n");
        Move_Forward(20,20);
		Delay_ms(100);

	}	


}

void linewalking_task(void)
{
	for(;;)
	{
		app_LineWalking();
		if(Auto_Line_flag == 0)
		{
			Move_Forward(0,0);
			break;
		}
	}
}
