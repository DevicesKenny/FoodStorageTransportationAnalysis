/*
 * main.c
 *
 *  Created on: Jan 7, 2023
 *      Author: Administrator
 *      EXTIʵ�ְ����жϣ���LCD9648����ʾ
 */

//#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
//#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "leds.h"
#include "time.h"
#include "uart.h"
#include "lcd9648.h"
#include "key.h"
#include "exti.h"
#include "sht30.h"
#include "beep.h"
#include "Dcmotor.h"
#include "wifi.h"

#define dataNum_page                2       //�����ռ���Ҫ��ʾ��ҳ��
#define settingsNum_page            2       //���ý�����Ҫ��ʾ��ҳ��

#define temp_humi_interface         1       //��ʪ���ռ�����
#define GPS_interface               2       //GPS�ռ�����

#define tempSettings_thre_upper     1       //�¶��������޽���
#define tempSettings_thre_lower     2       //�¶��������޽���

#define Stop_Rotate                 0       //����ֹͣ
#define Left_Rotate                 1       //������ת
#define Right_Rotate                2       //������ת

#define Temperature_Upper           40
#define Temperature_Lower           0

extern char SCIA_RxBUF[UARTa_LEN];  //����a���ݽ�������
extern char SCIB_RxBUF[UARTb_LEN];  //����b���ݽ�������
extern char key;                    //����ɨ�谴��ֵ

float temperature=0;                //��⵱ǰ���¶�ֵ
float humidity=0;                   //��⵱ǰ��ʪ��ֵ
char temp[6] = {0,0,0,0,0,0};       //�¶��ַ��ͽ���ת��
char humi[6] = {0,0,0,0,0,0};       //ʪ���ַ��ͽ���ת��

int threshold_upper = 30;           //�¶�������ֵ
int threshold_lower = 25;           //�¶�������ֵ
int threshold_upper_temp = 30;       //�¶�����Ԥ��ֵ
int threshold_lower_temp = 25;       //�¶�����Ԥ��ֵ
char thre_upper[3] = {0,0,0};       //�¶���������ת���洢
char thre_lower[3] = {0,0,0};       //�¶���������ת���洢

int key_lcd = 0;                    //������Ļ�л���Ĭ��Ϊ�ռ�������ʾ���棩   0��������ʾ�ռ�����      1���¶���ֵ���ý���
int data_page = 1;                  //��ǰ������ʾҳ��
int settings_page = 1;              //��ǰ���ý�����ʾҳ��

int fan_rotate = 0;                //����ת�ķ���

void key_operation(void);
void show_data(int page);
void show_settings(int page);
void ThresholdAdd(int page);
void ThresholdSub(int page);
void autoAdjust(void);
void fan_operation(int index);

char send_data[1000];

