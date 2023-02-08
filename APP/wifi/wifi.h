/*
 * wifi.h
 *
 *  Created on: Feb 1, 2023
 *      Author: Administrator
 */

#ifndef APP_WIFI_WIFI_H_
#define APP_WIFI_WIFI_H_


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include <stdio.h>
#include <string.h>

#define wifi_name                   "happiness"
#define wifi_password               "xingfu618"

#define mqtt_username               "0BRPZNL1L1smart_transport;12010126;HF895;1675779644"
#define mqtt_password               "c3388d3ed01551a4197885fc54d4d4e690ea3a788ddd2bd1341955e7840eb946;hmacsha256"
#define product_id                  "0BRPZNL1L1"                //10
#define device_name                 "smart_transport"           //15
#define device_secret               "djNYcsPjfzg6OL0TJIsacQ=="

void wait_OK(void);
void Connect_wifi(void);
int sendData(void);
void copy_str(char *des,char *src,unsigned char len);

#endif /* APP_WIFI_WIFI_H_ */
