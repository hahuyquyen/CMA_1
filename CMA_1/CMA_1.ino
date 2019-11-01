#include <EEPROM.h>
#include "Variable_html.h"
#include "Variable_wifi.h"
#include <driver/uart.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
//#include <HardwareSerial.h>
//HardwareSerial Serial2(2);
#define using_sta true
#define BUF_SIZE (125)
#define chedo 0  // 0 là IN,1 là OUT
static intr_handle_t handle_console_uart1;
//uint16_t urxlen;
static void IRAM_ATTR uart1_intr_handle(void *arg);
//static void IRAM_ATTR uart_intr_handle2(void *arg);
/*###########################
 UART 0 rxPin = 3;txPin = 1;
 UART 1 rxPin = 9;txPin = 10;
 UART 2 rxPin = 16;txPin = 17;
*/
SemaphoreHandle_t xCountingSemaphore;
SemaphoreHandle_t xSignal_FromRFID;
QueueHandle_t Queue_can;
QueueHandle_t Queue_mqtt;
QueueHandle_t Queue_display;
QueueHandle_t Queue_can_interrup;
/**************************** 
 *  Struct Data 
 ***************************/
typedef struct Data_user{
  uint8_t id;
  char id_RFID[25];
  double data_weight;
  double data_tare;
} data_user;
typedef struct Display{
  uint8_t id;
  char name_nv[50];
  double cannang;
  double tiencong;
} display_NV;
display_NV Display_NV;
data_user datatruyen_mqtt;  

/*
 * Web Server và MQTT
 */
AsyncWebServer server(4999);
WiFiClient espClient;
PubSubClient client(espClient);
//############################
const char* update_path = "/firmware";
const char* update_username = "CMA";
const char* update_password = "123456";
//##########################################
static int taskCore = 1;
void TaskRFID( void * pvParameters );
void TaskCAN( void * pvParameters );
void Display( void * pvParameters );
void http_re( void * pvParameters );
void callback_mqtt(char* topic, byte* payload, unsigned int length) ;
boolean reconnect_mqtt();
bool loadWiFiConf();
void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in);
void wifi_connect(wifi_mode_t wifi_mode,char *ssid,char *password,char *ap_ssid);
void setupWiFiConf(void);
void setting_uart();
void WiFiEvent(WiFiEvent_t event);
size_t content_len;
/*
 * Biến
 */
unsigned long _time_lastconnect_mqtt=0;
extern uint8_t rfid_data[20];

void printProgress(size_t prg, size_t sz) {
  printf("Progress: %d%%\n", (prg*100)/content_len);
}

/*
 * Setup
 */
void setup()
{   Queue_can = xQueueCreate(5,sizeof(data_user));
    Queue_mqtt = xQueueCreate(10,sizeof(data_user));
    Queue_display = xQueueCreate(3,sizeof(display_NV));
    Queue_can_interrup= xQueueCreate(3,sizeof(rfid_data));
    EEPROM.begin(1024);
    WiFi.disconnect(true);
    loadWiFiConf();
    if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
    }
#ifdef using_sta
    wifi_connect(WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
#else
    wifi_AP(WIFI_AP,"CMA_AU","123789456");
#endif  
    wifi_staticip(WiFiConf.sta_ip,WiFiConf.sta_gateway,WiFiConf.sta_subnet);   
    WiFi.onEvent(WiFiEvent);
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
        wifiOnDisconnect();
    }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
    setupWiFiConf();
    server.begin();
    Update.onProgress(printProgress);
    setting_uart();
    xTaskCreatePinnedToCore(
                        TaskRFID,   /* Function to implement the task */
                        "TaskRFID", /* Name of the task */
                        2048,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        5,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        taskCore);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        TaskCAN,   /* Function to implement the task */
                        "TaskCAN", /* Name of the task */
                        2048,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        4,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        0);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        Display,   /* Function to implement the task */
                        "Display", /* Name of the task */
                        2048,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        2,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        1);  /* Core where the task should run */   
    xTaskCreatePinnedToCore(
                        http_re,   /* Function to implement the task */
                        "http_re", /* Name of the task */
                        2048,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        3,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        0);  /* Core where the task should run */ 
    
                    
  client.setServer(WiFiConf.mqtt_server, atoi(WiFiConf.mqtt_port));
  client.setCallback(callback_mqtt);
  reconnect_mqtt();                   
}
/*
 * Main Loop luôn chạy Core 1
 */

void loop()
{  
  vTaskDelay(50);
  if (!client.connected()) {
    long now = xTaskGetTickCount();
    if (now - _time_lastconnect_mqtt > 5000) {
      _time_lastconnect_mqtt = now;
      if (reconnect_mqtt()) {
        _time_lastconnect_mqtt = 0;
      }
    }
  } else { client.loop();}
  if(xQueueReceive( Queue_mqtt, &datatruyen_mqtt,  ( TickType_t ) 2 )== pdPASS ){
      String input ="{\"id\":\"" + String(datatruyen_mqtt.id_RFID)+ "\",\"device\":\"" + String(chedo) + "\",\"data\":["+String(datatruyen_mqtt.data_weight)+","+ String(datatruyen_mqtt.data_tare)+"]}";
      int chieudai_mqtt=input.length(); 
      char msg[chieudai_mqtt+1];
      input.toCharArray(msg, sizeof(msg));
      client.publish(datatruyen_mqtt.id_RFID, msg);
  }    
}

void callback_mqtt(char* topic, byte* payload, unsigned int length) {
  if (strcmp(WiFiConf.mqtt_subto1,topic) == 0){printf("vung 1 \n");
  /*StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);*/
  xQueueSend( Queue_display, &Display_NV, ( TickType_t ) 2  );
   /* char message[length + 1];
    *  char* p = (char*)malloc(length + 1);
    *  memcpy(p,payload,length);
    *  free(p);
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';*/
  }
  if (strcmp(WiFiConf.mqtt_subto2,topic) == 0){printf("vung 2 \n");}
  if (strcmp(WiFiConf.mqtt_subto3,topic) == 0){printf("vung 3 \n");}
}

boolean reconnect_mqtt() {
  if (client.connect("NHANESP32",WiFiConf.mqtt_user,WiFiConf.mqtt_pass)) {
        if (WiFiConf.mqtt_subto1[0] != 'x'){client.subscribe( WiFiConf.mqtt_subto1 );}
        if (WiFiConf.mqtt_subto2[0] != 'x'){client.subscribe(WiFiConf.mqtt_subto2);}
        if (WiFiConf.mqtt_subto3[0] != 'x'){client.subscribe(WiFiConf.mqtt_subto3);}  
  }
  return client.connected();
}
 