/*******************************************************************************
* �� �� ��         : delay
* ��������         : ��ʱ������ͨ��ѭ��ռ��CPU���ﵽ��ʱ����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void delay(void)
{
    Uint16      i;
    Uint32      j;
    for(i=0;i<32;i++)
        for (j = 0; j < 100000; j++);
}


/*******************************************************************************
* �� �� ��         : main
* ��������         : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void main(){
    char *data_Json;

//    char test[1000];

    InitSysCtrl();//ϵͳʱ�ӳ�ʼ����Ĭ���ѿ���F28335��������ʱ��

    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    //���ƶ�ʱ�����д����FLASH���ô��뵽RAM��
    // ����FLASH��ʼ������ InitFlash();
    // ���Ӻ󽫲��� RamfuncsLoadStart, RamfuncsLoadEnd, ��RamfuncsRunStart
    // ����. ��ο� F28335.cmd �ļ�
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    // ����FLASH��ʼ������������flash�ȴ�״̬
    // �������������RAM������
    InitFlash();

    DC_Motor_Init();
    KEY_Init();
    LED_Init();
    TIM0_Init(150,500000);
    TIM1_Init(150,100000);
    UARTa_Init(9600);
    UARTb_Init(9600);
    LCD9648_Init();
    IICB_Init();
    SHT30_Init();
    BEEP_Init();

    LCD9648_Clear();
    flag_2s=1;                      //ֱ�ӿ�����һ�������ռ�
    Connect_wifi();

    sprintf(send_data,"AT+MQTTPUB=0,\"$thing/up/property/%s/%s\",\"data_Json\",1,0",product_id,device_name);
    data_Json = "{\\\"method\\\":\\\"report\\\"\\\,"       //19
                "\\\"clientToken\\\":\\\"123\\\"\\\,"      //20
                "\\\"params\\\":{"                  //10
                    "\\\"temp\\\":0 \\\,"              //10
                    "\\\"humi\\\":0 \\\,"              //10
                    "\\\"threshold_upper\\\":0 \\\,"   //21
                    "\\\"threshold_lower\\\":0 \\\,"   //21
                    "\\\"fan\\\":0\\\,"                //8
                    "\\\"control_mode\\\":0"       //17
//                    "\\\"GPS_Info\\\":{"            //12
//                        "\\\"longtitude\\\":0    \\\,"  //19
//                        "\\\"latitude\\\":0    "     //16
//                        "}"                     //1
                    "}"                         //1
                "}";
    sprintf(&send_data[62],"%s\",1,0\r\n",data_Json);

    while(1)
    {
        //ÿ��2s�����ݽ���һ���ռ����ϴ�
        if(flag_2s){
            flag_2s = 0;
            LCD9648_Clear();
            //��������ģʽ����ʼ��
            Measure_Peroid_Mode();
            //IIC������
            READ_IIC_B_DATA(&temperature,&humidity);

            temp[0] = (int)temperature/10+'0';
            temp[1] = (int)temperature%10+'0';
            UARTa_SendString("temperature���¶ȣ�: ");
            UARTa_SendString(temp);
            UARTa_SendString("��   \r\n");

            humi[0] = (int)humidity/10+'0';
            humi[1] = (int)humidity%10+'0';
            UARTa_SendString("humidity��ʪ�ȣ�: ");
            UARTa_SendString(humi);
            UARTa_SendString("%   \r\n\r\n");
            send_data[132] = temp[0];
            send_data[133] = temp[1];
            send_data[145] = humi[0];
            send_data[146] = humi[1];
//            sprintf(&send_data[84],humi);
            sendData();
//            sprintf(test,"AT+MQTTPUB=0,\"$thing/up/property/%s/%s\",\"data_Json\",1,0",product_id,device_name);
        }
        if(!key_lcd){           //key_lcd==0,��ʾ�����ռ�����
            show_data(data_page);
        }
        if(key_lcd){                  //key_lcd==1,�������ý���
            show_settings(settings_page);
        }
        key_operation();        //���о��󰴼���ز���

        //autoAdjust();

    }
}

/**
 * �����ж���ز���
 */
void key_operation(void)
{
    //��������Ļ�����л�
    if(key == KEY1_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        key_lcd = !key_lcd;
    }

    //������Ļ����
    if(key == KEY2_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        if(!key_lcd){                       //��ǰlcd������ʾ����ҳ
            if(data_page > 1){              //ҳ����������Ϊһҳ
                --data_page;
            }
            show_data(data_page);
        }else{                              //��ǰlcd������ʾ��ֵ����ҳ
            if(settings_page > 1){          //ҳ����������Ϊ1ҳ
                --settings_page;
            }
            show_settings(settings_page);
        }
    }

    if(key == KEY3_PRESS){
        key = KEY_UNPRESS;
    }

    //�������¶���ֵ�����ϵ�
    if(key == KEY4_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        if(threshold_lower<=threshold_upper){   //�������ж�����
            ThresholdAdd(settings_page);
        }
        show_settings(settings_page);

    }

    if(key == KEY5_PRESS){
        key = KEY_UNPRESS;
    }

    //�������¶���ֵ�����µ�
    if(key == KEY6_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        if(threshold_lower<=threshold_upper){
            ThresholdSub(settings_page);
        }
        show_settings(settings_page);
    }

    //testing------��������
    if(key == KEY7_PRESS){
        key = KEY_UNPRESS;
        BEEP_Play();
    }

    //������Ļ����
    if(key == KEY8_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        if(!key_lcd){                       //��ǰlcd������ʾ����ҳ
            if(data_page < dataNum_page){
                ++data_page;
                show_data(data_page);
            }else{
                data_page = dataNum_page;
            }
        }else{                              //��ǰlcd������ʾ��ֵ����ҳ
            if(settings_page < settingsNum_page){
                ++settings_page;
                show_settings(settings_page);
            }else{
                settings_page = settingsNum_page;
            }
        }
    }

    //testing---����ת
    if(key == KEY9_PRESS){
        key = KEY_UNPRESS;
    }
}

