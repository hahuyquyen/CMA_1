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

///#include <driver/uart.h>
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
/*
 * WiFi.macAddress()
 */
void printProgress(size_t prg, size_t sz) {
  printf("Progress: %d%%\n", (prg*100)/content_len);
}

/*
 * Setup
 */
void khoiTaoGiaTri(){
    sprintf(MQTT_TOPIC.dataAck, "/data/ack/%lu", ( unsigned long )idDevice) ;
    sprintf(MQTT_TOPIC.configGetId, "/config/%lu", ( unsigned long )idDevice) ;
    strlcpy(inforServer.nameThanhPham[0], ramChoDuLieu, sizeof(inforServer.nameThanhPham[0]));
    strlcpy(inforServer.nameSoLo[0], ramChoDuLieu, sizeof(inforServer.nameSoLo[0]));
    strlcpy(inforServer.nameLoaiCa[0], ramChoDuLieu, sizeof(inforServer.nameLoaiCa[0]));
    state_Running_conf::state_Running = state_Running_conf::Setting;
    Status_setting.state_select = 0;
    inforServer.PhanLoaiKV = PhanLoai::Not_Choose;
    inforServer.userSelectLoaiCa = 0;
    inforServer.userSelectNhaCC = 0;
    inforServer.userSelectThanhPham = 0;
    inforServer.tongThanhPham = 0;
    inforServer.tongNhaCC = 0;
    inforServer.tongLoaiCa = 0;
    inforServer.maLoaica[0]=0;
    inforServer.maNhaCC[0]=0;
    inforServer.maThanhPham[0]=0;
}
void setup()
{   Queue_can = xQueueCreate(5,sizeof(Data_CAN));
    Queue_RFID= xQueueCreate(5,sizeof(Data_RFID));
    Queue_RFID_NV= xQueueCreate(5,sizeof(Data_RFID));
    Queue_mqtt = xQueueCreate(10,sizeof(Data_TH));
    Queue_display = xQueueCreate(3,sizeof(Data_TH));
    Queue_can_interrup= xQueueCreate(3,sizeof(rfid_data));
    Queue_Time_blink= xQueueCreate(3,sizeof(uint16_t));
    xCountingSemaphore = xSemaphoreCreateCounting( 10, 0 );
    xSignal_FromRFID = xSemaphoreCreateCounting( 10, 0 );
    xSignal_Display_check = xSemaphoreCreateCounting( 10, 0 );
    xSignal_Display_checkdone = xSemaphoreCreateCounting( 2, 0 );
    xreset_id_nv = xSemaphoreCreateCounting( 2, 0 );
    xResetRfidMaRo = xSemaphoreCreateCounting( 2, 0 );
    EEPROM.begin(1024);
    WiFi.disconnect(true);
    if(!SPIFFS.begin(true)){printf("An Error has occurred while mounting SPIFFS\n");}
    Serial1.begin(9600, SERIAL_8N1, 26, 12); //12 tx 13 lÃ  rx(bau,se,rx,tx)
    Serial.begin(115200);
    SDSPI.begin(14,27,13,15); ///SCK,MISO,MOSI,ss
    if(!SD.begin(15,SDSPI)){Serial.println("Card Mount Failed");    }
    idDevice=EEPROM.readUInt(800);
    Serial.print("ID Device : ");
    Serial.println( idDevice);
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
                        8192,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        15,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        1);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        TaskCAN,   /* Function to implement the task */
                        "TaskCAN", /* Name of the task */
                        8192,      /* Stack size in words */
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
                        8192,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        13,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        0);  /* Core where the task should run */       
  xTaskCreatePinnedToCore(
                        Check_button,   /* Function to implement the task */
                        "Check_button", /* Name of the task */
                        2048,      /* Stack size in words */
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
//  printf("END set \n");
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
    timeCheckMQTT_SD=xTaskGetTickCount();
  }
  else if ((status_mqtt_connect)&&((xTaskGetTickCount() - timeCheckMQTT_SD >timeTruyenMQTT))){
      /*
       * Check nếu có file còn thì đọc và gửi MQTT
       */
       timeCheckMQTT_SD=xTaskGetTickCount();
       if (statusGetAllSD == false){
           File file = root_CMA.openNextFile();
           if(file){ readFile(SD,file.name(),file.size());}
           else{  statusGetAllSD = true;
                  timeEndReadSD=xTaskGetTickCount();
                  root_CMA.close();
           }
      }
      if((xTaskGetTickCount() - timeEndReadSD > 60000)&&(statusGetAllSD)){
        timeEndReadSD = xTaskGetTickCount();
        statusGetAllSD = false;
        Serial.println("reboot SD");
              root_CMA = SD.open("/CMA");             
       }    
  }
  /*
   * Lịch gửi server yêu cầu config
   */
  if ((status_mqtt_connect)&&(state_Running_conf::state_Running == state_Running_conf::Setting)){  
    if((xTaskGetTickCount() - timeFirstGetDataFromServer>30000)|| (timeFirstGetDataFromServer == 0)){
      timeFirstGetDataFromServer = xTaskGetTickCount();
      StaticJsonDocument<35> doc;
      char buffer[35];
      doc["i"]= idDevice;
      if (inforServer.tongLoaiCa == 0){
        doc["t"]= 1 ;
        serializeJson(doc, buffer);
        mqttClient.publish("/config", 0, true,buffer); 
      }
      if (inforServer.tongThanhPham == 0){
        doc["t"]= 3 ;
        serializeJson(doc, buffer);
        mqttClient.publish("/config", 0, true, buffer); 
      }
      if(inforServer.tongNhaCC== 0){
        doc["t"]= 2 ;
        serializeJson(doc, buffer);
        mqttClient.publish("/config", 0, true, buffer); 
      }
    }
  }
}

 
