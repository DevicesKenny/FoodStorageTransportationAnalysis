/*
 * main.c
 *
 *  Created on: Jan 7, 2023
 *      Author: Administrator
 *      EXTI实现按键中断，于LCD9648上显示
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

#define dataNum_page                2       //数据收集需要显示的页面
#define settingsNum_page            2       //设置界面需要显示的页面

#define temp_humi_interface         1       //温湿度收集界面
#define GPS_interface               2       //GPS收集界面

#define tempSettings_thre_upper     1       //温度设置上限界面
#define tempSettings_thre_lower     2       //温度设置下限界面

#define Stop_Rotate                 0       //风扇停止
#define Left_Rotate                 1       //风扇左转
#define Right_Rotate                2       //风扇右转

#define Temperature_Upper           40
#define Temperature_Lower           0

extern char SCIA_RxBUF[UARTa_LEN];  //串口a数据接收数组
extern char SCIB_RxBUF[UARTb_LEN];  //串口b数据接收数组
extern char key;                    //按键扫描按键值

float temperature=0;                //检测当前的温度值
float humidity=0;                   //检测当前的湿度值
char temp[6] = {0,0,0,0,0,0};       //温度字符型进行转换
char humi[6] = {0,0,0,0,0,0};       //湿度字符型进行转换

int threshold_upper = 30;           //温度上限阈值
int threshold_lower = 25;           //温度下限阈值
int threshold_upper_temp = 30;       //温度上限预设值
int threshold_lower_temp = 25;       //温度下限预设值
char thre_upper[3] = {0,0,0};       //温度上限数据转化存储
char thre_lower[3] = {0,0,0};       //温度下限数据转化存储

int key_lcd = 0;                    //按键屏幕切换（默认为收集数据显示界面）   0：数据显示收集界面      1：温度阈值设置界面
int data_page = 1;                  //当前数据显示页号
int settings_page = 1;              //当前设置界面显示页号

int fan_rotate = 0;                //风扇转的方向

void key_operation(void);
void show_data(int page);
void show_settings(int page);
void ThresholdAdd(int page);
void ThresholdSub(int page);
void autoAdjust(void);
void fan_operation(int index);

char send_data[1000];

/*******************************************************************************
* 函 数 名         : delay
* 函数功能         : 延时函数，通过循环占用CPU，达到延时功能
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void delay(void)
{
    Uint16      i;
    Uint32      j;
    for(i=0;i<32;i++)
        for (j = 0; j < 100000; j++);
}


/*******************************************************************************
* 函 数 名         : main
* 函数功能         : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main(){
    char *data_Json;

//    char test[1000];

    InitSysCtrl();//系统时钟初始化，默认已开启F28335所有外设时钟

    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    //复制对时间敏感代码和FLASH配置代码到RAM中
    // 包括FLASH初始化函数 InitFlash();
    // 链接后将产生 RamfuncsLoadStart, RamfuncsLoadEnd, 和RamfuncsRunStart
    // 参数. 请参考 F28335.cmd 文件
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    // 调用FLASH初始化函数来设置flash等待状态
    // 这个函数必须在RAM中运行
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
    flag_2s=1;                      //直接开启第一轮数据收集
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
        //每隔2s对数据进行一次收集与上传
        if(flag_2s){
            flag_2s = 0;
            LCD9648_Clear();
            //设置周期模式，初始化
            Measure_Peroid_Mode();
            //IIC读数据
            READ_IIC_B_DATA(&temperature,&humidity);

            temp[0] = (int)temperature/10+'0';
            temp[1] = (int)temperature%10+'0';
            UARTa_SendString("temperature（温度）: ");
            UARTa_SendString(temp);
            UARTa_SendString("℃   \r\n");

            humi[0] = (int)humidity/10+'0';
            humi[1] = (int)humidity%10+'0';
            UARTa_SendString("humidity（湿度）: ");
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
        if(!key_lcd){           //key_lcd==0,显示数据收集界面
            show_data(data_page);
        }
        if(key_lcd){                  //key_lcd==1,进入设置界面
            show_settings(settings_page);
        }
        key_operation();        //进行矩阵按键相关操作

        //autoAdjust();

    }
}

/**
 * 按键中断相关操作
 */
