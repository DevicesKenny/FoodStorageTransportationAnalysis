/*
 * iic.c
 *
 *  Created on: 2018-2-5
 *      Author: Administrator
 */

#include "iic.h"


/*******************************************************************************
* 函 数 名         : IIC_Init
* 函数功能		   : IIC初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IICA_Init(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;	  	//上拉
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;   	// 输出端口
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;  	// IO口
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3; 	// 不同步

	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	  	//上拉
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;   	// 输出端口
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;  	// IO口
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;   // 不同步
	EDIS;
}

/*******************************************************************************
* 函 数 名         : SDA_OUT
* 函数功能		   : SDA输出配置
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_OUT(void)
{
	EALLOW;
	GpioCtrlRegs.GPBDIR.bit.GPIO32=1;       //Output. SDA
	EDIS;
}

/*******************************************************************************
* 函 数 名         : SDA_IN
* 函数功能		   : SDA输入配置
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_IN(void)
{
	EALLOW;
	GpioCtrlRegs.GPBDIR.bit.GPIO32=0;       //Input, SDA
	EDIS;
}

/*******************************************************************************
* 函 数 名         : IIC_Start
* 函数功能		   : 产生IIC起始信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_SETH;
	IIC_SCL_SETH;
	DELAY_US(5);
	IIC_SDA_SETL;//START:when CLK is high,DATA change form high to low
	DELAY_US(6);
	IIC_SCL_SETL;//钳住I2C总线，准备发送或接收数据
}

/*******************************************************************************
* 函 数 名         : IIC_Stop
* 函数功能		   : 产生IIC停止信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_SETL;
	IIC_SDA_SETL;//STOP:when CLK is high DATA change form low to high
	IIC_SCL_SETH;
 	DELAY_US(6);
 	IIC_SDA_SETH;//发送I2C总线结束信号
	DELAY_US(6);
}

/*******************************************************************************
* 函 数 名         : IIC_Wait_Ack
* 函数功能		   : 等待应答信号到来
* 输    入         : 无
* 输    出         : 1，接收应答失败
        			 0，接收应答成功
*******************************************************************************/
unsigned char IIC_Wait_Ack(void)
{
	unsigned char tempTime=0;

	IIC_SDA_SETH;
	DELAY_US(1);
	SDA_IN();      //SDA设置为输入
	IIC_SCL_SETH;
	DELAY_US(1);
	while(READ_SDA)
	{
		tempTime++;
		if(tempTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_SETL;//时钟输出0
	return 0;
}

/*******************************************************************************
* 函 数 名         : IIC_Ack
* 函数功能		   : 产生ACK应答
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL_SETL;
	SDA_OUT();
	IIC_SDA_SETL;
	DELAY_US(2);
	IIC_SCL_SETH;
	DELAY_US(5);
	IIC_SCL_SETL;
}

/*******************************************************************************
* 函 数 名         : IIC_NAck
* 函数功能		   : 产生NACK非应答
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL_SETL;
	SDA_OUT();
	IIC_SDA_SETH;
	DELAY_US(2);
	IIC_SCL_SETH;
	DELAY_US(5);
	IIC_SCL_SETL;
}

/*******************************************************************************
* 函 数 名         : IIC_Send_Byte
* 函数功能		   : IIC发送一个字节
* 输    入         : txd：发送一个字节
* 输    出         : 无
*******************************************************************************/
void IIC_Send_Byte(unsigned char txd)
{
	unsigned char t;
	SDA_OUT();
	IIC_SCL_SETL;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        if((txd&0x80)>0) //0x80  1000 0000
        	IIC_SDA_SETH;
		else
			IIC_SDA_SETL;
        txd<<=1;
        DELAY_US(2);   //对TEA5767这三个延时都是必须的
        IIC_SCL_SETH;
		DELAY_US(2);
		IIC_SCL_SETL;
		DELAY_US(2);
    }
}

/*******************************************************************************
* 函 数 名         : IIC_Read_Byte
* 函数功能		   : IIC读一个字节
* 输    入         : ack=1时，发送ACK，ack=0，发送nACK
* 输    出         : 应答或非应答
*******************************************************************************/
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
    	IIC_SCL_SETL;
    	DELAY_US(2);
        IIC_SCL_SETH;
        receive<<=1;
        if(READ_SDA)receive++;
        DELAY_US(1);
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}

void IICB_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟
    GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;     //上拉
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;     // 输出端口
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;    // IO口
    GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 3;   // 不同步

    GpioCtrlRegs.GPBPUD.bit.GPIO51 = 0;     //上拉
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;     // 输出端口
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;    // IO口
    GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 3;   // 不同步
    EDIS;
}

