/*
 * beep.c
 *
 *  Created on: Jan 7, 2023
 *      Author: Administrator
 */

#include "beep.h"

/*******************************************************************************
* �� �� ��         : BEEP_Init
* ��������         : ��������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void BEEP_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��
    //BEEP�˿�����
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
