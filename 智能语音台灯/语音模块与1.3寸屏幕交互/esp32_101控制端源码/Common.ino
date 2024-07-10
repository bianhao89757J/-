
void connect_wifi()                               //联网
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);           //用固定的账号密码连接网络

  while (WiFi.status() != WL_CONNECTED) {         //未连接上的话
    for (uint8_t n = 0; n < 10; n++){             //每500毫秒检测一次状态
      PowerOn_Loading(50);
    }
  }
  while(loadNum < 194){                           //让动画走完
    PowerOn_Loading(1);
  }
  
  Serial.print("\nWiFi connected to: ");
  Serial.println(WIFI_SSID);
  Serial.print("IP:   ");
  Serial.println(WiFi.localIP());                 //得到IP地址
}

void PowerOn_Loading(uint8_t delayTime)           //开机联网显示的进度条，输入延时时间
{
  clk.setColorDepth(8);
  clk.createSprite(200, 50);                      //创建Sprite
  clk.fillSprite(0x0000);                         //填充颜色

  clk.drawRoundRect(0,0,200,16,8,0xFFFF);         //画一个圆角矩形
  clk.fillRoundRect(3,3,loadNum,10,5,0xFFFF);     //画一个填充的圆角矩形
  clk.setTextDatum(CC_DATUM);                     //显示对齐方式
  clk.setTextColor(TFT_GREEN, 0x0000);            //文本的前景色和背景色
  clk.drawString("Connecting to WiFi",100,40,2);  //显示文本
  clk.pushSprite(20,110);                         //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite
  loadNum += 1;                                   //进度条位置变化，直到加载完成
  if(loadNum>=194){
    loadNum = 194;
  }
  delay(delayTime);
}

void TimeUpdateShow()                        //时间显示
{
  clk.setColorDepth(8); 

  //--------------------中间时间区显示开始--------------------
  //时分
  clk.createSprite(140, 48);                      //创建Sprite，先在Sprite内存中画点，然后将内存中的点一次推向屏幕，这样刷新比较快
  clk.fillSprite(bgColor);                        //背景色
  //clk.loadFont(FxLED_48);
  clk.setTextDatum(CC_DATUM);                     //显示对齐方式
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色
  clk.drawString(hourMinute(),70,24,7);           //绘制时和分
  //clk.unloadFont();
  clk.pushSprite(28,40);                          //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite
  
  //秒
  clk.createSprite(40, 32);
  clk.fillSprite(bgColor);
  clk.loadFont(FxLED_32);                         //加载字体
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor); 
  clk.drawString(num2str(second()),20,16);
  clk.unloadFont();                               //卸载字体
  clk.pushSprite(170,60);
  clk.deleteSprite();
  //--------------------中间时间区显示结束--------------------

  //--------------------底部时间区显示开始--------------------
  clk.loadFont(ZdyLwFont_20);                     //加载汉字字体
  
  //星期
  clk.createSprite(58, 32);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);
  clk.drawString(week(),29,16);                   //周几
  clk.pushSprite(1,168);
  clk.deleteSprite();
  
  //月日
  clk.createSprite(98, 32);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);  
  clk.drawString(monthDay(),49,16);               //几月几日
  clk.pushSprite(61,168);
  clk.deleteSprite();

  clk.unloadFont();                               //卸载字体
  //--------------------底部时间区显示结束--------------------
}





void getCityCode()                                //发送HTTP请求并且将服务器响应通过串口输出
{
  String URL = "http://wgeo.weather.com.cn/ip/?_="+String(now());

  httpClient.begin(Client,URL);                          //配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.setUserAgent("esp8266");             //用户代理版本，其实没什么用 最重要是后端服务器支持
  //httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");//设置请求头中的User-Agent
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");
 
  int httpCode = httpClient.GET();                //启动连接并发送HTTP请求
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);
  
  if (httpCode == HTTP_CODE_OK) {                 //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    String str = httpClient.getString();
    int aa = str.indexOf("id=");
    if (aa > -1){                                 //应答包里找到ID了
       cityCode = str.substring(aa+4,aa+4+9);     //9位长度
       Serial.println(cityCode); 
       CityWeaterShow();                           //显示天气信息
       weatherOldTime = millis();
    }
    else{                                         //没有找到ID
      Serial.println("获取城市代码失败");  
    }
  } 
  else{
    Serial.println("请求城市代码错误：");
    Serial.println(httpCode);
  }
 
  httpClient.end();                               //关闭与服务器连接
}

