/*
 * 24cxx.h
 *
 *  Created on: 2018-2-26
 *      Author: Administrator
 */

#ifndef _24cxx_H
#define _24cxx_H

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#include "iic.h"


#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767

//开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02

unsigned char AT24CXX_ReadOneByte(Uint16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(Uint16 WriteAddr,unsigned char DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(Uint16 WriteAddr,Uint32 DataToWrite,unsigned char Len);//指定地址开始写入指定长度的数据
Uint32 AT24CXX_ReadLenByte(Uint16 ReadAddr,unsigned char Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(Uint16 WriteAddr,unsigned char *pBuffer,Uint16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(Uint16 ReadAddr,unsigned char *pBuffer,Uint16 NumToRead);   	//从指定地址开始读出指定长度的数据

unsigned char AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC


#endif /* 24CXX_H_ */
