/*
 * lcd9648.c
 *
 *  Created on: 2018��10��15��
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
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��
	GpioCtrlRegs.GPAPUD.all = 0;
	GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;

	GpioCtrlRegs.GPAMUX1.all = 0;   // ����GPIO0-GPIO15Ϊͨ��I/O��
	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;

	GpioCtrlRegs.GPADIR.all = 0x0FFF;// ����GPIO0-GPIO11Ϊ�������
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

	LCD9648_WriteCmd(0xe2);		  //�����λ
	LCD9648_WriteCmd(0xc8);		  //0xC8��ͨ����ѡ��ѡ����0xC0Ϊ��������
	LCD9648_WriteCmd(0xa0);		  //0xA0�η���ѡ����������0xA1Ϊ������
	LCD9648_WriteCmd(0x2f);
	LCD9648_WriteCmd(0x26);
	LCD9648_WriteCmd(0x81);		  //������Աȶ�
	LCD9648_WriteCmd(0x10);
	LCD9648_WriteCmd(0xaf);		  //������ʾ
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
 * ��ʾ�����ַ�����32λ��
 */
unsigned char LCD9648_Write16CnCHAR(unsigned char  x, unsigned char y, unsigned char *cn)
{
	unsigned char j=0, x1=0, x2=0, wordNum=0;
	unsigned char num = 0;

	num = sizeof(CN16CHAR)/sizeof(CN16CHAR[0]);
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y > 7)
	{
		return 0;
	}

	//--X������ֻ�ܴ�0��96��������ֱ�ӷ���--//
	if(x > 96)
	{
		return 0;
	}
	y += 0xB0;	   //��ȡY�����ֵ
	//--����Y����--//
	LCD9648_WriteCmd(y);
	while ( *cn != '\0')	 //��C�������ַ��������ԡ�\0����β
	{

		//--����Y����--//
		LCD9648_WriteCmd(y);

		for (wordNum=0; wordNum<num; wordNum++)
		{
			//--����Y����--//
			LCD9648_WriteCmd(y);

			x1 = (x >> 4) & 0x0F;   //����X����Ҫ��������ָߵ�4λ������������ȡ����4λ
			x2 = x & 0x0F;          //ȥ����λ
			//--����X����--//
			LCD9648_WriteCmd(0x10 + x1);   //��4λ
			LCD9648_WriteCmd(0x00 + x2);	//��4λ

			//--��ѯҪд�������ֿ��е�λ��--//
			if (((CN16CHAR[wordNum].Index[0]&0xff) == (*cn)&0xff)               //�޸ģ�ȡ��ģ�ĺ���λ����16λ�����бȽ�
				 &&((CN16CHAR[wordNum].Index[1]&0xff) == (*(cn+1)&0xff)))       //�޸ģ�ȡ��ģ�ĺ���λ����16λ�����бȽ�
			{
				for (j=0; j<32; j++) //дһ����
				{
					if (j == 16)	 //����16X16�õ�����Y���꣬�����ڵ���16ʱ���л�����
					{
						//--����Y����--//
						LCD9648_WriteCmd(y + 1);

						//--����X����--//
						LCD9648_WriteCmd(0x10 + x1);   //��4λ
						LCD9648_WriteCmd(0x00 + x2);	//��4λ
					}
					LCD9648_WriteData(CN16CHAR[wordNum].Msk[j]);
				}
				x += 16;
			}//if�鵽�ֽ���
		}
		cn += 2;

	}	//while����
	return 1;
}

/**
 * ��ʾ�ַ��ַ��������ֻ���ĸ��
 */
unsigned char LCD9648_Write8CnCHAR(unsigned char  x, unsigned char y, unsigned char *cn)
{
    unsigned char j=0, x1=0, x2=0, wordNum=0;
    unsigned char num = 0;

    num = sizeof(CN8CHAR)/sizeof(CN8CHAR[0]);
    //--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
    if(y > 7)
    {
        return 0;
    }

    //--X������ֻ�ܴ�0��96��������ֱ�ӷ���--//
    if(x > 96)
    {
        return 0;
    }
    y += 0xB0;     //��ȡY�����ֵ
    //--����Y����--//
    LCD9648_WriteCmd(y);
    while ( *cn != '\0')     //��C�������ַ��������ԡ�\0����β
    {

        //--����Y����--//
        LCD9648_WriteCmd(y);

        for (wordNum=0; wordNum<num; wordNum++)
        {
            //--����Y����--//
            LCD9648_WriteCmd(y);

            x1 = (x >> 4) & 0x0F;   //����X����Ҫ��������ָߵ�4λ������������ȡ����4λ
            x2 = x & 0x0F;          //ȥ����λ
            //--����X����--//
            LCD9648_WriteCmd(0x10 + x1);   //��4λ
            LCD9648_WriteCmd(0x00 + x2);    //��4λ

            //--��ѯҪд�������ֿ��е�λ��--//
            if ((CN8CHAR[wordNum].Index[0]&0xff) == (*cn)&0xff)             //�޸ģ�ȡ��ģ�ĺ���λ����16λ�����бȽ�
            {
                for (j=0; j<16; j++) //дһ����
                {
                    if (j == 8)     //����16X16�õ�����Y���꣬�����ڵ���16ʱ���л�����
                    {
                        //--����Y����--//
                        LCD9648_WriteCmd(y + 1);

                        //--����X����--//
                        LCD9648_WriteCmd(0x10 + x1);   //��4λ
                        LCD9648_WriteCmd(0x00 + x2);    //��4λ
                    }
                    LCD9648_WriteData(CN8CHAR[wordNum].Msk[j]);
                }
                x += 8;
            }//if�鵽�ֽ���
        }
        cn += 1;

    }   //while����
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

