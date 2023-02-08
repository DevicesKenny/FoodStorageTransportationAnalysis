/*
 * relay.c
 *
 *  Created on: Jan 8, 2023
 *      Author: Administrator
 */


#include "relay.h"


void Relay_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟

    //继电器端口配置
    GpioCtrlRegs.GPAMUX1.bit.GPIO15=0;
    GpioCtrlRegs.GPADIR.bit.GPIO15=1;
    GpioCtrlRegs.GPAPUD.bit.GPIO15=0;

    EDIS;

    GpioDataRegs.GPACLEAR.bit.GPIO15=1;
}

