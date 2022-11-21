#ifndef _ALIYUN_MQTT_H_
#define	_ALIYUN_MQTT_H_

#include "stm32f4xx.h"

//#define MQTT_BROKERADDRESS 		"iot-06z00fwmv0qc9zl.mqtt.iothub.aliyuncs.com"
//#define MQTT_CLIENTID 			"0001|securemode=3,signmethod=hmacsha1|"
//#define MQTT_USARNAME 			"Embedded_control&ht9k9M4qhAj"
//#define MQTT_PASSWD 			"7373DB0640BBB5BC56F310CD31373220C0254615"
//#define	MQTT_PUBLISH_TOPIC 		"/sys/ht9k9M4qhAj/Embedded_control/thing/event/property/post"
//#define MQTT_SUBSCRIBE_TOPIC 	"/sys/ht9k9M4qhAj/Embedded_control/thing/service/property/set"

#define MQTT_BROKERADDRESS 		"t.yoyolife.fun"
#define MQTT_CLIENTID 			"chen"
#define MQTT_USARNAME 			"3071f580444edae15becd1c66714c780"
#define MQTT_PASSWD 			"123456"
#define	MQTT_PUBLISH_TOPIC 		"/iot/1879/chen/sub/"
#define MQTT_SUBSCRIBE_TOPIC 	"/iot/1879/chen/upload/"


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))


//MQTT连接服务器
extern int32_t mqtt_connect(char *client_id,char *user_name,char *password);

//MQTT消息订阅
extern int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);

//MQTT消息发布
extern uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);

//MQTT发送心跳包
extern int32_t mqtt_send_heart(void);

extern int32_t esp8266_mqtt_init(void);

extern void mqtt_disconnect(void);

extern void mqtt_report_devices_status(void);

#endif