/*******************************************************************************
* 函 数 名         : SDA_OUT_B
* 函数功能         : SDA输出配置
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_OUT_B(void)
{
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO50=1;       //Output. SDA
    EDIS;
}

/*******************************************************************************
* 函 数 名         : SDA_IN_B
* 函数功能         : SDA输入配置
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_IN_B(void)
{
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO50=0;       //Input, SDA
    EDIS;
}

/*******************************************************************************
* 函 数 名         : IIC_B_Start
* 函数功能         : 产生IIC起始信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_B_Start(void)
{
    SDA_OUT_B();     //sda线输出
    IIC_B_SDA_SETH;
    IIC_B_SCL_SETH;
    DELAY_US(5);
    IIC_B_SDA_SETL;//START:when CLK is high,DATA change form high to low
    DELAY_US(6);
    IIC_B_SCL_SETL;//钳住I2C总线，准备发送或接收数据
}

/*******************************************************************************
* 函 数 名         : IIC_B_Stop
* 函数功能         : 产生IIC停止信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_B_Stop(void)
{
    SDA_OUT_B();//sda线输出
    IIC_B_SCL_SETL;
    IIC_B_SDA_SETL;//STOP:when CLK is high DATA change form low to high
    IIC_B_SCL_SETH;
    DELAY_US(6);
    IIC_B_SDA_SETH;//发送I2C总线结束信号
    DELAY_US(6);
}

/*******************************************************************************
* 函 数 名         : IIC_B_Wait_Ack
* 函数功能         : 等待应答信号到来
* 输    入         : 无
* 输    出         : 1，接收应答失败
                     0，接收应答成功
*******************************************************************************/
unsigned char IIC_B_Wait_Ack(void)
{
    unsigned char tempTime=0;

    IIC_B_SDA_SETH;
    DELAY_US(1);
    SDA_IN_B();      //SDA设置为输入
    IIC_B_SCL_SETH;
    DELAY_US(1);
    while(READ_B_SDA)
    {
        tempTime++;
        if(tempTime>250)
        {
            IIC_B_Stop();
            return 1;
        }
    }
    IIC_B_SCL_SETL;//时钟输出0
    return 0;
}

/*******************************************************************************
* 函 数 名         : IIC_B_Ack
* 函数功能         : 产生ACK应答
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_B_Ack(void)
{
    IIC_B_SCL_SETL;
    SDA_OUT_B();
    IIC_B_SDA_SETL;
    DELAY_US(2);
    IIC_B_SCL_SETH;
    DELAY_US(5);
    IIC_B_SCL_SETL;
}

/*******************************************************************************
* 函 数 名         : IIC_B_NAck
* 函数功能         : 产生NACK非应答
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_B_NAck(void)
{
    IIC_B_SCL_SETL;
    SDA_OUT_B();
    IIC_B_SDA_SETH;
    DELAY_US(2);
    IIC_B_SCL_SETH;
    DELAY_US(5);
    IIC_B_SCL_SETL;
}

/*******************************************************************************
* 函 数 名         : IIC_B_Send_Byte
* 函数功能         : IIC发送一个字节
* 输    入         : txd：发送一个字节
* 输    出         : 无
*******************************************************************************/
void IIC_B_Send_Byte(unsigned char txd)
{
    unsigned char t;
    SDA_OUT_B();
    IIC_B_SCL_SETL;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        if((txd&0x80)>0) //0x80  1000 0000
            IIC_B_SDA_SETH;
        else
            IIC_B_SDA_SETL;
        txd<<=1;
        DELAY_US(2);   //对TEA5767这三个延时都是必须的
        IIC_B_SCL_SETH;
        DELAY_US(2);
        IIC_B_SCL_SETL;
        DELAY_US(2);
    }
}

/*******************************************************************************
* 函 数 名         : IIC_B_Read_Byte
* 函数功能         : IIC读一个字节
* 输    入         : ack=1时，发送ACK，ack=0，发送nACK
* 输    出         : 应答或非应答
*******************************************************************************/
unsigned char IIC_B_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN_B();//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        IIC_B_SCL_SETL;
        DELAY_US(2);
        IIC_B_SCL_SETH;
        receive<<=1;
        if(READ_B_SDA)receive++;
        DELAY_US(1);
    }
    if (!ack)
        IIC_B_NAck();//发送nACK
    else
        IIC_B_Ack(); //发送ACK
    return receive;
}

