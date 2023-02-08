/*
 * wifi.c
 *
 *  Created on: Feb 1, 2023
 *      Author: Administrator
 */

#include "wifi.h"
#include "uart.h"

char send_at[1000];        //�洢���ڷ�������
extern char send_data[1000];


void Connect_wifi(void)
{
//    UARTb_SendString("AT+UART=9600,8,1,0,0\r\n");
//    wait_OK();

    UARTb_SendString("AT+RST\r\n");//AT����ָ��
    DELAY_US(6000000);

    Flag_Uartb_Receive_OK = 0;
    UARTb_SendString("AT+CWMODE=3\r\n");//  ���ù���ģʽ
    wait_OK();

    sprintf(send_at,"AT+CWJAP=\"%s\",\"%s\"\r\n",wifi_name,wifi_password);  //����wifi
    UARTb_SendString(send_at);
    wait_OK();

    UARTb_SendString("AT+CIPMUX=1\r\n");//������ģ������
    wait_OK();

    memset(send_at,1000,0);

    sprintf(send_at,"AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n",mqtt_username,mqtt_password);
    UARTb_SendString(send_at);      //����mqttЭ�������Ʒ�����
    wait_OK();

    sprintf(send_at,"AT+MQTTCLIENTID=0,\"%s\"\r\n",product_id); //���ӷ�������Ŀid
    UARTb_SendString(send_at);
    wait_OK();

    sprintf(send_at,"AT+MQTTCONN=0,\"%s.iotcloud.tencentdevices.com\",1883,0\r\n",product_id);//������Ŀ�����Ʒ,0����������1��������
    UARTb_SendString(send_at);
    wait_OK();

    sprintf(send_at,"AT+MQTTSUB=0,\"$thing/up/property/%s/%s\",1\r\n",product_id,device_name);  //�������⣨Subscribe Topic��
    UARTb_SendString(send_at);
    wait_OK();

}

/**
 * �ȴ�����OK����
 */
void wait_OK(void)
{
    while(!Flag_Uartb_Receive_OK);          //�ȴ����յ�OK�ַ���
    Flag_Uartb_Receive_OK = 0;
    UARTb_Clear();                          //�������b���ջ���
}

/**
 * {
            "method":"report",
            "clientToken":"123",
            "timestamp":1628646783,
            "params":{
                "power_switch":1,
                "color":1,
                "brightness":32
            }
}
 */
int sendData(void)
{
//    char data_Json[1000];
//    sprintf(send_at,"AT+MQTTPUB=0,\"$thing/up/property/%s/%s\",\"data_Json\",1,0",product_id,device_name);
//    memset(send_at,1000,'\0');
//    memset(data_Json,1000,'\0');
//    copy_str(send_at,"AT+MQTTPUB=0,\"$thing/up/property/          /               \",\"",62);
//    copy_str(&send_at[33],product_id,10);
//    copy_str(&send_at[44],device_name,15);
//    copy_str(data_Json,
//            "{\"method\":\"report\","       //19
//            "\"clientToken\":\"123\","      //20
//            "\"params\":{"                  //10
//                "\"temp\":00,"              //10
//                "\"humi\":00,"              //10
//                "\"threshold_upper\":00,"   //21
//                "\"threshold_lower\":00,"   //21
//                "\"fan\":0,"                //8
//                "\"control_mode\":0,"       //17
//                "\"GPS_Info\":{"            //12
//                    "\"longtitude\":0000,"  //18
//                    "\"latitude\":0000"     //15
//                    "}"                     //1
//                "}"                         //1
//            "}",184);                       //1
//    copy_str(&send_at[62],data_Json,184);
//    copy_str(&send_at[246],"\",1,0\r\n",7);
//    UARTb_SendString(send_at);
//    UARTa_SendString("{\"method\":\"report\","       //19
//                        "\"clientToken\":\"123\","      //20
//                        "\"params\":{"                  //10
//                            "\"temp\":00,"              //10
//                            "\"humi\":00,"              //10
//                            "\"threshold_upper\":00,"   //21
//                            "\"threshold_lower\":00,"   //21
//                            "\"fan\":0,"                //8
//                            "\"control_mode\":0,"       //17
//                            "\"GPS_Info\":{"            //12
//                                "\"longtitude\":0000,"  //18
//                                "\"latitude\":0000"     //15
//                                "}"                     //1
//                            "}"                         //1
//                        "}");
    UARTb_SendString(send_data);
    wait_OK();
    return 1;
//    wait_OK();
}
void copy_str(char *des,char *src,unsigned char len)
{
    unsigned char i;
    for(i = 0; i < len; i++){
        *(des+i) = *(src+i);
    }
}
