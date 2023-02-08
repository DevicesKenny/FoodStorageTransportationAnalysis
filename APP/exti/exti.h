/*
 * exti.h
 *
 *  Created on: Jan 9, 2023
 *      Author: Administrator
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void EXTI1_Init(void);
interrupt void EXTI1_IRQn(void);

void EXTI2_Init(void);
interrupt void EXTI2_IRQn(void);

void EXTI3_Init(void);


#endif /* EXTI_H_ */
