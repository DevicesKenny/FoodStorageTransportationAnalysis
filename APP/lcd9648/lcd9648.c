/*
 * lcd9648.c
 *
 *  Created on: 2018年10月15日
 *      Author: Administrator
 */

#include "lcd9648.h"
#include "charcode.h"

void LCD9648_SendData(unsigned char dat)
{
   unsigned char i;

   for(i=0; i<8; i++)
   {
      if( (dat&0x80)!=0 )
      {
    	  LCD9648_SDA_SETH;
      }
      else
      {
    	  LCD9648_SDA_SETL;
      }

      dat <<= 1;

      LCD9648_SCL_SETL;
      DELAY_US(1);
	  LCD9648_SCL_SETH;
	  DELAY_US(1);
   }
}

void LCD9648_WriteCmd(unsigned int cmd)
{
	LCD9648_CS_SETL;
	LCD9648_RS_SETL;
	DELAY_US(2);
	LCD9648_SendData(cmd);
	DELAY_US(2);
	LCD9648_CS_SETH;

}
void LCD9648_WriteData(unsigned int data)
{
	LCD9648_CS_SETL;
	LCD9648_RS_SETH;
	DELAY_US(2);
	LCD9648_SendData(data);
	DELAY_US(2);
	LCD9648_CS_SETH;
}

void LCD9648_GPIO_Init(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟
	GpioCtrlRegs.GPAPUD.all = 0;
	GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;

	GpioCtrlRegs.GPAMUX1.all = 0;   // 配置GPIO0-GPIO15为通用I/O口
	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;

	GpioCtrlRegs.GPADIR.all = 0x0FFF;// 配置GPIO0-GPIO11为输出引脚
	GpioCtrlRegs.GPBDIR.bit.GPIO60=1;

	GpioDataRegs.GPASET.bit.GPIO0 = 1;
	GpioDataRegs.GPBSET.bit.GPIO60 = 1;
	GpioDataRegs.GPASET.bit.GPIO1 = 1;
	GpioDataRegs.GPASET.bit.GPIO2 = 1;
	GpioDataRegs.GPASET.bit.GPIO3 = 1;
	EDIS;
}

void LCD9648_Init(void)
{
	LCD9648_GPIO_Init();

	LCD9648_CS_SETL;

	LCD9648_RSET_SETH;
	DELAY_US(10000);

	LCD9648_RSET_SETL;
	DELAY_US(10000);

	LCD9648_RSET_SETH;
	DELAY_US(10000);

	LCD9648_WriteCmd(0xe2);		  //软件复位
	LCD9648_WriteCmd(0xc8);		  //0xC8普通方向选择选择反向，0xC0为正常方向
	LCD9648_WriteCmd(0xa0);		  //0xA0段方向选择正常方向（0xA1为反方向
	LCD9648_WriteCmd(0x2f);
	LCD9648_WriteCmd(0x26);
	LCD9648_WriteCmd(0x81);		  //背景光对比度
	LCD9648_WriteCmd(0x10);
	LCD9648_WriteCmd(0xaf);		  //开启显示
}

void LCD9648_Clear(void)
{
	unsigned char i,j;

	for(i=0; i < 9; i++)
	{
		LCD9648_WriteCmd(0x40);
		LCD9648_WriteCmd(0xb0+i);
		LCD9648_WriteCmd(0x10);
		LCD9648_WriteCmd(0x00);

		for(j = 0; j < 96; j++)
		{
			LCD9648_WriteData(0x00);
		}
	}
}

/**
 * 显示汉字字符串（32位）
 */
