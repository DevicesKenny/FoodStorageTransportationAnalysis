/*
 * iic.h
 *
 *  Created on: 2018-2-5
 *      Author: Administrator
 */

#ifndef IIC_H_
#define IIC_H_


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


#define IIC_SCL_SETH	(GpioDataRegs.GPBSET.bit.GPIO33=1)
#define IIC_SCL_SETL	(GpioDataRegs.GPBCLEAR.bit.GPIO33=1)

#define IIC_SDA_SETH	(GpioDataRegs.GPBSET.bit.GPIO32=1)
#define IIC_SDA_SETL	(GpioDataRegs.GPBCLEAR.bit.GPIO32=1)

#define READ_SDA   		(GpioDataRegs.GPBDAT.bit.GPIO32)

#define IIC_B_SCL_SETH    (GpioDataRegs.GPBSET.bit.GPIO51=1)
#define IIC_B_SCL_SETL    (GpioDataRegs.GPBCLEAR.bit.GPIO51=1)

#define IIC_B_SDA_SETH    (GpioDataRegs.GPBSET.bit.GPIO50=1)
#define IIC_B_SDA_SETL    (GpioDataRegs.GPBCLEAR.bit.GPIO50=1)

#define READ_B_SDA        (GpioDataRegs.GPBDAT.bit.GPIO50)

//IIC���в�������
void IICA_Init(void);                //��ʼ��IIC��IO��
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IICB_Init(void);                //��ʼ��IIC��IO��
void IIC_B_Start(void);               //����IIC��ʼ�ź�
void IIC_B_Stop(void);                //����IICֹͣ�ź�
void IIC_B_Send_Byte(unsigned char txd);          //IIC����һ���ֽ�
unsigned char IIC_B_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC_B_Wait_Ack(void);               //IIC�ȴ�ACK�ź�
void IIC_B_Ack(void);                 //IIC����ACK�ź�
void IIC_B_NAck(void);                //IIC������ACK�ź�

#endif /* IIC_H_ */
