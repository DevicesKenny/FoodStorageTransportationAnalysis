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

//定时器0初始化函数
//Freq：CPU时钟频率（150MHz）
//Period：定时周期值，单位us
void TIM0_Init(float Freq, float Period)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
	EDIS;

	//设置定时器0的中断入口地址为中断向量表的INT0
	EALLOW;
	PieVectTable.TINT0 = &TIM0_IRQn;
	EDIS;

	//指向定时器0的寄存器地址
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	//设置定时器0的周期寄存器值
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	//设置定时器预定标计数器值为0
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	//确保定时器0为停止状态
	CpuTimer0Regs.TCR.bit.TSS = 1;
	//重载使能
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;

	ConfigCpuTimer(&CpuTimer0, Freq, Period);

	//开始定时器功能
	CpuTimer0Regs.TCR.bit.TSS=0;
	//开启CPU第一组中断并使能第一组中断的第7个小中断，即定时器0
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	//使能总中断
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

    //设置定时器1的中断入口地址为中断向量表的INT13
    EALLOW;
    PieVectTable.XINT13 = &TIM1_IRQn;
    EDIS;

    //指向定时器0的寄存器地址
    CpuTimer1.RegsAddr = &CpuTimer1Regs;
    //设置定时器1的周期寄存器值
    CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
    //设置定时器预定标计数器值为0
    CpuTimer1Regs.TPR.all  = 0;
    CpuTimer1Regs.TPRH.all = 0;
    //确保定时器1为停止状态
    CpuTimer1Regs.TCR.bit.TSS = 1;
    //重载使能
    CpuTimer1Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer1.InterruptCount = 0;

    ConfigCpuTimer(&CpuTimer1, Freq, Period);

    //开始定时器功能
    CpuTimer1Regs.TCR.bit.TSS=0;
    //开启中断，timer1无pie中断
    IER |= M_INT13;
    //使能总中断
    EINT;
    ERTM;
}

interrupt void TIM1_IRQn(void)
{
    EALLOW;
    //隔100ms扫描一次按键，进行按键中断
    key = KEY_Scan(0);
    EDIS;
}

void TIM2_Init(float Freq, float Period)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer 2
    EDIS;

    //设置定时器1的中断入口地址为中断向量表的INT14
    EALLOW;
    PieVectTable.TINT2 = &TIM2_IRQn;
    EDIS;

    //指向定时器0的寄存器地址
    CpuTimer2.RegsAddr = &CpuTimer2Regs;
    //设置定时器1的周期寄存器值
    CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
    //设置定时器预定标计数器值为0
    CpuTimer2Regs.TPR.all  = 0;
    CpuTimer2Regs.TPRH.all = 0;
    //确保定时器1为停止状态
    CpuTimer2Regs.TCR.bit.TSS = 1;
    //重载使能
    CpuTimer2Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer2.InterruptCount = 0;

    ConfigCpuTimer(&CpuTimer2, Freq, Period);

    //开始定时器功能
    CpuTimer2Regs.TCR.bit.TSS=0;
    //开启中断，timer2无pie中断
    IER |= M_INT13;
    //使能总中断
    EINT;
    ERTM;
}

interrupt void TIM2_IRQn(void)
{
    EALLOW;


    EDIS;
}

