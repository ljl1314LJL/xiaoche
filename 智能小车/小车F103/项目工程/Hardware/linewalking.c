#include "linewalking.h"

extern uint8_t Auto_Line_flag; // 循迹功能标志位


void LineWalking_GPIO_Init(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(LineWalk_L1_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_L1_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(LineWalk_L1_PORT, &GPIO_InitStructure);		 



	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(LineWalk_L2_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_L2_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(LineWalk_L2_PORT, &GPIO_InitStructure);	



	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(LineWalk_R1_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_R1_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(LineWalk_R1_PORT, &GPIO_InitStructure);	

	

	// /*开启外设时钟*/
	// RCC_APB2PeriphClockCmd(LineWalk_R2_RCC, ENABLE); 
	// /*选择要控制的引脚*/															   
  	// GPIO_InitStructure.GPIO_Pin = LineWalk_R2_PIN;	
	// /*设置引脚模式为通用推挽输出*/
  	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	// /*设置引脚速率为50MHz */   
  	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	// /*调用库函数，初始化PORT*/
  	// GPIO_Init(LineWalk_R2_PORT, &GPIO_InitStructure);	


	 

}


/*
* Function       bsp_GetLineWalking
* @author        liusen
* @date          2017.07.20    
* @brief         获取巡线状态
* @param[in]     int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2  四路巡线位指针
* @param[out]    void
* @retval        void
* @par History   无
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

	bsp_GetLineWalking(&LineL1, &LineL2, &LineR1, &LineR2);	//获取黑线检测状态	

	if( LineL2 == 0 && LineR1 == 0 &&LineL1 == 1 && LineR2 == 1 )  //L2和R1双亮
    {
      	Move_Forward(55,55);
		//printf("1直走\r\n");
		
    }

    else if ( LineL1 == 0 && LineL2 == 1 && LineR1 == 1 && LineR2 == 1 ) // L1单亮
	{ 
      	Move_Forward(100,0);
		//printf("2右小弯\r\n");
		//Delay_ms(500);
    }  

    else if( LineL1 == 1 && LineL2 == 0 && LineR1 == 1 && LineR2 == 1 ) //L2单亮
    {  
		Move_Forward(100,0); 
		//printf("直角右转弯\r\n");
		//Delay_ms(100);
	}

    else if ( LineL1 == 1 && LineL2 == 1 && LineR1 == 0 && LineR2 == 1) //R1单亮
    {  
		Move_Forward(0,100);
		//printf("左小弯\r\n");
		//Delay_ms(300);
	}

    else if (LineL1 == 1 && LineL2 == 1 && LineR1 == 1 && LineR2 == 0) //R2单亮
    {   
		//printf("5\r\n");
		Move_Forward(0,100);   
	}

	else if (LineL1 == 0 && LineL2 == 0 && LineR1 == 1 && LineR2 == 1) // L1和L2亮
	{  
		Move_Forward(100,0);
	}

	else if (LineL1 == 0 && LineL2 == 0 && LineR1 == 0 && LineR2 == 1) // L1和L2 R1亮
	{  
		Move_Forward(100,0);
	}

	else if (LineL1 == 1 && LineL2 == 1 && LineR1 == 0 && LineR2 == 0) // R1和R2亮
	{  
		Move_Forward(0,100);
	}

	else if (LineL1 == 1 && LineL2 == 0 && LineR1 == 0 && LineR2 == 0) // L2和R1 R2亮
	{  
		Move_Forward(0,100);
	}
	
    else if(LineL1 == 1 && LineL2 == 1 && LineR1 == 1 && LineR2 == 1) // 没有颜色
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
