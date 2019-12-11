
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
#include <EasyButton.h>


// RTC
RTC_DS3231 rtc;
//LCD
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R2,/*CS=*/ U8X8_PIN_NONE,/*CS=*/ U8X8_PIN_NONE);
//SD CArd
SPIClass SDSPI(HSPI);
File root_CMA ;
// MQTT and server
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
AsyncWebServer server(web_port);
WiFiClient espClient;

//display_NV Display_NV;
Data_TH datatruyen_mqtt;

void TaskRFID( void * pvParameters );
void TaskCAN( void * pvParameters );
void Display( void * pvParameters );
void http_re( void * pvParameters );
void callback_mqtt(char* topic, byte* payload, unsigned int length) ;
//boolean reconnect_mqtt();
//bool loadWiFiConf();
//void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in);
//void wifi_connect(byte _mode = 0 ,wifi_mode_t wifi_mode = WIFI_AP,char *ssid = (char *)"",char *password = (char *)"",char *ap_ssid = (char *)"ESP AP");
//void setupWiFiConf(void);
//void setting_uart();
//void WiFiEvent(WiFiEvent_t event);
size_t content_len;
void printProgress(size_t prg, size_t sz) {
#ifdef debug_UART
  printf("Progress: %d%%\n", (prg * 100) / content_len);
#endif
}

//////////////////////////////////////////////////////////////////
////// Khoi tao gia trị mac dinh ////////////////////////////
//////////////////////////////////////////////////////////////////
void khoiTaoGiaTri() {
  inforServer.mqttConfig.setTopicACK(( unsigned long )stateMachine.idDevice);
  inforServer.mqttConfig.setTopicGetConfig(( unsigned long )stateMachine.idDevice);
#ifdef debug_UART
  Serial.println(inforServer.mqttConfig.topicGetStatusACK); 
  Serial.println(inforServer.mqttConfig.topicGetConfig);
#endif
  // inforServer.copyData(inforServer.thanhPham.arrayName[0], ramChoDuLieu);
  // inforServer.copyData(inforServer.nhaCC.arrayName[0], ramChoDuLieu);
  // inforServer.copyData(inforServer.giaiDoan.arrayName[0], ramChoDuLieu);
  strlcpy(inforServer.thanhPham.arrayName[0], ramChoDuLieu, sizeof(inforServer.thanhPham.arrayName[0]));
  strlcpy(inforServer.nhaCC.arrayName[0], ramChoDuLieu, sizeof(inforServer.nhaCC.arrayName[0]));
  strlcpy(inforServer.giaiDoan.arrayName[0], ramChoDuLieu, sizeof(inforServer.giaiDoan.arrayName[0]));

  stateMachine.deviceStatus = deviceSetting;
  stateMachine.bottonSelect = 0;
  inforServer.nhaCC.userSelect = 0;
  inforServer.thanhPham.userSelect = 0;
  inforServer.thanhPham.total = 0;
  inforServer.nhaCC.total = 0;
  inforServer.nhaCC.arrayType[0] = 0;
  inforServer.thanhPham.arrayType[0] = 0;
  inforServer.giaiDoan.cheDoInOut = 0;
  inforServer.giaiDoan.total = 0;
  inforServer.giaiDoan.userSelect = 0;
  inforServer.giaiDoan.arrayType[0] = 0;
  //   statusPeripheral.sdCard.statusConnect = false;
}
void setup()
{ pinMode(pinPower, OUTPUT);
  digitalWrite(pinPower, HIGH);
  Serial.begin(115200);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinLedRed, OUTPUT);
  /* Serial.println(statusPeripheral.wifi.statusConnectAP);
    Serial.println(statusPeripheral.mqtt.statusMqttConnect);
    Serial.println(statusPeripheral.mqtt.statusMqttConnect);
    Serial.println(statusPeripheral.mqtt.timeTruyenMQTT);*/
  Queue_can = xQueueCreate(3, sizeof(Data_CAN));
  Queue_RFID = xQueueCreate(3, sizeof(Data_RFID));
  Queue_RFID_NV = xQueueCreate(3, sizeof(Data_RFID));
  Queue_mqtt = xQueueCreate(5, sizeof(Data_TH));
  Queue_display = xQueueCreate(3, sizeof(Data_TH));
  //   Queue_can_interrup= xQueueCreate(3,sizeof(rfid_data));
  Queue_Time_blink = xQueueCreate(3, sizeof(uint16_t));
  xCountingSemaphore = xSemaphoreCreateCounting( 2, 0 );
  xSignal_FromRFID = xSemaphoreCreateCounting( 2, 0 );
  xSignal_Display_check = xSemaphoreCreateCounting( 2, 0 );
  xSignal_Display_checkdone = xSemaphoreCreateCounting( 2, 0 );
  xreset_id_nv = xSemaphoreCreateCounting( 2, 0 );
  xResetRfidMaRo = xSemaphoreCreateCounting( 2, 0 );
  //Get ID device
  EEPROM.begin(1024);
  stateMachine.getIdControl();
