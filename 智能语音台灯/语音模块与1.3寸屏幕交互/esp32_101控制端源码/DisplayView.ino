
//初始化TFT
void TFTDisplayInit(){
  tft.init();                                     //TFT初始化
  tft.setRotation(0);                             //旋转角度0-3
  tft.fillScreen(0x0000);                         //清屏tft.color565(255,6,31  
  tft.setTextColor(TFT_BLACK, bgColor);           //设置字体颜色

  //设置  TJpgDec
  TJpgDec.setJpgScale(1);                         //设置放大倍数
  TJpgDec.setSwapBytes(true);                     //交换字节
  TJpgDec.setCallback(tft_output);                //回调函数tft_output()
}

//显示回调函数
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  if (y >= tft.height()) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}

// 太空表主结构
void MainView(){
    TJpgDec.drawJpg(0,0,watchtop, sizeof(watchtop));//显示顶部图标 240*20
    TJpgDec.drawJpg(0,220,watchbottom, sizeof(watchbottom));//显示底部图标 240*20

  //绘制一个窗口
  tft.setViewport(0, 20, 240, 200);               //中间的显示区域大小
  tft.fillScreen(0x0000);                         //清屏
  tft.fillRoundRect(0,0,240,200,5,bgColor);       //实心圆角矩形
  //tft.resetViewport();

  //绘制线框
  //  drawFastHLine 画横线
  //  drawFastVLine 画竖线  
  tft.drawFastHLine(0,34,240,TFT_BLACK);          //这些坐标都是窗体内部坐标
  tft.drawFastVLine(150,0,34,TFT_BLACK);
  tft.drawFastHLine(0,166,240,TFT_BLACK);
  tft.drawFastVLine(60,166,34,TFT_BLACK);
  tft.drawFastVLine(160,166,34,TFT_BLACK);

  TJpgDec.drawJpg(161,171,temperature, sizeof(temperature));//温度图标
  TJpgDec.drawJpg(159,130,humidity, sizeof(humidity));  //湿度图标


}


void jpgSlideShow(){
  int x=80,y=94,dt=30;                            //dt图片播放间隔时长
  TJpgDec.drawJpg(x,y,i0, sizeof(i0));            
  delay(dt);
  TJpgDec.drawJpg(x,y,i1, sizeof(i1));
  delay(dt);
  TJpgDec.drawJpg(x,y,i2, sizeof(i2));
  delay(dt);
  TJpgDec.drawJpg(x,y,i3, sizeof(i3));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i4, sizeof(i4));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i5, sizeof(i5));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i6, sizeof(i6));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i7, sizeof(i7));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i8, sizeof(i8));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i9, sizeof(i9));
  delay(dt);  
}
