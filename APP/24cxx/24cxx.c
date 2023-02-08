/*
 * 24cxx.c
 *
 *  Created on: 2018-2-26
 *      Author: Administrator
 */


#include "24cxx.h"


/*******************************************************************************
* 函 数 名         : AT24CXX_Init
* 函数功能		   : AT24CXX初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void AT24CXX_Init(void)
{
	IICA_Init();//IIC初始化
}

/*******************************************************************************
* 函 数 名         : AT24CXX_ReadOneByte
* 函数功能		   : 在AT24CXX指定地址读出一个数据
* 输    入         : ReadAddr:开始读数的地址
* 输    出         : 读到的数据
*******************************************************************************/
unsigned char AT24CXX_ReadOneByte(Uint16 ReadAddr)
{
	unsigned char temp=0;
    IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
	}
	else
	{
		IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据
	}
	IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1);           //进入接收模式
	IIC_Wait_Ack();
    temp=IIC_Read_Byte(0);
    IIC_Stop();//产生一个停止条件
	return temp;
}

/*******************************************************************************
* 函 数 名         : AT24CXX_WriteOneByte
* 函数功能		   : 在AT24CXX指定地址写入一个数据
* 输    入         : WriteAddr  :写入数据的目的地址
					 DataToWrite:要写入的数据
* 输    出         : 无
*******************************************************************************/
void AT24CXX_WriteOneByte(Uint16 WriteAddr,unsigned char DataToWrite)
{
    IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
	}
	else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据
	}
	IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);     //发送字节
	IIC_Wait_Ack();
    IIC_Stop();//产生一个停止条件
	DELAY_US(10*1000);
}

/*******************************************************************************
* 函 数 名         : AT24CXX_WriteLenByte
* 函数功能		   : 在AT24CXX里面的指定地址开始写入长度为Len的数据
					 用于写入16bit或者32bit的数据
* 输    入         : WriteAddr  :写入数据的目的地址
					 DataToWrite:要写入的数据
					 Len        :要写入数据的长度2,4
* 输    出         : 无
*******************************************************************************/
void AT24CXX_WriteLenByte(Uint16 WriteAddr,Uint32 DataToWrite,unsigned char Len)
{
	unsigned char t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
}

/*******************************************************************************
* 函 数 名         : AT24CXX_ReadLenByte
* 函数功能		   : 在AT24CXX里面的指定地址开始读出长度为Len的数据
					 用于读出16bit或者32bit的数据
* 输    入         : ReadAddr   :开始读出的地址
					 Len        :要读出数据的长度2,4
* 输    出         : 读取的数据
*******************************************************************************/
Uint32 AT24CXX_ReadLenByte(Uint16 ReadAddr,unsigned char Len)
{
	unsigned char t;
	Uint32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}
	return temp;
}

/*******************************************************************************
* 函 数 名         : AT24CXX_Check
* 函数功能		   : 检查AT24CXX是否正常
* 输    入         : 无
* 输    出         : 1:检测失败，0:检测成功
*******************************************************************************/
unsigned char AT24CXX_Check(void)
{
	unsigned char temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX
	if(temp==0x36)return 0;
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X36);
	    temp=AT24CXX_ReadOneByte(255);
		if(temp==0X36)return 0;
	}
	return 1;
}

/*******************************************************************************
* 函 数 名         : AT24CXX_Read
* 函数功能		   : 在AT24CXX里面的指定地址开始读出指定个数的数据
* 输    入         : ReadAddr :开始读出的地址 对24c02为0~255
					 pBuffer  :数据数组首地址
					 NumToRead:要读出数据的个数
* 输    出         : 无
*******************************************************************************/
void AT24CXX_Read(Uint16 ReadAddr,unsigned char *pBuffer,Uint16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}

/*******************************************************************************
* 函 数 名         : AT24CXX_Write
* 函数功能		   : 在AT24CXX里面的指定地址开始写入指定个数的数据
* 输    入         : WriteAddr :开始写入的地址 对24c02为0~255
					 pBuffer  :数据数组首地址
					 NumToRead:要读出数据的个数
* 输    出         : 无
*******************************************************************************/
void AT24CXX_Write(Uint16 WriteAddr,unsigned char *pBuffer,Uint16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
