/*
 * time.h
 *
 *  Created on: 2018-1-24
 *      Author: Administrator
 */

#ifndef TIME_H_
#define TIME_H_

#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件
#include "key.h"
#include "beep.h"

void TIM0_Init(float Freq, float Period);
interrupt void TIM0_IRQn(void);

void TIM1_Init(float Freq, float Period);
interrupt void TIM1_IRQn(void);

void TIM2_Init(float Freq, float Period);
interrupt void TIM2_IRQn(void);

extern int count_5s;
extern int flag_5s;
extern int count_2s;
extern int flag_2s;

#endif /* TIME_H_ */
