
// UDP时间服务初始化
void UDPTimeInit(){
  Serial.println("Starting UDP");                 //连接时间服务器
  Udp.begin(localPort);
  Serial.print("Local port: ");
//  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300); //时间校准间隔
}


time_t getNtpTime()                               //获取NTP时间
{
  IPAddress ntpServerIP;                          //NTP服务器的IP地址

  while (Udp.parsePacket() > 0) ;                 //之前的数据没有处理的话一直等待 discard any previously received packets
  WiFi.hostByName(ntpServerName, ntpServerIP);    //从网站名获取IP地址
  
  sendNTPpacket(ntpServerIP);                     //发送数据包
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();                 //接收数据
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);    //从缓冲区读取数据
      
      unsigned long secsSince1900;
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0;                                       //没获取到数据的话返回0
}

void sendNTPpacket(IPAddress &address)            //发送数据包到NTP服务器
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);       //缓冲区清零

  packetBuffer[0] = 0b11100011;                   //LI, Version, Mode   填充缓冲区数据
  packetBuffer[1] = 0;                            //Stratum, or type of clock
  packetBuffer[2] = 6;                            //Polling Interval
  packetBuffer[3] = 0xEC;                         //Peer Clock Precision
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  Udp.beginPacket(address, 123);                  //NTP服务器端口123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);       //发送udp数据
  Udp.endPacket();                                //发送结束
}


String week()                                     //星期
{
  String wk[7] = {"日","一","二","三","四","五","六"};
  String s = "周" + wk[weekday()-1];
  return s;
}

String monthDay()                                 //月日
{
  String s = String(month());
  s = s + "月" + day() + "日";
  return s;
}

String hourMinute()                               //时分
{
  String s = num2str(hour());
  s = s + ":" + num2str(minute());
  return s;
}

String num2str(int digits)                        //数字转换成字符串，保持2位显示
{
  String s = "";
  if (digits < 10)
    s = s + "0";
  s = s + digits;
  return s;
}

void printDigits(int digits)                      //打印时间数据
{
  Serial.print(":");
  if (digits < 10)                                //打印两位数字
    Serial.print('0');
  Serial.print(digits);
}
