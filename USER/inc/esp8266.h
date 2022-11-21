#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "usart.h"

#define EN_DEBUG_ESP8266	0

//热点或wifi账号密码
#define MY_WIFI_NAME HONORX10
#define MY_WIFI_PASS 10025050
//clientid
#define MY_MQTT_CLINETID CHEN
//MQTT服务器
#define MY_MQTTCONN t.yoyolife.fun
//MQTT用户名密码
#define MY_MQTTUSER 3071f580444edae15becd1c66714c780
#define MY_MQTTPASS 123456
//订阅主题
#define MY_MQTTSUB /iot/1879/chen/sub/
//发布主题
#define MY_MQTTPUB /iot/1879/chen/updata/


//添加WIFI热点宏定义，此处根据自己的wifi作调整
#define WIFI_SSID 			"HONORX10"
#define WIFI_PASSWORD		"10025050"

extern uint8_t  g_esp8266_tx_buf[512];
extern volatile uint8_t  g_esp8266_rx_buf[512];
extern volatile uint32_t g_esp8266_rx_cnt;
extern volatile uint32_t g_esp8266_rx_end;

extern volatile uint32_t g_esp8266_transparent_transmission_sta;

extern void 	esp8266_init(void);
extern int32_t  esp8266_self_test(void);
extern int32_t 	esp8266_exit_transparent_transmission (void);
extern int32_t 	esp8266_entry_transparent_transmission(void);
extern int32_t 	esp8266_connect_ap(char* ssid,char* pswd);
extern int32_t 	esp8266_connect_server(char* mode,char* ip,uint16_t port);
extern int32_t 	esp8266_disconnect_server(void);
extern void 	esp8266_send_bytes(uint8_t *buf,uint32_t len);
extern void 	esp8266_send_str(char *buf);
extern void 	esp8266_send_at(char *str);
extern int32_t  esp8266_enable_multiple_id(uint32_t b);
extern int32_t 	esp8266_create_server(uint16_t port);
extern int32_t 	esp8266_close_server(uint16_t port);
extern int32_t 	esp8266_enable_echo(uint32_t b);
extern int32_t 	esp8266_reset(void);
extern int32_t 	esp8266_find(char *str,uint32_t timeout);
#endif
