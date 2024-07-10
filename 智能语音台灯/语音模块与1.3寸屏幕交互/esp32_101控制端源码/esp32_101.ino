
#include "main.h"
#define  VERSION   "V101"

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);                           //初始化串口
  Serial.println();                               //打印回车换行

  TFTDisplayInit();                               //初始化TFT
  connect_wifi();                                 //联网处理
  UDPTimeInit();                                  //初始化时间服务
  
  MainView();                                     //太空表主结构
  
//  getCityCode();                                  //通过IP地址获取城市代码

  CityWeaterShow();                           //获取天气信息
  weatherOldTime = millis();
}

void loop()
{
  SoftSerialRx();
  if(dhStatus != 0){
    runImg(dhStatus);                          
  }else{
    //时钟动画
    taikongren();
  }
                           
}

//太空人业务
void taikongren(){
  //主界面动画
  if (now() != oldTime){                    //如果本次数据和上次不一样的话，刷新
    oldTime = now();
    TimeUpdateShow();
  }
  if(millis() - weatherUpdateTime > 600000){              //10分钟更新一次天气
    weatherUpdateTime = millis();
    CityWeaterShow();
  }
  weatherShow();                                 //天气数据滚动显示
  jpgSlideShow();   
}
