/*
 * Dcmotor.h
 *
 *  Created on: Jan 8, 2023
 *      Author: Administrator
 */

#ifndef MOTOR_DCMOTOR_H_
#define MOTOR_DCMOTOR_H_


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define DC_MOTOR_INA_SETH           (GpioDataRegs.GPASET.bit.GPIO2=1)
#define DC_MOTOR_INA_SETL           (GpioDataRegs.GPACLEAR.bit.GPIO2=1)
#define DC_MOTOR_INB_SETH           (GpioDataRegs.GPASET.bit.GPIO3=1)
#define DC_MOTOR_INB_SETL           (GpioDataRegs.GPACLEAR.bit.GPIO3=1)

#define DC_MOTOR_INC_SETH           (GpioDataRegs.GPASET.bit.GPIO4=1)
#define DC_MOTOR_INC_SETL           (GpioDataRegs.GPACLEAR.bit.GPIO4=1)
#define DC_MOTOR_IND_SETH           (GpioDataRegs.GPASET.bit.GPIO5=1)
#define DC_MOTOR_IND_SETL           (GpioDataRegs.GPACLEAR.bit.GPIO5=1)

void DC_Motor_Init(void);
void leftRotate(void);
void rightRotate(void);
void stopRotate(void);

#endif /* MOTOR_DCMOTOR_H_ */
