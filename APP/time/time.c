/*
 * time.c
 *
 *  Created on: 2018-1-24
 *      Author: Administrator
 */


#include "time.h"
#include "leds.h"

int count_5s = 10;
int flag_5s = 0;
int count_2s = 4;
int flag_2s = 0;

char key = KEY_UNPRESS;

//��ʱ��0��ʼ������
//Freq��CPUʱ��Ƶ�ʣ�150MHz��
//Period����ʱ����ֵ����λus
void TIM0_Init(float Freq, float Period)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
	EDIS;

	//���ö�ʱ��0���ж���ڵ�ַΪ�ж��������INT0
	EALLOW;
	PieVectTable.TINT0 = &TIM0_IRQn;
	EDIS;

	//ָ��ʱ��0�ļĴ�����ַ
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	//���ö�ʱ��0�����ڼĴ���ֵ
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	//���ö�ʱ��Ԥ���������ֵΪ0
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	//ȷ����ʱ��0Ϊֹͣ״̬
	CpuTimer0Regs.TCR.bit.TSS = 1;
	//����ʹ��
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;

	ConfigCpuTimer(&CpuTimer0, Freq, Period);

	//��ʼ��ʱ������
	CpuTimer0Regs.TCR.bit.TSS=0;
	//����CPU��һ���жϲ�ʹ�ܵ�һ���жϵĵ�7��С�жϣ�����ʱ��0
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	//ʹ�����ж�
	EINT;
	ERTM;

}

interrupt void TIM0_IRQn(void)
{
	EALLOW;
	count_2s--;
	if(count_2s == 0){
	    count_2s = 4;
	    flag_2s = 1;
	}
	LED2_TOGGLE;
	PieCtrlRegs.PIEACK.bit.ACK1=1;
	EDIS;
}

void TIM1_Init(float Freq, float Period)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
    EDIS;

    //���ö�ʱ��1���ж���ڵ�ַΪ�ж��������INT13
    EALLOW;
    PieVectTable.XINT13 = &TIM1_IRQn;
    EDIS;

    //ָ��ʱ��0�ļĴ�����ַ
    CpuTimer1.RegsAddr = &CpuTimer1Regs;
    //���ö�ʱ��1�����ڼĴ���ֵ
    CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
    //���ö�ʱ��Ԥ���������ֵΪ0
    CpuTimer1Regs.TPR.all  = 0;
    CpuTimer1Regs.TPRH.all = 0;
    //ȷ����ʱ��1Ϊֹͣ״̬
    CpuTimer1Regs.TCR.bit.TSS = 1;
    //����ʹ��
    CpuTimer1Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer1.InterruptCount = 0;

    ConfigCpuTimer(&CpuTimer1, Freq, Period);

    //��ʼ��ʱ������
    CpuTimer1Regs.TCR.bit.TSS=0;
    //�����жϣ�timer1��pie�ж�
    IER |= M_INT13;
    //ʹ�����ж�
    EINT;
    ERTM;
}

interrupt void TIM1_IRQn(void)
{
    EALLOW;
    //��100msɨ��һ�ΰ��������а����ж�
    key = KEY_Scan(0);
    EDIS;
}

void TIM2_Init(float Freq, float Period)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer 2
    EDIS;

    //���ö�ʱ��1���ж���ڵ�ַΪ�ж��������INT14
    EALLOW;
    PieVectTable.TINT2 = &TIM2_IRQn;
    EDIS;

    //ָ��ʱ��0�ļĴ�����ַ
    CpuTimer2.RegsAddr = &CpuTimer2Regs;
    //���ö�ʱ��1�����ڼĴ���ֵ
    CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
    //���ö�ʱ��Ԥ���������ֵΪ0
    CpuTimer2Regs.TPR.all  = 0;
    CpuTimer2Regs.TPRH.all = 0;
    //ȷ����ʱ��1Ϊֹͣ״̬
    CpuTimer2Regs.TCR.bit.TSS = 1;
    //����ʹ��
    CpuTimer2Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer2.InterruptCount = 0;

    ConfigCpuTimer(&CpuTimer2, Freq, Period);

    //��ʼ��ʱ������
    CpuTimer2Regs.TCR.bit.TSS=0;
    //�����жϣ�timer2��pie�ж�
    IER |= M_INT13;
    //ʹ�����ж�
    EINT;
    ERTM;
}

interrupt void TIM2_IRQn(void)
{
    EALLOW;


    EDIS;
}

