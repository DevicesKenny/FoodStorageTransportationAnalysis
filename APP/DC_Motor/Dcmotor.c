/*
 * Dcmotor.c
 *
 *  Created on: Jan 8, 2023
 *      Author: Administrator
 */

#include "Dcmotor.h"

void DC_Motor_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��
    //DC_MOTOR ��1·�˿�����
    GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;
    GpioCtrlRegs.GPADIR.bit.GPIO2=1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO3=0;
    GpioCtrlRegs.GPADIR.bit.GPIO3=1;

    //DC_MOTOR ��2·�˿�����
    GpioCtrlRegs.GPAMUX1.bit.GPIO4=0;
    GpioCtrlRegs.GPADIR.bit.GPIO4=1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO5=0;
    GpioCtrlRegs.GPADIR.bit.GPIO5=1;

    EDIS;

    GpioDataRegs.GPACLEAR.bit.GPIO2=1;
    GpioDataRegs.GPACLEAR.bit.GPIO3=1;
    GpioDataRegs.GPACLEAR.bit.GPIO4=1;
    GpioDataRegs.GPACLEAR.bit.GPIO5=1;
}

void leftRotate(void){
    DC_MOTOR_INC_SETH;
    DC_MOTOR_IND_SETL;
}

void rightRotate(void){
    DC_MOTOR_INC_SETL;
    DC_MOTOR_IND_SETH;
}

void stopRotate(void){
    DC_MOTOR_INC_SETL;
    DC_MOTOR_IND_SETL;
}