void CityWeaterShow()                              //获取城市天气
{
  String URL = "http://d1.weather.com.cn/weather_index/" + cityCode + ".html?_="+String(now());

  httpClient.begin(Client,URL);                          //配置请求地址。
  httpClient.setUserAgent("esp8266");             //用户代理版本，其实没什么用 最重要是后端服务器支持
  //httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");//设置请求头中的User-Agent
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");
 
  int httpCode = httpClient.GET();                //启动连接并发送HTTP请求
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);
  
  if (httpCode == HTTP_CODE_OK) {                 //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    String str = httpClient.getString();
    int indexStart = str.indexOf("weatherinfo\":");//寻找起始和结束位置
    int indexEnd = str.indexOf("};var alarmDZ");

    String jsonCityDZ = str.substring(indexStart+13,indexEnd);//复制字符串
    Serial.println(jsonCityDZ);

    indexStart = str.indexOf("dataSK =");         //寻找起始和结束位置
    indexEnd = str.indexOf(";var dataZS");
    String jsonDataSK = str.substring(indexStart+8,indexEnd);//复制字符串
    Serial.println(jsonDataSK);

    indexStart = str.indexOf("\"f\":[");          //寻找起始和结束位置
    indexEnd = str.indexOf(",{\"fa");
    String jsonFC = str.substring(indexStart+5,indexEnd);//复制字符串
    Serial.println(jsonFC);
    
    weaterData(&jsonCityDZ,&jsonDataSK,&jsonFC);  //显示天气信息
    Serial.println("获取成功");
    
  } 
  else {
    Serial.println("请求城市天气错误：");
    Serial.print(httpCode);
  }
 
  httpClient.end();                               //关闭与服务器连接
}

void weaterData(String *cityDZ,String *dataSK,String *dataFC)//天气信息写到屏幕上
{
  DynamicJsonDocument doc(512);
  deserializeJson(doc, *dataSK);
  JsonObject sk = doc.as<JsonObject>();

  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);                     //加载汉字字体
  
  //温度显示
  clk.createSprite(54, 32);                       //创建Sprite
  clk.fillSprite(bgColor);                        //填充颜色
  clk.setTextDatum(CC_DATUM);                     //显示对齐方式
  clk.setTextColor(TFT_BLACK, bgColor);           //文本的前景色和背景色 
  clk.drawString(sk["temp"].as<String>()+"℃",27,16);//显示文本
  clk.pushSprite(185,168);                        //Sprite中内容一次推向屏幕
  clk.deleteSprite();                             //删除Sprite

  //城市名称显示
  clk.createSprite(88, 32); 
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor); 
  clk.drawString(sk["cityname"].as<String>(),44,16);
  clk.pushSprite(151,1);
  clk.deleteSprite();
  
  //PM2.5空气指数显示
  uint16_t pm25BgColor = tft.color565(156,202,127);//优
  String aqiTxt = "优";
  int pm25V = sk["aqi"];
  if (pm25V > 200){
    pm25BgColor = tft.color565(136,11,32);        //重度，显示颜色和空气质量程度
    aqiTxt = "重度";
  }
  else if (pm25V > 150){
    pm25BgColor = tft.color565(186,55,121);       //中度
    aqiTxt = "中度";
  }
  else if (pm25V > 100){
    pm25BgColor = tft.color565(242,159,57);       //轻
    aqiTxt = "轻度";
  }
  else if (pm25V > 50){
    pm25BgColor = tft.color565(247,219,100);      //良
    aqiTxt = "良";
  }
  clk.createSprite(50, 24); 
  clk.fillSprite(bgColor);
  clk.fillRoundRect(0,0,50,24,4,pm25BgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(0xFFFF); 
  clk.drawString(aqiTxt,25,13);
  clk.pushSprite(5,130);
  clk.deleteSprite();

  //湿度显示
  clk.createSprite(56, 24); 
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor); 
  clk.drawString(sk["SD"].as<String>(),28,13);
  //clk.drawString("100%",28,13);
  clk.pushSprite(180,130);
  clk.deleteSprite();

  scrollText[0] = "实时天气 " + sk["weather"].as<String>();//滚动显示的数据缓冲区
  scrollText[1] = "空气质量 " + aqiTxt;
  scrollText[2] = "风向 " + sk["WD"].as<String>()+sk["WS"].as<String>();
  
  //左上角滚动字幕
  deserializeJson(doc, *cityDZ);
  JsonObject dz = doc.as<JsonObject>();
  //Serial.println(sk["ws"].as<String>());
  //String aa = "今日天气:" + dz["weather"].as<String>() + "，温度:最低" + dz["tempn"].as<String>() + "，最高" + dz["temp"].as<String>() + " 空气质量:" + aqiTxt + "，风向:" + dz["wd"].as<String>() + dz["ws"].as<String>();
  //Serial.println(aa);
  scrollText[3] = "今日" + dz["weather"].as<String>();
  
  deserializeJson(doc, *dataFC);
  JsonObject fc = doc.as<JsonObject>();
  scrollText[4] = "最低温度"+fc["fd"].as<String>()+"℃";
  scrollText[5] = "最高温度"+fc["fc"].as<String>()+"℃";
  
  clk.unloadFont();                               //卸载字体
}

