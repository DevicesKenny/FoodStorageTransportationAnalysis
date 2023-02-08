/*
 * uart.c
 *
 *  Created on: 2018-1-31
 *      Author: Administrator
 */

#include "uart.h"
#include "leds.h"

interrupt void SCIA_RXIRQn(void);
interrupt void SCIB_RXIRQn(void);

char *msg;
char SCIA_RxBUF[UARTa_LEN];
char SCIB_RxBUF[UARTb_LEN];

unsigned char Flag_Uarta_Receive;
unsigned char Flag_Uartb_Receive;
unsigned char Flag_Uartb_Receive_OK = 0;
Uint16 Uarta_STA = 0;
Uint16 Uartb_STA = 0;

/**
 * ����A��ʼ��
 */
void UARTa_Init(Uint32 baud)
{
	unsigned char scihbaud=0;
	unsigned char scilbaud=0;
	Uint16 scibaud=0;

	scibaud=37500000/(8*baud)-1;
	scihbaud=scibaud>>8;
	scilbaud=scibaud&0xff;

	EALLOW;
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
    EDIS;

	InitSciaGpio();

    //����SCI����A�ж�ʹ��
    EALLOW;
    PieVectTable.SCIRXINTA = &SCIA_RXIRQn;
    EDIS;

	//Initalize the SCI FIFO
	SciaRegs.SCIFFTX.all=0xE040;//0xE040//0xC04F
	SciaRegs.SCIFFRX.all=0x2041;//0x204f//0x406F
	SciaRegs.SCIFFRX.bit.RXFFIENA = 1;
	SciaRegs.SCIFFCT.all=0x0;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;

	// Note: Clocks were turned on to the SCIA peripheral
	// in the InitSysCtrl() function
	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
								   // No parity,8 char bits,
								   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all =0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
	SciaRegs.SCIHBAUD    =scihbaud;  // 9600 baud @LSPCLK = 37.5MHz.
	SciaRegs.SCILBAUD    =scilbaud;
//	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset


	//�����жϣ���9�е�1��С�ж�
	IER |= M_INT9;          //ʹ�ܵ�9��
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;//ѡ��RX�ж�
	//ʹ�����ж�
	EINT;
	ERTM;
}

/**
 * ����A�жϺ���
 */
interrupt void SCIA_RXIRQn(void)
{
    Uint16 ReceivedChar;

    EALLOW;

    //LED7_TOGGLE;//SciaRegs.SCIFFRX.bit.RXFFST !=1  if(ScicRegs.SCIRXST.bit.RXRDY== 0)
    if(SciaRegs.SCIFFRX.bit.RXFFST != 0){
        ReceivedChar = SciaRegs.SCIRXBUF.all;
        SCIA_RxBUF[Uarta_STA] = ReceivedChar;
        UARTa_SendByte(ReceivedChar);
        if((SCIA_RxBUF[Uarta_STA] == 0x0a) && (SCIA_RxBUF[Uarta_STA-1] == 0x0d)){
            Flag_Uarta_Receive = 1;
        }
        Uarta_STA++;
        if(Uarta_STA > (UARTa_LEN-1)){
            Uarta_STA = 0;
        }
    }

    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;    //����жϱ�־λ
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    EDIS;
}

// Transmit a character from the SCI'
/**
 * ����A���͵����ֽ�
 */
void UARTa_SendByte(int a)
{
	while (SciaRegs.SCIFFTX.bit.TXFFST != 0);
	SciaRegs.SCITXBUF=a;
}

/**
 * ����A�����ַ���
 */
void UARTa_SendString(char * msg)
{
	int i=0;

	while(msg[i] != '\0')
	{
		UARTa_SendByte(msg[i]);
		i++;
	}
}

/**
 * ����A���ݽ������
 */
void UARTa_Clear(void)
{
    int j ;
    for(j = 0; j < UARTa_LEN; j++){
        SCIA_RxBUF[j] = '\0';
    }
    Uarta_STA = 0;
}

/**
 * ����B��ʼ��
 */
