#include <EEPROM.h>
#include "Config.h"
#include "Variable_wifi.h"
#include <driver/uart.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>

AsyncWebServer server(web_port);
WiFiClient espClient;
PubSubClient client(espClient);

display_NV Display_NV;
data_user datatruyen_mqtt; 
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
    wifi_connect(0,WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
#else
    wifi_connect(1,WIFI_AP,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
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
                        8192,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        5,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        1);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        TaskCAN,   /* Function to implement the task */
                        "TaskCAN", /* Name of the task */
                        8192,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        4,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        0);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        Display,   /* Function to implement the task */
                        "Display", /* Name of the task */
                        8192,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        2,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        1);  /* Core where the task should run */   
    xTaskCreatePinnedToCore(
                        http_re,   /* Function to implement the task */
                        "http_re", /* Name of the task */
                        8192,      /* Stack size in words */
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
  if (status_wifi_connect_AP == false){
    if (counter_wifi_disconnect == 50){
      WiFi.disconnect(true);
      printf("Chuyen\n");
      wifi_connect(2, WIFI_AP_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd,(char *)"esp32");
      counter_wifi_disconnect++;
    }
    else if (counter_wifi_disconnect < 50) {
        vTaskDelay(500);
        counter_wifi_disconnect = counter_wifi_disconnect + 1;
        printf("STA Disconnected\n");
        wifi_connect(0, WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
    }
    else printf("qua 50\n");
    vTaskDelay(500);
  }
  else if (!client.connected()) {
    long now = xTaskGetTickCount();
    if (now - _time_lastconnect_mqtt > 5000) {
      _time_lastconnect_mqtt = now;
      if (reconnect_mqtt()) {
        _time_lastconnect_mqtt = 0;
      }
    }
  } else { client.loop();}
  if(xQueueReceive( Queue_mqtt, &datatruyen_mqtt,  ( TickType_t ) 2 )== pdPASS ){
      /*Cách 1: Cấp phát không dùng con trỏ. Bộ nhớ nằm trong Stack.
       * Stack là bộ nhớ cố định nên nhiều lúc tiết kiệm ta dùng bộ nhớ heap
       * 819990 bytes (41%)   bytes. 40888 , leaving 286792
       * 819018                      40888           286792
       */
     /* String input ="{\"id\":\"" + String(datatruyen_mqtt.id_RFID) + "\",\"device\":\"" + String(chedo) + "\",\"data\":["+String(datatruyen_mqtt.data_weight)+","+ String(datatruyen_mqtt.data_tare)+"]}";
      int chieudai_mqtt=input.length(); 
      char msg[chieudai_mqtt+1];
      input.toCharArray(msg, sizeof(msg));
      client.publish(datatruyen_mqtt.id_RFID, msg);*/
      //##################################################################
      /*
       * Dung heap 
       */
      size_t size_needed = snprintf(NULL, 0, "{\"id\":\"%s\",\"device\":\"%s\",\"data\":[%g,%g]}", datatruyen_mqtt.id_RFID, chedo, datatruyen_mqtt.data_weight,datatruyen_mqtt.data_tare) + 1;
      char* msg1 = (char*)malloc(size_needed);
      if (msg1 != NULL) {  // malloc ok
       sprintf(msg1, "{\"id\":\"%s\",\"device\":\"%s\",\"data\":[%g,%g]}", datatruyen_mqtt.id_RFID,chedo, datatruyen_mqtt.data_weight,datatruyen_mqtt.data_tare) ;
       printf("mqtt %s\n",msg1);
       client.publish(datatruyen_mqtt.id_RFID, msg1);
      }
      else {printf("Ko du heap, reset\n");ESP.restart(); }
      free(msg1);
      
      /*
       * Dung 1 bo nho
       */
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
 
