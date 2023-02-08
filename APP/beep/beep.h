/*
 * beep.h
 *
 *  Created on: Jan 7, 2023
 *      Author: Administrator
 */

#ifndef BEEP_H_
#define BEEP_H_

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


#define BEEP_ON         (GpioDataRegs.GPASET.bit.GPIO6=1)
#define BEEP_OFF        (GpioDataRegs.GPACLEAR.bit.GPIO6=1)
#define BEEP_TOGGLE     (GpioDataRegs.GPATOGGLE.bit.GPIO6=1)

void BEEP_Init(void);
void BEEP_Play(void);
void BEEP_STOP(void);

extern int count_beep;

#endif /* BEEP_H_ */