#ifdef debug_UART
  Serial.print("ID Device : ");
  Serial.println( stateMachine.idDevice);
#endif
  // Doc Eprom va khoi tao gia tri

  loadWiFiConf();
  khoiTaoGiaTri();
  if (!SPIFFS.begin(true)) {
#ifdef debug_UART

    printf("An Error has occurred while mounting SPIFFS\n");
#endif
  }
  //SD CArd
  SDSPI.begin(14, 27, 13, 15); ///SCK,MISO,MOSI,ss
  if (!SD.begin(15, SDSPI, 10000000)) {
#ifdef debug_UART
    Serial.println("Card Mount Failed");
#endif
  }
  else {
    statusPeripheral.sdCard.statusConnect = true;
#ifdef debug_UART
    Serial.println("SD card OK");
#endif
  }


  //RTC
  if (! rtc.begin()) {
#ifdef debug_UART
    Serial.println(F("Couldn't find RTC"));
#endif
  }
  if (rtc.lostPower()) {
#ifdef debug_UART
    Serial.println(F("Ghi Time"));
#endif
    rtc.adjust(DateTime(2019, 12, 4, 13, 53, 0));
  }
  //Wifi

  // WiFi.disconnect(true);
  // WiFi.setSleep(false);
#ifdef using_sta
  wifi_connect(0, WIFI_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd, WiFiConf.ap_ssid);
#else
  wifi_connect(1, WIFI_AP, WiFiConf.sta_ssid, WiFiConf.sta_pwd, WiFiConf.ap_ssid);
