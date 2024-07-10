#ifndef __MAIN_H__
#define __MAIN_H__
/*
 * 类库调用
 * 函数声明
 * 变量生命
 * 参数设置
 */

//---------------------库文件---------------------
//#include <ESP8266WiFi.h>
#include <WiFi.h>
//#include <ESP8266HTTPClient.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <TFT_eSPI.h> 
#include <SPI.h>
#include <TJpg_Decoder.h>
#include "Arduino.h"
#include "SoftwareSerial.h"

//---------------------图片相关参数---------------------
#include "font/ZdyLwFont_20.h"          //字体头文件
#include "font/FxLED_32.h"
#include "img/pangzi/i0.h"              //太空人图片
#include "img/pangzi/i1.h"
#include "img/pangzi/i2.h"
#include "img/pangzi/i3.h"
#include "img/pangzi/i4.h"
#include "img/pangzi/i5.h"
#include "img/pangzi/i6.h"
#include "img/pangzi/i7.h"
#include "img/pangzi/i8.h"
#include "img/pangzi/i9.h"
#include "img/temperature.h"            //温度图标
#include "img/humidity.h"               //湿度图标
#include "img/watch_top.h"              //顶部图标
#include "img/watch_bottom.h"           //底部图标
#include "img/face.h"                   //脸部

//---------------------实例化TFT---------------------
TFT_eSPI tft = TFT_eSPI();              //引脚请自行配置tft_espi库中的 User_Setup.h文件
TFT_eSprite clk = TFT_eSprite(&tft);
TFT_eSprite clkb = TFT_eSprite(&tft);   

//---------------------路由器账户号信息---------------------
const char* WIFI_SSID     = "z"; //家里无线路由器的账号和密码，----------要修改成自己的----------，引号不要去掉
const char* WIFI_PASSWORD = "00000000";

//---------------------声明网络---------------------
WiFiClient Client;
HTTPClient httpClient;                  //创建 HTTPClient 对象

//---------------------设置软串口---------------------
SoftwareSerial mySoftwareSerial(16, 17); // RX, TX


//---------------------动画相关参数---------------------
uint8_t loadNum = 6;                    //开机启动进度条的起始位置
uint16_t bgColor = 0xFFFF;              //背景颜色 565颜色
//String cityCode = "101040100";          //天气城市代码
String cityCode = "101050901";          //天气城市代码

uint8_t Dis_Count = 0;                  //滚动显示内容计数
String scrollText[6];                   //滚动显示的数据缓冲区

//---------------------NTP相关参数---------------------
static const char ntpServerName[] = "ntp1.aliyun.com"; //NTP服务器   
const int timeZone = 8;                 //时区，东八区为北京时间
WiFiUDP Udp;
unsigned int localPort = 8888;          //连接时间服务器的本地端口号
time_t oldTime = 0;                 //上一次获取到的时间
const int NTP_PACKET_SIZE = 48;         //NTP发送数据包长度
byte packetBuffer[NTP_PACKET_SIZE];     //NTP数据包缓冲区

//---------------------Time 相关参数---------------------
int Led_Flag = HIGH;                    //默认当前灭灯
bool Led_State = false;                 //灯状态
unsigned long weatherOldTime;           //天气文字切换时间
unsigned long weatherUpdateTime = 0;
time_t getNtpTime();
void digitalClockDisplay();   //时间显示
void printDigits(int digits); //时间格式化
String num2str(int digits);   //数字转字符
void sendNTPpacket(IPAddress &address);

//---------------------接收数据 相关参数---------------------
StaticJsonDocument<64> cmd;
int dhStatus = 0;

#endif