void UARTb_Init(Uint32 baud)
{

    unsigned char scihbaud=0;
    unsigned char scilbaud=0;
    Uint16 scibaud=0;

    scibaud=37500000/(8*baud)-1;
    scihbaud=scibaud>>8;
    scilbaud=scibaud&0xff;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   // SCI-B
    EDIS;

    InitScibGpio();

    //����SCI����B�ж�ʹ��
    EALLOW;
    PieVectTable.SCIRXINTB = &SCIB_RXIRQn;
    EDIS;

    //Initalize the SCI FIFO
    ScibRegs.SCIFFTX.all=0xE040;//0xE040//0xC04F
    ScibRegs.SCIFFRX.all=0x2041;//0x204f//0x406F
    ScibRegs.SCIFFRX.bit.RXFFIENA = 1;
    ScibRegs.SCIFFCT.all=0x0;
    ScibRegs.SCIFFRX.bit.RXFIFORESET=1;
//    ScibRegs.SCICTL1.bit.SWRESET=1;
//    ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;//����ʹ�ܷ���FIFO
//    ScibRegs.SCIFFRX.bit.RXFIFORESET=1;//����ʹ�ܽ���FIFO

    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    ScibRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL2.all =0x0003;
    ScibRegs.SCICTL2.bit.TXINTENA =1;
    ScibRegs.SCICTL2.bit.RXBKINTENA =1;
    ScibRegs.SCIHBAUD    =scihbaud;  // 9600 baud @LSPCLK = 37.5MHz.
    ScibRegs.SCILBAUD    =scilbaud;
//  ScibRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
    ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset


    //�����жϣ���9�е�3��С�ж�
    IER |= M_INT9;          //ʹ�ܵ�9��
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;//ѡ��RX�ж�
    //ʹ�����ж�
    EINT;
    ERTM;
}

/**
 * ����B�жϺ���
 */
interrupt void SCIB_RXIRQn(void)
{
    Uint16 ReceivedChar;
    //UARTa_SendString("Test!!!!\r\n");
    EALLOW;

//    LED4_TOGGLE;//SciaRegs.SCIFFRX.bit.RXFFST !=1  if(ScicRegs.SCIRXST.bit.RXRDY== 0)
//    while (ScibRegs.SCIFFRX.bit.RXFFST != 0)// wait for XRDY =1 for empty state
//    {
//        ReceivedChar = ScibRegs.SCIRXBUF.all;
//        SCIB_RxBUF[i] = ReceivedChar;
//        i++;
//    }
//    for(j = 0; j < i; j++){
//        UARTa_SendByte(SCIB_RxBUF[j]);
//    }
//
//    DELAY_US(1000);
//
//    ScibRegs.SCIFFRX.bit.RXFIFORESET = 0;
//    ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;

    if(ScibRegs.SCIFFRX.bit.RXFFST != 0){
        ReceivedChar = ScibRegs.SCIRXBUF.all;
        SCIB_RxBUF[Uartb_STA] = ReceivedChar;
        UARTa_SendByte(ReceivedChar);           //����͸��
        if(SCIB_RxBUF[Uartb_STA] == 'K'){
            if(SCIB_RxBUF[Uartb_STA-1] == 'O'){
                Flag_Uartb_Receive_OK = 1;
            }
        }
        Uartb_STA++;
        if(Uartb_STA > (UARTb_LEN - 1)){
            Uartb_STA = 0;
        }
    }
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;    //����жϱ�־λ
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    EDIS;
}

// Transmit a character from the SCI'
/**
 * ����B���͵����ֽ�
 */
void UARTb_SendByte(int a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST != 0);
    ScibRegs.SCITXBUF=a;
}

/**
 * ����B�����ַ���
 */
void UARTb_SendString(char * msg)
{
    int i=0;

    while(msg[i] != '\0')
    {
        UARTb_SendByte(msg[i]);
        i++;
    }
}

/**
 * ����B������յ�������
 */
void UARTb_Clear(void)
{
    unsigned int j ;
    for(j = 0; j < UARTb_LEN; j++){
        SCIB_RxBUF[j] = '\0';
    }
    Uartb_STA = 0;
}

