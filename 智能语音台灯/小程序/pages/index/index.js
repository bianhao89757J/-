// index.js
// 获取应用实例
const app = getApp()

const { connect } = require('../../utils/mqtt')

const mqttHost = 'mqtt.lsmjh1314.xyz'//mqtt域名
const mqttPort = 8084//mqtt服务器端口

const deviceSubTopic = 'testtopicSub'//设备订阅Topic
const devicePubTopic = 'testtopicPub'//设备发布topic

const mpSubTopic = devicePubTopic
const mpPubTopic = deviceSubTopic

Page({
  data: { 
    client:null,
    Temp:0,
    Hum:0,
    Light:0,
    Led:false,
    Beep:false
  },

  onLedChance(event){
    const that = this
    console.log(event.detail.value);

    const sw = event.detail.value
    that.setData({Led:sw})

    if(sw){
      that.data.client.publish(mpPubTopic,JSON.stringify({
        target:"Led",
        value:1
      }),function (err) {
        if(!err){
          console.log('成功下发指令——开灯');
        }
      })
    }else{
      that.data.client.publish(mpPubTopic,JSON.stringify({
        target:"Led",
        value:0
      }),function (err) {
        if(!err){
          console.log('成功下发指令——关灯');
        }
      })
    }
  },
  onBeepChance(event){
    const that = this
    console.log(event.detail.value);

    const sw = event.detail.value
    that.setData({Beep:sw})

    if(sw){
      that.data.client.publish(mpPubTopic,JSON.stringify({
        target:"Beep",
        value:1
      }),function (err) {
        if(!err){
          console.log('成功下发指令——打开报警器');
        }
      })
    }else{
      that.data.client.publish(mpPubTopic,JSON.stringify({
        target:"Beep",
        value:0
      }),function (err) {
        if(!err){
          console.log('成功下发指令——关闭报警器');
        }
      })
    }
  },
  
  // 事件处理函数
  onShow(){
    const that = this
    that.setData({
      client: connect('wxs://mqtt.lsmjh1314.xyz:8084/mqtt')
     })

     that.data.client.on('connect',function () {
       console.log('成功连接!')
       wx.showToast({
         title: '连接成功',
         icon:'success',
         mask:true
       })
        that.data.client.subscribe(mpSubTopic,function (err) {
          if(!err){
            console.log('成功订阅设备上行数据!')
          }
        })
     })
     that.data.client.on('message',function (topic,message) {
       console.log(topic);
       //console.log(message);

       let dataFromDev={}
       try {
          dataFromDev =JSON.parse(message)
          console.log(dataFromDev);
        that.setData({
          Temp:dataFromDev.Temp,
          Hum:dataFromDev.Hum,
          Light:dataFromDev.Light,
          Led:dataFromDev.Led,
          Beep:dataFromDev.Beep,
        })

       } catch (error) {
        //console.log(error);
        console.log('JSON解析失败',error);
       }
      
     })
  },
})