/**
 * lcd9648��ʾ���ݽ���
 */
void show_data(int page)            //page <= dataNum_page
{
    if(page == temp_humi_interface){
       LCD9648_Write16CnCHAR(10,0,"�¶ȣ�");
       LCD9648_Write8CnCHAR(60,0,(unsigned char*)temp);
       LCD9648_Write16CnCHAR(80,0,"��");
       LCD9648_Write16CnCHAR(10,3,"ʪ�ȣ�");
       LCD9648_Write8CnCHAR(60,3,(unsigned char*)humi);
       LCD9648_Write8CnCHAR(80,3,"%");
    }
    if(page == GPS_interface){
       LCD9648_Write8CnCHAR(10,0,"GPS:");
    }
}

/**
 * lcd9648��ʾ��ֵ���ý���
 */
void show_settings(int page)        //page <= settingsNum_page
{
    if(page == tempSettings_thre_upper){
        LCD9648_Write16CnCHAR(20,0,"��ֵ����");
        LCD9648_Write16CnCHAR(10,3,"���ޣ�");

        thre_upper[0] = (int)threshold_upper/10+'0';
        thre_upper[1] = (int)threshold_upper%10+'0';

        LCD9648_Write8CnCHAR(60,3,(unsigned char*)thre_upper);
        LCD9648_Write16CnCHAR(80,3,"��");
    }
    if(page == tempSettings_thre_lower){
        LCD9648_Write16CnCHAR(20,0,"��ֵ����");
        LCD9648_Write16CnCHAR(10,3,"���ޣ�");

        thre_lower[0] = (int)threshold_lower/10+'0';
        thre_lower[1] = (int)threshold_lower%10+'0';

        LCD9648_Write8CnCHAR(60,3,(unsigned char*)thre_lower);
        LCD9648_Write16CnCHAR(80,3,"��");
    }
}

/**
 * ������ֵ��������
 */
void ThresholdAdd(int page)
{
    if(page == tempSettings_thre_upper){      //�¶�����������ֵҳ��
        threshold_upper++;
    }
    if(page == tempSettings_thre_lower){      //�¶�����������ֵҳ��
        threshold_lower++;
    }
    if(threshold_upper < threshold_lower){      //���ݱȽϲ��淶���������������ʱ������++������>���ޣ��������޻ص�
        if(page == tempSettings_thre_lower){
            threshold_lower--;
        }
    }
    if(threshold_upper > Temperature_Upper){
        threshold_upper--;
    }
    if(threshold_lower < Temperature_Lower){
        threshold_lower++;
    }
}

/**
 * ������ֵ�ݼ�����
 */
void ThresholdSub(int page)
{
    if(page == tempSettings_thre_upper){
        threshold_upper--;
    }
    if(page == tempSettings_thre_lower){
        threshold_lower--;
    }
    if(threshold_upper < threshold_lower){      //���ݱȽϲ��淶���������������ʱ������--������<���ޣ��������޻ص�
        if(page == tempSettings_thre_upper){
            threshold_upper++;
        }
    }
    if(threshold_upper > Temperature_Upper){
            threshold_upper--;
    }
    if(threshold_lower < Temperature_Lower){
        threshold_lower++;
    }
}

/**
 * �Զ����ڲ���
 *  �� ��������ֵ�м�---����������Ӧ����led����               //led7����������
 *  �� ������ֵ���ޣ��������£���������2s��������������������    //led6����������
 *  �� ������ֵ���ޣ����н��£���������2s������������������ת��   //�����������
 */
void autoAdjust(void)
{
    if((((int)temperature) <= threshold_upper) && (((int)temperature) >= threshold_lower)){
        LED7_ON;
        LED6_OFF;
        stopRotate();
    }

    else if(((int)temperature) < threshold_lower){
        LED7_OFF;
        LED6_ON;
        stopRotate();
    }

    else if(((int)temperature) > threshold_upper){
        LED6_OFF;
        LED7_OFF;
        leftRotate();
    }

}

/**
 * ���ȴ�����ز���
 */
void fan_operation(int index)
{
    if(index%3 == Stop_Rotate){
        stopRotate();
    }
    if(index%3 == Left_Rotate){
        leftRotate();
    }
    if(index%3 == Right_Rotate){
        rightRotate();
    }
}
