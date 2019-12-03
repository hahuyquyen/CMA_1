extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <Arduino.h>
#include "FS.h"
#include <AsyncMqttClient.h>
#include <EEPROM.h>
#include "Config.h"
#include "Variable_wifi.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <U8g2lib.h>
#include "SD.h"
#include "RTClib.h"
#include <SPI.h>

RTC_DS3231 rtc;
SPIClass SDSPI(HSPI);
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0,/*CS=*/ U8X8_PIN_NONE,/*CS=*/ U8X8_PIN_NONE);// 

File root_CMA ;
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
AsyncWebServer server(web_port);
WiFiClient espClient;

//display_NV Display_NV;
Data_TH datatruyen_mqtt; 
//static intr_handle_t handle_console_uart1;
//static void IRAM_ATTR uart1_intr_handle(void *arg);
void TaskRFID( void * pvParameters );
void TaskCAN( void * pvParameters );
void Display( void * pvParameters );
void http_re( void * pvParameters );
void callback_mqtt(char* topic, byte* payload, unsigned int length) ;
boolean reconnect_mqtt();
bool loadWiFiConf();
void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in);
void wifi_connect(byte _mode = 0 ,wifi_mode_t wifi_mode = WIFI_AP,char *ssid = (char *)"",char *password = (char *)"",char *ap_ssid = (char *)"ESP AP");
void setupWiFiConf(void);
void setting_uart();
void WiFiEvent(WiFiEvent_t event);
size_t content_len;
void printProgress(size_t prg, size_t sz) {
  printf("Progress: %d%%\n", (prg*100)/content_len);
}

/*
 * Setup
 */
void khoiTaoGiaTri(){
    sprintf(mqttConfig.topicGetStatusACK, "/data/ack/%lu", ( unsigned long )stateMachine.idDevice) ;
    sprintf(mqttConfig.topicGetConfig, "/config/%lu", ( unsigned long )stateMachine.idDevice) ;
    strlcpy(inforServer.nameThanhPham[0], ramChoDuLieu, sizeof(inforServer.nameThanhPham[0]));
    strlcpy(inforServer.nameNhaCC[0], ramChoDuLieu, sizeof(inforServer.nameNhaCC[0]));
    strlcpy(giaiDoanCan.nameGiaiDoan[0], ramChoDuLieu, sizeof(giaiDoanCan.nameGiaiDoan[0]));
    stateMachine.deviceStatus = deviceSetting;
    stateMachine.bottonSelect = 0;
    inforServer.userSelectNhaCC = 0;
    inforServer.userSelectThanhPham = 0;
    inforServer.tongThanhPham = 0;
    inforServer.tongNhaCC = 0;
    inforServer.maNhaCC[0]=0;
    inforServer.maThanhPham[0]=0;
    giaiDoanCan.cheDoInOut=0;
    giaiDoanCan.tongGiaiDoan=0;
    giaiDoanCan.userSelecGiaiDoan=0;
    giaiDoanCan.maGiaiDoan[0]=0;
}
void setup()
{       pinMode(pinPower, OUTPUT);
    digitalWrite(pinPower, HIGH);
    Queue_can = xQueueCreate(5,sizeof(Data_CAN));
    Queue_RFID= xQueueCreate(5,sizeof(Data_RFID));
    Queue_RFID_NV= xQueueCreate(5,sizeof(Data_RFID));
    Queue_mqtt = xQueueCreate(10,sizeof(Data_TH));
    Queue_display = xQueueCreate(3,sizeof(Data_TH));
 //   Queue_can_interrup= xQueueCreate(3,sizeof(rfid_data));
    Queue_Time_blink= xQueueCreate(3,sizeof(uint16_t));
    xCountingSemaphore = xSemaphoreCreateCounting( 10, 0 );
    xSignal_FromRFID = xSemaphoreCreateCounting( 10, 0 );
    xSignal_Display_check = xSemaphoreCreateCounting( 2, 0 );
    xSignal_Display_checkdone = xSemaphoreCreateCounting( 2, 0 );
    xreset_id_nv = xSemaphoreCreateCounting( 2, 0 );
    xResetRfidMaRo = xSemaphoreCreateCounting( 2, 0 );
    EEPROM.begin(1024);
    WiFi.disconnect(true);
    WiFi.setSleep(false);
    if(!SPIFFS.begin(true)){printf("An Error has occurred while mounting SPIFFS\n");}
    Serial1.begin(9600, SERIAL_8N1, 26, 12); //12 tx 13 lÃ  rx(bau,se,rx,tx)
    Serial.begin(115200);
    SDSPI.begin(14,27,13,15); ///SCK,MISO,MOSI,ss
    if(!SD.begin(15,SDSPI)){Serial.println("Card Mount Failed");    }
    stateMachine.idDevice=EEPROM.readUInt(800);
    Serial.print("ID Device : ");
    Serial.println( stateMachine.idDevice);
    loadWiFiConf();
    khoiTaoGiaTri();
    if (! rtc.begin()) {Serial.println(F("Couldn't find RTC"));} 
    if (rtc.lostPower()) { rtc.adjust(DateTime(2019, 11, 21,11, 20, 0));}
#ifdef using_sta
    wifi_connect(0,WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
#else
    wifi_connect(1,WIFI_AP,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
#endif  
    wifi_staticip(WiFiConf.sta_ip,WiFiConf.sta_gateway,WiFiConf.sta_subnet);   
    WiFi.onEvent(WiFiEvent);
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
        wifiOnDisconnect();
    }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
        wifigotip();
    }, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
    setupWiFiConf();
    server.begin();
    Update.onProgress(printProgress);
    xTaskCreatePinnedToCore(
                        TaskRFID,   /* Function to implement the task */
                        "TaskRFID", /* Name of the task */
                        3072,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        15,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        1);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        TaskCAN,   /* Function to implement the task */
                        "TaskCAN", /* Name of the task */
                        3072,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        14,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        0);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        Display,   /* Function to implement the task */
                        "Display", /* Name of the task */
                        8192,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        12,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        1);  /* Core where the task should run */   
    xTaskCreatePinnedToCore(
                        http_re,   /* Function to implement the task */
                        "http_re", /* Name of the task */
                        4096,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        13,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        0);  /* Core where the task should run */       
  xTaskCreatePinnedToCore(
                        Check_button,   /* Function to implement the task */
                        "Check_button", /* Name of the task */
                        3072,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        11,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        0);  /* Core where the task should run */                             
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(WiFiConf.mqtt_server, atoi(WiFiConf.mqtt_port));       
  mqttClient.setCredentials(WiFiConf.mqtt_user,WiFiConf.mqtt_pass);      
  root_CMA = SD.open("/CMA");
}
/*
 * Main Loop luÃ´n cháº¡y Core 1
 */