#endif
  wifi_staticip(WiFiConf.sta_ip, WiFiConf.sta_gateway, WiFiConf.sta_subnet);
  WiFi.onEvent(WiFiEvent);
  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
    wifiOnDisconnect(info);
  }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
    wifigotip();
  }, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
  setupWiFiConf();
  server.begin();
  Update.onProgress(printProgress);
  //Free RTOS
  xTaskCreatePinnedToCore(
    Check_button,   /* Function to implement the task */
    "Check_button", /* Name of the task */
    3072,      /* Stack size in words */
    NULL,       /* Task input parameter */
    11,          /* Priority of the task */
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
    TaskCAN,   /* Function to implement the task */
    "TaskCAN", /* Name of the task */
    3072,      /* Stack size in words */
    NULL,       /* Task input parameter */
    13,          /* Priority of the task */
    NULL,       /* Task handle. */
    0);  /* Core where the task should run */
  xTaskCreatePinnedToCore(
    http_re,   /* Function to implement the task */
    "http_re", /* Name of the task */
    4096,      /* Stack size in words */
    NULL,       /* Task input parameter */
    14,          /* Priority of the task */
    NULL,       /* Task handle. */
    0);  /* Core where the task should run */
  xTaskCreatePinnedToCore(
    TaskRFID,   /* Function to implement the task */
    "TaskRFID", /* Name of the task */
    3072,      /* Stack size in words */
    NULL,       /* Task input parameter */
    15,          /* Priority of the task */
    NULL,       /* Task handle. */
    1);  /* Core where the task should run */
  //MQTT
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(WiFiConf.mqtt_server, atoi(WiFiConf.mqtt_port));
  mqttClient.setCredentials(WiFiConf.mqtt_user, WiFiConf.mqtt_pass);
  root_CMA = SD.open("/CMA");
}
/*
   Main Loop luÃ´n cháº¡y Core 1
*/
void loop()
{
  vTaskDelay(30);
  if (statusPeripheral.wifi.statusConnectAP == false) {
    if ((statusPeripheral.wifi.counterWifiDisconnect == 15) && ( xTaskGetTickCount() - statusPeripheral.wifi.lastTimeConnect > 500)) {
      statusPeripheral.wifi.lastTimeConnect = xTaskGetTickCount();
      WiFi.disconnect(true);
      wifi_connect(2, WIFI_AP_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd, (char *)"esp32");
      statusPeripheral.wifi.counterWifiDisconnect ++;
#ifdef debug_UART
      printf("Wifi 2 che do: AP va STA\n");
#endif
    }
    else if ((statusPeripheral.wifi.counterWifiDisconnect < 15) && ( xTaskGetTickCount() - statusPeripheral.wifi.lastTimeConnect > 1000)) {
      statusPeripheral.wifi.lastTimeConnect = xTaskGetTickCount();
      statusPeripheral.wifi.counterWifiDisconnect = statusPeripheral.wifi.counterWifiDisconnect + 1;
      wifi_connect(0, WIFI_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd, WiFiConf.ap_ssid);
    }
  }
  /*
     truyen mqtt nếu không truyền thì sẽ kiểm tra thẻ nhớ đọc và gửi
  */
  if (xQueueReceive( Queue_mqtt, &datatruyen_mqtt,  ( TickType_t ) 1 ) == pdPASS ) {
    truyen_mqtt();
    statusPeripheral.sdCard.lastTimeSendMQTT = xTaskGetTickCount();
  }
  else if ((statusPeripheral.mqtt.statusMqttConnect) && ((xTaskGetTickCount() - statusPeripheral.sdCard.lastTimeSendMQTT > statusPeripheral.mqtt.timeTruyenMQTT))) {
    /*
       Check nếu có file còn lưu trong thu muc SD card thì đọc và gửi MQTT den server
    */
    statusPeripheral.sdCard.lastTimeSendMQTT = xTaskGetTickCount();
    // reconnect SD card
    if (statusPeripheral.sdCard.statusConnect == false) {
      if (statusPeripheral.mqtt.timeTruyenMQTT == 1000) {
        root_CMA.close();
      }
      SD.end() ;
      delay(200);
      if (!SD.begin(15, SDSPI, 6000000)) {
#ifdef debug_UART
        Serial.println("Card Mount Failed");
#endif
        statusPeripheral.mqtt.timeTruyenMQTT = 3000;
      }
      else {
        statusPeripheral.sdCard.statusConnect = true;
        statusPeripheral.mqtt.timeTruyenMQTT = 1000;
      }
#ifdef debug_UART
      Serial.println(" Reinit SD Card");
#endif
    }
    // open next file
    else if (statusPeripheral.sdCard.statusGetAllFile == false) {
      File file = root_CMA.openNextFile();
      if (file) {
        if (!file.isDirectory()) {
          if (file.name()[5] == 'O') { //delete
            Serial.print("delete file ");
            Serial.println(file.name());
            deleteFile(SD, file.name());
          }
          else readFile(SD, file.name(), file.size());
        }
      }
      else {
        root_CMA.close();
        statusPeripheral.sdCard.statusGetAllFile = true;
        statusPeripheral.sdCard.lastTimeReadEnd = xTaskGetTickCount();
      }
    }
    // Neu da mo het thi cho 60s va mo lai và check folder
    else if ((xTaskGetTickCount() - statusPeripheral.sdCard.lastTimeReadEnd > 30 * 60000) && (statusPeripheral.sdCard.statusGetAllFile)) { //30 phut moi mo lai va
      statusPeripheral.sdCard.lastTimeReadEnd = xTaskGetTickCount();
      statusPeripheral.sdCard.statusGetAllFile = false;

      root_CMA = SD.open("/CMA");

      if (!root_CMA) {
        statusPeripheral.sdCard.statusConnect = false;
      }
#ifdef debug_UART
      Serial.println("Read Folder SD CATRĐ");
#endif
    }

  }
  
    //////////////////////////////////////////////////////////////////
    ////// lasttime yêu cau server tra data ////////////////////////////
    //////////////////////////////////////////////////////////////////
    if ((statusPeripheral.mqtt.statusMqttConnect) && (stateMachine.deviceStatus == deviceSetting)) {
      if ((xTaskGetTickCount() - statusPeripheral.mqtt.lastTimeGetDataConfig > 1000) || (statusPeripheral.mqtt.lastTimeGetDataConfig == 0)) {
        statusPeripheral.mqtt.lastTimeGetDataConfig = xTaskGetTickCount();
        checkSendMQTTConfig();
      }
    }
}
  void sendMQTTConfig(uint8_t loaiconfig = 0 , uint8_t maboxung = 0) {
    StaticJsonDocument<55> doc;
    char buffer[55];
    doc["i"] = stateMachine.idDevice;
    doc["t"] = loaiconfig ;
    doc["d"] = maboxung;
    serializeJson(doc, buffer);
    mqttClient.publish("/config", 0, true, buffer);
  }
  void checkSendMQTTConfig() {
    if (inforServer.nhaCC.total == 0) {
      sendMQTTConfig(1, 0);
    }
    else if (inforServer.giaiDoan.total == 0) {
      sendMQTTConfig(2, 0);
    }
    //Fix loi 01
    else if ((inforServer.giaiDoan.userSelect != 0) && (inforServer.thanhPham.total == 0) && (stateMachine.bottonSelect > 1)) { // Chi gui yeu cau khi da chon khu vuc can và qua buoc chọn thành phảm
      sendMQTTConfig(3, inforServer.giaiDoan.arrayType[inforServer.giaiDoan.userSelect]);
    }
  }
