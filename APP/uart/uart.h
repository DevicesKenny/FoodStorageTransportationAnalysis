/*
 * uart.h
 *
 *  Created on: 2018-1-31
 *      Author: Administrator
 */

#ifndef UART_H_
#define UART_H_

#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件

#define UARTa_LEN 255
#define UARTb_LEN 255

extern unsigned char Flag_Uarta_Receive;
extern unsigned char Flag_Uartb_Receive;
extern unsigned char Flag_Uartb_Receive_OK;

void UARTa_Init(Uint32 baud);
void UARTa_SendByte(int a);
void UARTa_SendString(char * msg);
void UARTa_Clear(void);

void UARTb_Init(Uint32 baud);
void UARTb_SendByte(int a);
void UARTb_SendString(char * msg);
void UARTb_Clear(void);

#endif /* UART_H_ */
