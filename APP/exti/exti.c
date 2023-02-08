/*
 * exti.c
 *
 *  Created on: Jan 9, 2023
 *      Author: Administrator
 */

#include "exti.h"
#include "leds.h"
#include "key.h"
#include "uart.h"

unsigned char flag_key = 0;
int key_code = 0;

void EXTI1_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // GPIO input clock
    EDIS;

    EALLOW;
    //KEY�˿�����
    GpioCtrlRegs.GPAMUX1.bit.GPIO12=0;
    GpioCtrlRegs.GPADIR.bit.GPIO12=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO12=0;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;        // �ⲿ�ж�1��XINT1����ϵͳʱ��SYSCLKOUTͬ��

//    GpioCtrlRegs.GPAMUX1.bit.GPIO13=0;
//    GpioCtrlRegs.GPADIR.bit.GPIO13=0;
//    GpioCtrlRegs.GPAPUD.bit.GPIO13=0;
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 0;        // �ⲿ�ж�1��XINT1����ϵͳʱ��SYSCLKOUTͬ��
//
//    GpioCtrlRegs.GPAMUX1.bit.GPIO14=0;
//    GpioCtrlRegs.GPADIR.bit.GPIO14=0;
//    GpioCtrlRegs.GPAPUD.bit.GPIO14=0;
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 0;        // �ⲿ�ж�1��XINT1����ϵͳʱ��SYSCLKOUTͬ��

    GpioCtrlRegs.GPBMUX2.bit.GPIO48=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO48=1;
    GpioCtrlRegs.GPBPUD.bit.GPIO48=0;
    GpioDataRegs.GPBCLEAR.bit.GPIO48=1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO49=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO49=1;
    GpioCtrlRegs.GPBPUD.bit.GPIO49=0;
    GpioDataRegs.GPBCLEAR.bit.GPIO49=1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO50=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO50=1;
    GpioCtrlRegs.GPBPUD.bit.GPIO50=0;
    GpioDataRegs.GPBCLEAR.bit.GPIO50=1;

    EDIS;

    EALLOW;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 12;   // XINT1��GPIO12
    EDIS;

    EALLOW; // �޸ı������ļĴ������޸�ǰӦ���EALLOW���
    PieVectTable.XINT1 = &EXTI1_IRQn;
    EDIS;   // EDIS����˼�ǲ������޸ı������ļĴ���

    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // ʹ��PIE��1��INT4

    XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // �½��ش����ж�
    XIntruptRegs.XINT1CR.bit.ENABLE= 1;        // ʹ��XINT1

    IER |= M_INT1;                              // ʹ��CPU�ж�1��INT1��
    EINT;                                       // ��ȫ���ж�
    ERTM;
}

interrupt void EXTI1_IRQn(void)
{
    EALLOW;
    DELAY_US(10000);
    //��1��ɨ��
    KEY_L1_SetL;
    KEY_L2_SetH;
    KEY_L3_SetH;

    if(KEY_H1==0){
        DELAY_US(10000);
        while(KEY_H1==0);
        flag_key = 1;
        key_code = 1;
        PieCtrlRegs.PIEACK.bit.ACK1=1;

        KEY_L1_SetH;
        KEY_L2_SetH;
        KEY_L3_SetH;
        return;
    }

    //��2��ɨ��
    KEY_L1_SetH;
    KEY_L2_SetL;
    KEY_L3_SetH;
    if(KEY_H1==0){
        DELAY_US(10000);
        while(KEY_H1==0);
        flag_key = 1;
        key_code = 2;
        PieCtrlRegs.PIEACK.bit.ACK1=1;

        KEY_L1_SetH;
        KEY_L2_SetH;
        KEY_L3_SetH;
        return;
    }

    //��3��ɨ��
    KEY_L1_SetH;
    KEY_L2_SetH;
    KEY_L3_SetL;

    if(KEY_H1==0){
        DELAY_US(10000);
        while(KEY_H1==0);
        flag_key = 1;
        key_code = 3;
        PieCtrlRegs.PIEACK.bit.ACK1=1;

        KEY_L1_SetH;
        KEY_L2_SetH;
        KEY_L3_SetH;
        return;
    }
//    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    EDIS;
}


void EXTI2_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // GPIO input clock
    EDIS;

    EALLOW;
    //KEY�˿�����
    GpioCtrlRegs.GPAMUX1.bit.GPIO13=0;
    GpioCtrlRegs.GPADIR.bit.GPIO13=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO13=0;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 2;        // �ⲿ�ж�2��XINT2�������޶�6����������
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0xFF;   // ÿ���������ڵ�����Ϊ510*SYSCLKOUT

    GpioCtrlRegs.GPBMUX2.bit.GPIO48=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO48=1;
    GpioCtrlRegs.GPBPUD.bit.GPIO48=0;
    GpioDataRegs.GPBCLEAR.bit.GPIO48=1;
    EDIS;

    EALLOW;
    GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 13;   // XINT2��GPIO13
    EDIS;

    EALLOW; // �޸ı������ļĴ������޸�ǰӦ���EALLOW���
    PieVectTable.XINT2 = &EXTI2_IRQn;
    EDIS;   // EDIS����˼�ǲ������޸ı������ļĴ���

    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // ʹ��PIE��1��INT5

    XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // �½��ش����ж�
    XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // ʹ��XINT2

    IER |= M_INT1;                              // ʹ��CPU�ж�1��INT1��
    EINT;                                       // ��ȫ���ж�
    ERTM;
}

interrupt void EXTI2_IRQn(void)
{
    Uint32 i;
    for(i=0;i<10000;i++);    //����������

    PieCtrlRegs.PIEACK.bit.ACK1=1;
}


