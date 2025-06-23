#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"  
#include "smartcar.h"
#include "Delay.h"
#include <string.h>
#include <stdio.h>
#include "duoji.h"

int fputc(int ch, FILE *f) ;

void _sys_exit(int return_code);


void UART2_Config(u32 baud);
void UART2_SendString(char * str);
void UART1_Config(u32 baud);
void USART1_IRQHandler(void);
void UART2_sendfloat(float num);
void linewalking_task(void);
void UART3_Config(u32 baud);

#endif
