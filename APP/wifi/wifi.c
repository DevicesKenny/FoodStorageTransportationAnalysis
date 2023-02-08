/*
 * wifi.c
 *
 *  Created on: Feb 1, 2023
 *      Author: Administrator
 */

#include "wifi.h"
#include "uart.h"

char send_at[1000];        //存储串口发送内容
extern char send_data[1000];


void Connect_wifi(void)
{
//    UARTb_SendString("AT+UART=9600,8,1,0,0\r\n");
//    wait_OK();

    UARTb_SendString("AT+RST\r\n");//AT测试指令
    DELAY_US(6000000);

    Flag_Uartb_Receive_OK = 0;
    UARTb_SendString("AT+CWMODE=3\r\n");//  设置工作模式
    wait_OK();

    sprintf(send_at,"AT+CWJAP=\"%s\",\"%s\"\r\n",wifi_name,wifi_password);  //连接wifi
    UARTb_SendString(send_at);
    wait_OK();

    UARTb_SendString("AT+CIPMUX=1\r\n");//启动多模块连接
    wait_OK();

    memset(send_at,1000,0);

    sprintf(send_at,"AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n",mqtt_username,mqtt_password);
    UARTb_SendString(send_at);      //利用mqtt协议连接云服务器
    wait_OK();

    sprintf(send_at,"AT+MQTTCLIENTID=0,\"%s\"\r\n",product_id); //连接服务器项目id
    UARTb_SendString(send_at);
    wait_OK();

    sprintf(send_at,"AT+MQTTCONN=0,\"%s.iotcloud.tencentdevices.com\",1883,0\r\n",product_id);//连接项目具体产品,0代表不重连，1代表重连
    UARTb_SendString(send_at);
    wait_OK();

    sprintf(send_at,"AT+MQTTSUB=0,\"$thing/up/property/%s/%s\",1\r\n",product_id,device_name);  //订阅主题（Subscribe Topic）
    UARTb_SendString(send_at);
    wait_OK();

}

/**
 * 等待接收OK反馈
 */
void wait_OK(void)
{
    while(!Flag_Uartb_Receive_OK);          //等待接收到OK字符串
    Flag_Uartb_Receive_OK = 0;
    UARTb_Clear();                          //清除串口b接收缓存
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