void weatherShow()                               //天气滚动条显示
{
  
  if(millis() - weatherOldTime > 2500){                    //2.5秒切换一次显示内容
    if(scrollText[Dis_Count]){                    //如果滚动显示缓冲区有数据
      clkb.setColorDepth(8);
      clkb.loadFont(ZdyLwFont_20);                //加载汉字字体
      
//      for(int pos = 24; pos>0; pos--){            //24点，每次移动一个点，从下往上移
//        Dis_Scroll(pos);
//      }

      //滚动显示修改为静态显示，滚动的时候太空人会卡顿
      clkb.createSprite(148, 24);                 //创建Sprite，先在Sprite内存中画点，然后将内存中的点一次推向屏幕，这样刷新比较快
      clkb.fillSprite(bgColor);                   //背景色
      clkb.setTextWrap(false);
      clkb.setTextDatum(CC_DATUM);                //显示对齐方式
      clkb.setTextColor(TFT_BLACK, bgColor);      //文本的前景色和背景色
      clkb.drawString(scrollText[Dis_Count],74,12);//打显示内容
      clkb.pushSprite(2,4);                       //Sprite中内容一次推向屏幕
      clkb.deleteSprite();                        //删除Sprite
  
      
      //clkb.deleteSprite();                      //删除Sprite，这个我移动到Dis_Scroll函数里了
      clkb.unloadFont();                          //卸载字体
  
      if (Dis_Count >= 5){                        //总共显示五条信息
        Dis_Count = 0;                            //回第一个
      }
      else{
        Dis_Count += 1;                           //准备切换到下一个  
      }
      //Serial.println(Dis_Count);
    }
    weatherOldTime = millis();
  }
}

void Dis_Scroll(int pos){                         //字体滚动
  clkb.createSprite(148, 24);                     //创建Sprite，先在Sprite内存中画点，然后将内存中的点一次推向屏幕，这样刷新比较快
  clkb.fillSprite(bgColor);                       //背景色
  clkb.setTextWrap(false);
  clkb.setTextDatum(CC_DATUM);                    //显示对齐方式
  clkb.setTextColor(TFT_BLACK, bgColor);          //文本的前景色和背景色
  clkb.drawString(scrollText[Dis_Count],74,pos+12);//打显示内容
  clkb.pushSprite(2,4);                           //Sprite中内容一次推向屏幕
  clkb.deleteSprite();                            //删除Sprite
}

void SoftSerialRx(){
  String revData;
  
  if(mySoftwareSerial.available()){
    revData = mySoftwareSerial.readStringUntil('\n');
    String val = "kaixin";
    revData.trim();
    Serial.println(revData);
    Serial.println(revData.length());
    Serial.println(val.length());
    
//    if( val=="kaixin"){
//      dhStatus =1;
//    }
    if( revData=="kaixin"){
      dhStatus =1;
    }
    if(revData== "chongbai"){
      dhStatus =2;
    }
    if(revData== "shuaku"){
      dhStatus =3;
    }
    if(revData== "tiaopi"){
      dhStatus = 4;
    }

    if(revData== "wyc"){
      dhStatus = 5;
    }

    if(revData== "kongtiao"){
      dhStatus = 6;
    }
  }
}

//开心表情
void kaixindh(){
  TJpgDec.drawJpg(0,0,kaixin, sizeof(kaixin));//
  delay(450);
  TJpgDec.drawJpg(0,0,kaixin2, sizeof(kaixin2));//
  delay(900);

}

//耍酷 表情
void shuakudh(){
  TJpgDec.drawJpg(0,0,shuaku, sizeof(shuaku));//
  delay(450);
  TJpgDec.drawJpg(0,0,kaixin2, sizeof(kaixin2));//
  delay(900);
}

//调皮 表情
void tiaopidh(){
  TJpgDec.drawJpg(0,0,tiaopi, sizeof(tiaopi));//
  delay(450);
  TJpgDec.drawJpg(0,0,kaixin2, sizeof(kaixin2));//
  delay(900);
}

//自定义 表情
void zidingyidh(){
  TJpgDec.drawJpg(0,0,tiaopi, sizeof(tiaopi));//
  delay(450);
  TJpgDec.drawJpg(0,0,kaixin2, sizeof(kaixin2));//
  delay(900);
}



//崇拜 表情
void chongbaidh(){
  TJpgDec.drawJpg(0,0,chongbai, sizeof(chongbai));//
  delay(450);
  TJpgDec.drawJpg(0,0,kaixin2, sizeof(kaixin2));//
  delay(900);

}
void wyc(){
  TJpgDec.drawJpg(30,0,wyc1, sizeof(wyc1));//
  delay(4050);
  TJpgDec.drawJpg(0,0,wyc2, sizeof(wyc2));
  delay(4050);
}

void runImg(int index){
  tft.resetViewport();
  if(index == 1){
    tft.fillScreen(0xFFFF);
    kaixindh();
  }

  if(index == 2){
    tft.fillScreen(0xFFFF);
    chongbaidh();
  }

  if(index == 3){
    tft.fillScreen(0xFFFF);
    shuakudh();
  }

  if(index == 4){
    tft.fillScreen(0xFFFF);
    tiaopidh();
  }

  if(index == 5){
    tft.fillScreen(0xFFFF);
    wyc();
  }

  if(index == 6){
    tft.fillScreen(0xFFFF);
    tiaopidh();
  }
  tft.resetViewport();
  CityWeaterShow();
  dhStatus = 0;
  MainView();
}
