# 智能控制
* 基于STM32实现的智能控制项目，可通过微信小程序和蓝牙助手连接控制,也可以通过实体按键控制
* 项目演示视频：[桌面系统演示视频](https://www.bilibili.com/video/BV1Lg411i7Uy/?vd_source=4b7747d106b1d19643b7f3f9c6e34d22)
## 1.硬件介绍
```
    STM32开发板f407芯片
    传感器模块:
            ESP8266WIFI模块
            蓝牙2.0模块
            超声波模块HC_05
            矩阵按键
            烟雾报警器MQ2
            MCP6050电子陀螺仪
            SG90舵机
            OLED屏0.96寸
            ......
```
## 2.技术介绍
* FREERTOS嵌入式实时操作系统
* ESP8266+MQTT
* STM32标准库
* C语言

## 3.思维导图
![智能家居项目](https://user-images.githubusercontent.com/49366044/203249550-3b10a984-501a-41dd-a371-f105660f5839.svg)

## 4.功能

 **监测**

* 通过超声波加舵机实现一定范围内监测任务，如果监测到前方有目标停留舵机则会停下并亮起提示灯，长时间则会警报

 **环境数据监测**

* 读取传感器数据，并显示到OLED和上传到微信小程序显示,如果超过设定的范围则会警报,并上传警报到微信小程序

 **蓝牙功能**
    
* 设置时间和闹钟，切换模式

 **微信小程序控制**

* 在单片机连接到wifi,可实现远程控制和上传数据功能，远程控制灯和警报,查看环境数据和显示警报

## 5.其他

## 特点
- 兼容性强
- 移植性强

## 作者
- [@CHEN](https://github.com/1002cc)