void key_operation(void)
{
    //按键对屏幕进行切换
    if(key == KEY1_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        key_lcd = !key_lcd;
    }

    //按键屏幕上移
    if(key == KEY2_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        if(!key_lcd){                       //当前lcd界面显示数据页
            if(data_page > 1){              //页面下限至少为一页
                --data_page;
            }
            show_data(data_page);
        }else{                              //当前lcd界面显示阈值设置页
            if(settings_page > 1){          //页面下限至少为1页
                --settings_page;
            }
            show_settings(settings_page);
        }
    }

    if(key == KEY3_PRESS){
        key = KEY_UNPRESS;
    }

    //按键对温度阈值进行上调
    if(key == KEY4_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        if(threshold_lower<=threshold_upper){   //上下限判断条件
            ThresholdAdd(settings_page);
        }
        show_settings(settings_page);

    }

    if(key == KEY5_PRESS){
        key = KEY_UNPRESS;
    }

    //按键对温度阈值进行下调
    if(key == KEY6_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        if(threshold_lower<=threshold_upper){
            ThresholdSub(settings_page);
        }
        show_settings(settings_page);
    }

    //testing------蜂鸣器响
    if(key == KEY7_PRESS){
        key = KEY_UNPRESS;
        BEEP_Play();
    }

    //按键屏幕下移
    if(key == KEY8_PRESS){
        key = KEY_UNPRESS;
        LCD9648_Clear();
        if(!key_lcd){                       //当前lcd界面显示数据页
            if(data_page < dataNum_page){
                ++data_page;
                show_data(data_page);
            }else{
                data_page = dataNum_page;
            }
        }else{                              //当前lcd界面显示阈值设置页
            if(settings_page < settingsNum_page){
                ++settings_page;
                show_settings(settings_page);
            }else{
                settings_page = settingsNum_page;
            }
        }
    }

    //testing---风扇转
    if(key == KEY9_PRESS){
        key = KEY_UNPRESS;
    }
}

/**
 * lcd9648显示数据界面
 */
void show_data(int page)            //page <= dataNum_page
{
    if(page == temp_humi_interface){
       LCD9648_Write16CnCHAR(10,0,"温度：");
       LCD9648_Write8CnCHAR(60,0,(unsigned char*)temp);
       LCD9648_Write16CnCHAR(80,0,"℃");
       LCD9648_Write16CnCHAR(10,3,"湿度：");
       LCD9648_Write8CnCHAR(60,3,(unsigned char*)humi);
       LCD9648_Write8CnCHAR(80,3,"%");
    }
    if(page == GPS_interface){
       LCD9648_Write8CnCHAR(10,0,"GPS:");
    }
}

/**
 * lcd9648显示阈值设置界面
 */
void show_settings(int page)        //page <= settingsNum_page
{
    if(page == tempSettings_thre_upper){
        LCD9648_Write16CnCHAR(20,0,"阈值设置");
        LCD9648_Write16CnCHAR(10,3,"上限：");

        thre_upper[0] = (int)threshold_upper/10+'0';
        thre_upper[1] = (int)threshold_upper%10+'0';

        LCD9648_Write8CnCHAR(60,3,(unsigned char*)thre_upper);
        LCD9648_Write16CnCHAR(80,3,"℃");
    }
    if(page == tempSettings_thre_lower){
        LCD9648_Write16CnCHAR(20,0,"阈值设置");
        LCD9648_Write16CnCHAR(10,3,"下限：");

        thre_lower[0] = (int)threshold_lower/10+'0';
        thre_lower[1] = (int)threshold_lower%10+'0';

        LCD9648_Write8CnCHAR(60,3,(unsigned char*)thre_lower);
        LCD9648_Write16CnCHAR(80,3,"℃");
    }
}

/**
 * 设置阈值递增函数
 */
void ThresholdAdd(int page)
{
    if(page == tempSettings_thre_upper){      //温度上限设置阈值页面
        threshold_upper++;
    }
    if(page == tempSettings_thre_lower){      //温度下限设置阈值页面
        threshold_lower++;
    }
    if(threshold_upper < threshold_lower){      //数据比较不规范调整：上下限相等时，下限++到下限>上限，进行下限回调
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
 * 设置阈值递减函数
 */
void ThresholdSub(int page)
{
    if(page == tempSettings_thre_upper){
        threshold_upper--;
    }
    if(page == tempSettings_thre_lower){
        threshold_lower--;
    }
    if(threshold_upper < threshold_lower){      //数据比较不规范调整：上下限相等时，上限--到上限<下限，进行上限回调
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
 * 自动调节测试
 *  ① 上下限阈值中间---正常，无响应（或led亮）               //led7亮，表正常
 *  ② 超过阈值下限，进行升温（蜂鸣器响2s，代表不正常，随后灯亮）    //led6亮，不正常
 *  ③ 超过阈值上限，进行降温（蜂鸣器响2s，代表不正常，随后风扇转）   //电机驱动风扇
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
 * 风扇代码相关操作
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
