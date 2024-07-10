#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define BLINKER_WIFI
#include <Blinker.h>

SoftwareSerial mySoftwareSerial(4, 5); // RX, TX //实例化软串口设置通讯用的RX和TX引脚 这是8266的引脚
DFRobotDFPlayerMini myDFPlayer; //实例化库
void printDetail(uint8_t type, int value);//定义一个方法用来打印错误信息
char auth[] = "eac744d3f392";
char ssid[] = "z";
char pswd[] = "00000000";
uint8_t is_run = 0;//是否按顺序播放停止或继续播放用 
uint8_t playInSequence = 0;//播放循序 记录现在是向上播放还是向下播放 默认下一首
uint8_t i=5;
// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerButton Button2("btn-j8m");
BlinkerButton Button3("btn-23f");
BlinkerButton Button4("btn-h5h");
BlinkerButton Button5("btn-a3o");
BlinkerButton Button6("btn-0k8");
BlinkerButton Button7("btn-o4j");
// 按下button1按键即会执行该函数
void button1_callback(const String & state) {
    BLINKER_LOG("get button1 state: ", state);
    digitalWrite(2, !digitalRead(LED_BUILTIN));
    myDFPlayer.previous();//上一首
    playInSequence = 1;//并将顺序改为向上播放
}
// 按下button2按键即会执行该函数
void button2_callback(const String & state) {
    BLINKER_LOG("get button2 state: ", state);
    digitalWrite(2, !digitalRead(LED_BUILTIN));
    myDFPlayer.next();//下一首
    playInSequence = 0;//并将顺序改为向下播放
}
// 按下button3按键即会执行该函数
void button3_callback(const String & state) {
    is_run = !is_run;
}
void button4_callback(const String & state) {
     myDFPlayer.volume(0);
}
void button5_callback(const String & state) {
     myDFPlayer.volume(5);
}
void button6_callback(const String & state) {
     myDFPlayer.volume(20);
}
void button7_callback(const String & state) {
     myDFPlayer.volume(30);
}
void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  delay(3000);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));//库demo
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));// 正在初始化DFPlayer 可能需要3-5秒钟

  if (!myDFPlayer.begin(mySoftwareSerial)) {     //使用softwareSerial与mp3通信。
    Serial.println(F("Unable to begin:")); //无法开始
    Serial.println(F("1.Please recheck the connection!"));//请重新检查连接 也就是我们接的线路
    Serial.println(F("2.Please insert the SD card!")); //请插入SD卡！
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));//走到这里就说明设备在线可以继续操作 

  myDFPlayer.play(1);  //播放第一个MP3 第一首歌曲
  //调试输出
  BLINKER_DEBUG.stream(Serial);
  
  // 初始化有LED的IO
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  Button1.attach(button1_callback);//绑定button1的回调函数
  Button2.attach(button2_callback);//绑定button2的回调函数
  Button3.attach(button3_callback);//绑定button2的回调函数
  Button4.attach(button4_callback);//绑定button4的回调函数
  Button5.attach(button5_callback);//绑定button5的回调函数
  Button6.attach(button6_callback);//绑定button6的回调函数
  Button7.attach(button7_callback);//绑定button7的回调函数
}

void loop()
{
  static unsigned long timer = millis();//保持到下一次调用，直到下次赋新值
  if ((millis() - timer > 500000) && is_run) {
    timer = millis();
    //判断是向上播放还是向下播放
    if(playInSequence == 0){
      myDFPlayer.next();
    }else{
      myDFPlayer.previous();//上一首
    }
      //每5秒切换下一首语音 
  }
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //打印DFPlayer的详细信息，以处理不同的错误和状态。
  }
  Blinker.run();
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!")); //时间到！
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!")); //堆栈错误！
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!")); //卡已插入！
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!")); //卡片已删除！
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!")); //卡在线！
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!")); //播放完毕！
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack")); //获取错误堆栈
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match")); //校验和不匹配
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound")); //文件索引超出范围
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File")); //无法找到文件
          break;
        case Advertise:
          Serial.println(F("In Advertise")); //在广告中
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
