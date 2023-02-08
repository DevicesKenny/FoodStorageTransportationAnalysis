/*
 * sht30.h
 *
 *  Created on: Jan 20, 2023
 *      Author: Administrator
 */

#ifndef APP_SHT30_SHT30_H_
#define APP_SHT30_SHT30_H_

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "iic.h"

void SHT30_Init(void);
void READ_IIC_B_DATA(float *Temperature,float *Humidity);
void Measure_Peroid_Mode(void);
#endif /* APP_SHT30_SHT30_H_ */