unsigned char LCD9648_Write16CnCHAR(unsigned char  x, unsigned char y, unsigned char *cn)
{
	unsigned char j=0, x1=0, x2=0, wordNum=0;
	unsigned char num = 0;

	num = sizeof(CN16CHAR)/sizeof(CN16CHAR[0]);
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到96，大于则直接返回--//
	if(x > 96)
	{
		return 0;
	}
	y += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	LCD9648_WriteCmd(y);
	while ( *cn != '\0')	 //在C语言中字符串结束以‘\0’结尾
	{

		//--设置Y坐标--//
		LCD9648_WriteCmd(y);

		for (wordNum=0; wordNum<num; wordNum++)
		{
			//--设置Y坐标--//
			LCD9648_WriteCmd(y);

			x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
			x2 = x & 0x0F;          //去低四位
			//--设置X坐标--//
			LCD9648_WriteCmd(0x10 + x1);   //高4位
			LCD9648_WriteCmd(0x00 + x2);	//低4位

			//--查询要写的字在字库中的位置--//
			if (((CN16CHAR[wordNum].Index[0]&0xff) == (*cn)&0xff)               //修改：取字模的后两位（低16位）进行比较
				 &&((CN16CHAR[wordNum].Index[1]&0xff) == (*(cn+1)&0xff)))       //修改：取字模的后两位（低16位）进行比较
			{
				for (j=0; j<32; j++) //写一个字
				{
					if (j == 16)	 //由于16X16用到两个Y坐标，当大于等于16时，切换坐标
					{
						//--设置Y坐标--//
						LCD9648_WriteCmd(y + 1);

						//--设置X坐标--//
						LCD9648_WriteCmd(0x10 + x1);   //高4位
						LCD9648_WriteCmd(0x00 + x2);	//低4位
					}
					LCD9648_WriteData(CN16CHAR[wordNum].Msk[j]);
				}
				x += 16;
			}//if查到字结束
		}
		cn += 2;

	}	//while结束
	return 1;
}

/**
 * 显示字符字符串（数字或字母）
 */
unsigned char LCD9648_Write8CnCHAR(unsigned char  x, unsigned char y, unsigned char *cn)
{
    unsigned char j=0, x1=0, x2=0, wordNum=0;
    unsigned char num = 0;

    num = sizeof(CN8CHAR)/sizeof(CN8CHAR[0]);
    //--Y的坐标只能从0到7，大于则直接返回--//
    if(y > 7)
    {
        return 0;
    }

    //--X的坐标只能从0到96，大于则直接返回--//
    if(x > 96)
    {
        return 0;
    }
    y += 0xB0;     //求取Y坐标的值
    //--设置Y坐标--//
    LCD9648_WriteCmd(y);
    while ( *cn != '\0')     //在C语言中字符串结束以‘\0’结尾
    {

        //--设置Y坐标--//
        LCD9648_WriteCmd(y);

        for (wordNum=0; wordNum<num; wordNum++)
        {
            //--设置Y坐标--//
            LCD9648_WriteCmd(y);

            x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
            x2 = x & 0x0F;          //去低四位
            //--设置X坐标--//
            LCD9648_WriteCmd(0x10 + x1);   //高4位
            LCD9648_WriteCmd(0x00 + x2);    //低4位

            //--查询要写的字在字库中的位置--//
            if ((CN8CHAR[wordNum].Index[0]&0xff) == (*cn)&0xff)             //修改：取字模的后两位（低16位）进行比较
            {
                for (j=0; j<16; j++) //写一个字
                {
                    if (j == 8)     //由于16X16用到两个Y坐标，当大于等于16时，切换坐标
                    {
                        //--设置Y坐标--//
                        LCD9648_WriteCmd(y + 1);

                        //--设置X坐标--//
                        LCD9648_WriteCmd(0x10 + x1);   //高4位
                        LCD9648_WriteCmd(0x00 + x2);    //低4位
                    }
                    LCD9648_WriteData(CN8CHAR[wordNum].Msk[j]);
                }
                x += 8;
            }//if查到字结束
        }
        cn += 1;

    }   //while结束
    return 1;
}

void LCD9648_ClearTest(void)
{
	unsigned char i,j;

	for(i = 0; i < 6; i++)
	{
		LCD9648_WriteCmd(0x40);
		LCD9648_WriteCmd(0XB0+i);
		LCD9648_WriteCmd(0x10);
		LCD9648_WriteCmd(0x00);
		for(j = 0; j < 96; j++)
		{
			LCD9648_WriteData(0xff);
			DELAY_US(10000);
		}
	}

	for(i = 0; i < 7; i++)
	{
		LCD9648_WriteCmd(0XB0+i);
		LCD9648_WriteCmd(0x10);
		LCD9648_WriteCmd(0x00);
		for(j = 0; j < 96; j++)
		{
			LCD9648_WriteData(0x00);
			DELAY_US(10000);
		}
	}

}

