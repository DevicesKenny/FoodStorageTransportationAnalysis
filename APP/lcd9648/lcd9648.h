/*
 * lcd9648.h
 *
 *  Created on: 2018年10月15日
 *      Author: Administrator
 */

#ifndef _LCD9648_H_
#define _LCD9648_H_



#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件


#define LCD9648_SCL_SETH 	(GpioDataRegs.GPASET.bit.GPIO0=1)
#define LCD9648_SCL_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO0=1)

#define LCD9648_SDA_SETH 	(GpioDataRegs.GPBSET.bit.GPIO60=1)
#define LCD9648_SDA_SETL 	(GpioDataRegs.GPBCLEAR.bit.GPIO60=1)

#define LCD9648_RS_SETH 	(GpioDataRegs.GPASET.bit.GPIO1=1)
#define LCD9648_RS_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO1=1)

#define LCD9648_CS_SETH 	(GpioDataRegs.GPASET.bit.GPIO2=1)
#define LCD9648_CS_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO2=1)

#define LCD9648_RSET_SETH 	(GpioDataRegs.GPASET.bit.GPIO3=1)
#define LCD9648_RSET_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO3=1)


//#define LCD9648_SCL_SETH 	(GpioDataRegs.GPASET.bit.GPIO8=1)
//#define LCD9648_SCL_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO8=1)
//
//#define LCD9648_SDA_SETH 	(GpioDataRegs.GPASET.bit.GPIO7=1)
//#define LCD9648_SDA_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO7=1)
//
//#define LCD9648_RS_SETH 	(GpioDataRegs.GPASET.bit.GPIO6=1)
//#define LCD9648_RS_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO6=1)
//
//#define LCD9648_CS_SETH 	(GpioDataRegs.GPASET.bit.GPIO5=1)
//#define LCD9648_CS_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO5=1)
//
//#define LCD9648_RSET_SETH 	(GpioDataRegs.GPASET.bit.GPIO4=1)
//#define LCD9648_RSET_SETL 	(GpioDataRegs.GPACLEAR.bit.GPIO4=1)


void LCD9648_WriteCmd(unsigned int cmd);
void LCD9648_WriteData(unsigned int data);
void LCD9648_Init(void);
void LCD9648_Clear(void);
unsigned char LCD9648_Write16CnCHAR(unsigned char  x, unsigned char y, unsigned char *cn);
void LCD9648_ClearTest(void);

unsigned char LCD9648_Write8CnCHAR(unsigned char  x, unsigned char y, unsigned char *cn);

#endif /* APP_LCD9648_LCD9648_H_ */
