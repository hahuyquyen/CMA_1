extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include "FS.h"
#include <AsyncMqttClient.h>
#include <EEPROM.h>
#include "Config.h"
#include "Variable_wifi.h"
#include <driver/uart.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 27, /* data=*/ 26, /* CS=*/ U8X8_PIN_NONE);



AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
AsyncWebServer server(web_port);
WiFiClient espClient;

display_NV Display_NV;
Data_TH datatruyen_mqtt; 
static intr_handle_t handle_console_uart1;
static void IRAM_ATTR uart1_intr_handle(void *arg);
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
    pinMode(Pin_choose_IN_OUT, INPUT);
    if (digitalRead(Pin_choose_IN_OUT)==HIGH){
      status_IN_or_OUT = false;
    }
  //  Serial.begin(115200);
    EEPROM.begin(1024);
    WiFi.disconnect(true);

     Serial1.begin(9600, SERIAL_8N1, 13, 12); //12 tx 13 lÃ  rx
     Serial.begin(115200);

   // number_line_save_mqtt=EEPROM.readUInt(800);
    //printf("So line %u \n",number_line_save_mqtt);
    loaddata();
    loadWiFiConf();
    if(!SPIFFS.begin(true)){printf("An Error has occurred while mounting SPIFFS\n");}
  //  listDir(SPIFFS, "/", 0);
    check_file_exit();
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
    //setting_uart();
    chonloaica.PhanLoaiKV = PhanLoai::Not_Choose;
   /* chonloaica.SL_LoaiCa=5;
    chonloaica.STT_LoaiCa[1]=1;
    chonloaica.STT_LoaiCa[2]=3;
    chonloaica.STT_LoaiCa[3]=5;
    chonloaica.STT_LoaiCa[4]=7;
    chonloaica.STT_LoaiCa[5]=8;
    savedata();*/
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
}
/*
 * Main Loop luÃ´n cháº¡y Core 1
 */

void loop()
{  
  vTaskDelay(50);
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
       // printf("STA Disconnected\n");
        wifi_connect(0, WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
    }
  }
  if(xQueueReceive( Queue_mqtt, &datatruyen_mqtt,  ( TickType_t ) 2 )== pdPASS ){
    if (status_mqtt_connect){truyen_mqtt();}
    else {}
  }
  else if (status_mqtt_connect){}   
}


void Save_data_mqtt_to_local(){
}
void read_data_mqtt_to_local(){
}


 

