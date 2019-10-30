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
/*
 * #include <WebServer.h>
#include <WiFi.h>
 */
#define BUF_SIZE (255)
//#define UART_NUM_1 UART_NUM_1
//#define UART_NUM_2 UART_NUM_2
#define RD_BUF_SIZE (BUF_SIZE)
//static QueueHandle_t uart0_queue;
static intr_handle_t handle_console_uart1;
static intr_handle_t handle_console_uart2;
uint16_t urxlen;
static void IRAM_ATTR uart1_intr_handle(void *arg);
static void IRAM_ATTR uart_intr_handle2(void *arg);
/*###########################
 UART 0 rxPin = 3;txPin = 1;
 UART 1 rxPin = 9;txPin = 10;
 UART 2 rxPin = 16;txPin = 17;
*/
SemaphoreHandle_t xCountingSemaphore;
AsyncWebServer server(4999);
//WiFiClient espClient;
//PubSubClient client(espClient);
//############################
const char* update_path = "/firmware";
const char* update_username = "CMA";
const char* update_password = "123456";
//##########################################
static int taskCore = 1;
void TaskRFID( void * pvParameters );
void TaskCAN( void * pvParameters );
void Display( void * pvParameters );
void callback_mqtt(char* topic, byte* payload, unsigned int length) ;
boolean reconnect_mqtt();
bool loadWiFiConf();
void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in);
void wifi_connect(wifi_mode_t wifi_mode,char *ssid,char *password,char *ap_ssid);
void setupWiFiConf(void);
void setting_uart();
void WiFiEvent(WiFiEvent_t event);
data_user congnhan;
long lastReconnectAttempt = 0;
long lastMsg=0;
char ledState[64];
size_t content_len;

void http_re( void * pvParameters ){

    while(true){
      vTaskDelay(10);
    }
}
void printProgress(size_t prg, size_t sz) {
  printf("Progress: %d%%\n", (prg*100)/content_len);
}
void setup()
{
   // Serial.begin(115200);
    printf("Begin\n");
    EEPROM.begin(1024);
    WiFi.disconnect(true);
    loadWiFiConf();
    if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
    }
   
    wifi_connect(WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);  
    wifi_staticip(WiFiConf.sta_ip,WiFiConf.sta_gateway,WiFiConf.sta_subnet);   
  //  wifi_connect(WIFI_AP,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
    WiFi.onEvent(WiFiEvent);
    WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
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
                        1,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        taskCore);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        TaskCAN,   /* Function to implement the task */
                        "TaskCAN", /* Name of the task */
                        2048,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        1,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        1);  /* Core where the task should run */
    xTaskCreatePinnedToCore(
                        Display,   /* Function to implement the task */
                        "Display", /* Name of the task */
                        2048,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        1,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        1);  /* Core where the task should run */   
    xTaskCreatePinnedToCore(
                        http_re,   /* Function to implement the task */
                        "http_re", /* Name of the task */
                        2048,      /* Stack size in words */
                        NULL,       /* Task input parameter */
                        1,          /* Priority of the task */
                        NULL,       /* Task handle. */
                        0);  /* Core where the task should run */ 
    
                        
  //client.setServer("dsds", 1883);
  //client.setCallback(callback_mqtt);
                        
}

void loop()
{       //server.handleClient();
//printf("Main Loop: priority = %d",uxTaskPriorityGet(NULL));
  
        vTaskDelay(100);
  /*
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect_mqtt()) {
        lastReconnectAttempt = 0;
      }
    }
  } else { client.loop();}
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    client.publish("outTopic", ":Dsd");
  }  */    
}
/*
void callback_mqtt(char* topic, byte* payload, unsigned int length) {

}

boolean reconnect_mqtt() {
  if (client.connect("arduinoClient")) {
   // if (clientmqtt.connect("arduinoClient")) {
    client.publish("dsd","Reconnect");
    client.subscribe("5");
  }
  return client.connected();
}
 */
