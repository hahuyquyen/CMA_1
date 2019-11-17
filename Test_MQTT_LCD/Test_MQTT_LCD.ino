/*
This example uses FreeRTOS softwaretimers as there is no built-in Ticker library
*/

#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include <EEPROM.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
uint8_t stt_luu = 0;
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 27, /* data=*/ 26, /* CS=*/ U8X8_PIN_NONE);
static struct WiFiConfStruct {
  char sta_ssid[30][50];
} WiFiConf;
uint16_t address_
static struct Lo_ca {
  char sta_ssid[30][50];
} Lo_ca;
#define WIFI_SSID "CMA"
#define WIFI_PASSWORD "cma2018LHP515"

#define MQTT_HOST "tailor.cloudmqtt.com"
//"ddxnzgvu",  "0FZ_4Ml_KkoQ"
#define MQTT_PORT 18669

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    xTimerStart(wifiReconnectTimer, 0);
        break;
        default: break;     
    }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  uint16_t packetIdSub = mqttClient.subscribe("test", 1);

}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
  
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.print("  payload: ");
   Serial.print(stt_luu);
    Serial.print("  - ");
  Serial.print(payload);
      Serial.print("  - ");
  Serial.println(len);
  strcpy( WiFiConf.sta_ssid[stt_luu] , payload);
   u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
    u8g2.setCursor(10, 12);
    u8g2.print(payload);
    u8g2.setCursor(10, 32);
    u8g2.print(WiFiConf.sta_ssid[stt_luu]);
    u8g2.sendBuffer();  
    savedata();
    stt_luu ++ ;
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}
bool loaddata() {
    uint16_t bien=0;
  // Serial.println(sizeof(WiFiConf));
    for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
      bien ++ ;
      *((char*)&WiFiConf + t) = EEPROM.read(190 + t); //& lÃ  Ä‘á»‹a chá»‰  cá»§a biáº¿n Struc, *lÃ  data tá»©c lÃ  gÃ¡n data trong Ã´ nhá»› struc báº±ng eprom Ä‘á»�c dc (char*) lÃ  Ã©p kiá»ƒu dá»¯ liá»‡u
     /* Serial.print(700 + t);
      Serial.print("-");
      Serial.println(EEPROM.read(700 + t));*/
    }
    Serial.println("vvvv");
  // return true;
}
/*
 * LÆ°u Eprom thÃ´ng tin wifi
 */
void savedata(void) {
  for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
    EEPROM.write(190 + t, *((char*)&WiFiConf + t));
  }
  EEPROM.commit();
}
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
   if (!EEPROM.begin(5000))
  {
    Serial.println("failed to initialise EEPROM"); 
  }
  u8g2.begin();
  loaddata();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
  u8g2.setCursor(0, 12);
  u8g2.print(WiFiConf.sta_ssid[0]);
  u8g2.setCursor(0, 24);
  u8g2.print(WiFiConf.sta_ssid[1]);
  u8g2.setCursor(0, 36);
  u8g2.print(WiFiConf.sta_ssid[2]);
  u8g2.setCursor(0, 48);
  u8g2.print(WiFiConf.sta_ssid[3]);
  u8g2.sendBuffer(); 
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  WiFi.onEvent(WiFiEvent);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCredentials("ddxnzgvu",  "0FZ_4Ml_KkoQ");
  connectToWifi();
}

void loop() {
}
