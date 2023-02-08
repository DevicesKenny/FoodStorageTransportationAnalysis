/*
 * 24cxx.c
 *
 *  Created on: 2018-2-26
 *      Author: Administrator
 */


#include "24cxx.h"


/*******************************************************************************
* �� �� ��         : AT24CXX_Init
* ��������		   : AT24CXX��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_Init(void)
{
	IICA_Init();//IIC��ʼ��
}

/*******************************************************************************
* �� �� ��         : AT24CXX_ReadOneByte
* ��������		   : ��AT24CXXָ����ַ����һ������
* ��    ��         : ReadAddr:��ʼ�����ĵ�ַ
* ��    ��         : ����������
*******************************************************************************/
unsigned char AT24CXX_ReadOneByte(Uint16 ReadAddr)
{
	unsigned char temp=0;
    IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
	}
	else
	{
		IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д����
	}
	IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1);           //�������ģʽ
	IIC_Wait_Ack();
    temp=IIC_Read_Byte(0);
    IIC_Stop();//����һ��ֹͣ����
	return temp;
}

/*******************************************************************************
* �� �� ��         : AT24CXX_WriteOneByte
* ��������		   : ��AT24CXXָ����ַд��һ������
* ��    ��         : WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
					 DataToWrite:Ҫд�������
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_WriteOneByte(Uint16 WriteAddr,unsigned char DataToWrite)
{
    IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
	}
	else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д����
	}
	IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);     //�����ֽ�
	IIC_Wait_Ack();
    IIC_Stop();//����һ��ֹͣ����
	DELAY_US(10*1000);
}

/*******************************************************************************
* �� �� ��         : AT24CXX_WriteLenByte
* ��������		   : ��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
					 ����д��16bit����32bit������
* ��    ��         : WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
					 DataToWrite:Ҫд�������
					 Len        :Ҫд�����ݵĳ���2,4
* ��    ��         : ��
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
* �� �� ��         : AT24CXX_ReadLenByte
* ��������		   : ��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
					 ���ڶ���16bit����32bit������
* ��    ��         : ReadAddr   :��ʼ�����ĵ�ַ
					 Len        :Ҫ�������ݵĳ���2,4
* ��    ��         : ��ȡ������
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
* �� �� ��         : AT24CXX_Check
* ��������		   : ���AT24CXX�Ƿ�����
* ��    ��         : ��
* ��    ��         : 1:���ʧ�ܣ�0:���ɹ�
*******************************************************************************/
unsigned char AT24CXX_Check(void)
{
	unsigned char temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX
	if(temp==0x36)return 0;
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X36);
	    temp=AT24CXX_ReadOneByte(255);
		if(temp==0X36)return 0;
	}
	return 1;
}

/*******************************************************************************
* �� �� ��         : AT24CXX_Read
* ��������		   : ��AT24CXX�����ָ����ַ��ʼ����ָ������������
* ��    ��         : ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
					 pBuffer  :���������׵�ַ
					 NumToRead:Ҫ�������ݵĸ���
* ��    ��         : ��
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
* �� �� ��         : AT24CXX_Write
* ��������		   : ��AT24CXX�����ָ����ַ��ʼд��ָ������������
* ��    ��         : WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
					 pBuffer  :���������׵�ַ
					 NumToRead:Ҫ�������ݵĸ���
* ��    ��         : ��
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
