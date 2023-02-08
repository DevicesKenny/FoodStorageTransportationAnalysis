/*
 * sht30.c
 *
 *  Created on: Jan 20, 2023
 *      Author: Administrator
 */

#include "sht30.h"

void SHT30_Init(void)
{
    IIC_B_Start();
    IIC_B_Send_Byte(0x44<<1|0); //0代表写，1代表读
    IIC_B_Wait_Ack();
    IIC_B_Send_Byte(0x21);
    IIC_B_Wait_Ack();
    IIC_B_Send_Byte(0x30);
    IIC_B_Wait_Ack();
    IIC_B_Stop();
    DELAY_US(150);
}

void READ_IIC_B_DATA(float *Temperature,float *Humidity)
{
    Uint16 data[6];
    Uint16 temp,humi;
    IIC_B_Start();
    IIC_B_Send_Byte(0x44<<1|0); //0代表写，1代表读
    IIC_B_Wait_Ack();
    IIC_B_Send_Byte(0xe0);
    IIC_B_Wait_Ack();
    IIC_B_Send_Byte(0x00);
    IIC_B_Wait_Ack();

    IIC_B_Start();
    IIC_B_Send_Byte(0x44<<1|1); //0代表写，1代表读
    IIC_B_Wait_Ack();

    //获取温度数据
    data[0] = IIC_B_Read_Byte(1);
    data[1] = IIC_B_Read_Byte(1);

    data[2] = IIC_B_Read_Byte(1);
    //获取湿度数据
    data[3] = IIC_B_Read_Byte(1);
    data[4] = IIC_B_Read_Byte(1);

    data[5] = IIC_B_Read_Byte(0);
    IIC_B_Stop();

    temp = (data[0]<<8)|data[1];
    humi = (data[3]<<8)|data[4];

    //转换实际温度
    *Temperature = (175.0*(float)temp/65535.0-45.0); //T=-45+175*tem
    *Humidity = (100.0*(float)humi/65535.0);//RH=hum*100/(2^16-1)
}


void Measure_Peroid_Mode(void)
{
    IIC_Start();
    IIC_B_Send_Byte(0x44&0);
    IIC_B_Wait_Ack();
    IIC_B_Send_Byte(0x27);
    IIC_B_Wait_Ack();
    IIC_B_Send_Byte(0x37);
    IIC_B_Wait_Ack();
}