void loop()
{  
  vTaskDelay(10);
  if (status_wifi_connect_AP == false){
    if (counter_wifi_disconnect == 30){
      vTaskDelay(500);
      WiFi.disconnect(true);
      printf("Chuyen\n");
      wifi_connect(2, WIFI_AP_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd,(char *)"esp32");
      counter_wifi_disconnect++;
    }
    else if (counter_wifi_disconnect < 30) {
        vTaskDelay(1000);
        counter_wifi_disconnect = counter_wifi_disconnect + 1;
        wifi_connect(0, WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
    }
  }
  /*
   * truyen mqtt nếu không truyền thì sẽ kiểm tra thẻ nhớ đọc và gửi
   */
  if(xQueueReceive( Queue_mqtt, &datatruyen_mqtt,  ( TickType_t ) 1 )== pdPASS ){
    truyen_mqtt();
    SD_lastTimeSendMQTT=xTaskGetTickCount();
  }
  else if ((status_mqtt_connect)&&((xTaskGetTickCount() - SD_lastTimeSendMQTT >timeTruyenMQTT))){
      /*
       * Check nếu có file còn lưu trong thu muc SD card thì đọc và gửi MQTT den server
       */
       SD_lastTimeSendMQTT=xTaskGetTickCount();
       if (statusGetAllSD == false){
           File file = root_CMA.openNextFile();
           if(file){ readFile(SD,file.name(),file.size());}
           else{  statusGetAllSD = true;
                  SD_lastTimeReadEnd=xTaskGetTickCount();
                  root_CMA.close();
           }
      }
      if((xTaskGetTickCount() - SD_lastTimeReadEnd > 60000)&&(statusGetAllSD)){
        SD_lastTimeReadEnd = xTaskGetTickCount();
        statusGetAllSD = false;
        Serial.println("reboot SD");
              root_CMA = SD.open("/CMA");             
       }    
  }
  /*
   * Lịch gửi server yêu cầu config
   */
  if ((status_mqtt_connect)&&(stateMachine.deviceStatus == deviceSetting)){  
    if((xTaskGetTickCount() - MQTT_lastTimeGetDataConfig>1000)|| (MQTT_lastTimeGetDataConfig == 0)){
      MQTT_lastTimeGetDataConfig = xTaskGetTickCount();
      checkSendMQTTConfig();
    }
  }
}
void sendMQTTConfig(uint8_t loaiconfig = 0 , uint8_t maboxung = 0){
      StaticJsonDocument<55> doc;
      char buffer[55];
      doc["i"]= stateMachine.idDevice;
      doc["t"]= loaiconfig ;
      doc["d"]=maboxung;
      serializeJson(doc, buffer);
      mqttClient.publish("/config", 0, true, buffer); 
}
void checkSendMQTTConfig(){
      if(inforServer.tongNhaCC == 0){sendMQTTConfig(1,0);}
      else if (giaiDoanCan.tongGiaiDoan == 0){sendMQTTConfig(2,0);}
      //Fix loi 01
      else if ((giaiDoanCan.userSelecGiaiDoan != 0)&&(inforServer.tongThanhPham == 0)&&(stateMachine.bottonSelect>1)){ // Chi gui yeu cau khi da chon khu vuc can và qua buoc chọn thành phảm
         Serial.print("dsgfdgfs ");
        Serial.println(giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan]);
       sendMQTTConfig(3,giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan]);
      }
}
