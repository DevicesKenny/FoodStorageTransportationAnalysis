/*
 * beep.c
 *
 *  Created on: Jan 7, 2023
 *      Author: Administrator
 */

#include "beep.h"

/*******************************************************************************
* 函 数 名         : BEEP_Init
* 函数功能         : 蜂鸣器初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void BEEP_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟
    //BEEP端口配置
    GpioCtrlRegs.GPAMUX1.bit.GPIO6=0;
    GpioCtrlRegs.GPADIR.bit.GPIO6=1;
    GpioCtrlRegs.GPAPUD.bit.GPIO6=0;

    EDIS;

    GpioDataRegs.GPACLEAR.bit.GPIO6=1;

}

void BEEP_Play(void)
{
    int i = 0;
    while(i < 2000){
        i++;
        BEEP_TOGGLE;        //1000000/100/2=5kHz
        DELAY_US(100);
    }
}

void BEEP_STOP(void)
{
    BEEP_OFF;
}
