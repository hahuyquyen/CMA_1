extern "C" {
	//#include <dummy.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <WiFi.h>
//#include <ETH.h>
#include "Config.h"
#include <Arduino.h>
#include "FS.h"
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include <Update.h>
#include "SD.h"
#include "RTClib.h"
#include <SPI.h>
#include <EasyButton.h>
#include "Variable_wifi.h"
#include "VarriableData.h"
#define WEBSOCKET_DISABLED true
#ifdef debug_Web
#include <RemoteDebug.h>
#include <DNSServer.h>
#include "ESPmDNS.h"
RemoteDebug Debug;
#endif
#include "Digi28SS.h"
#include "JT2850.h"
#include "Modbus485.h"
#include "TongHopData.h"

struct stateMachineConf stateMachine;
struct statusPeripheralConf statusPeripheral;
modbus485HMI mb;
// RTC
RTC_DS3231 rtc;
DateTime timeStamp;
struct Data_TH dataEncoderJsonMqtt;
//LCD
//SD CArd
SPIClass SDSPI(HSPI);
File root_CMA;
// MQTT and server
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
AsyncWebServer server(web_port);
WiFiClient espClient;

//display_NV Display_NV;

//#pragma once
/*
SaveData
*/
//bool loadWiFiConf();
bool saveWiFiConf(void){ return setChar((char*)&WiFiConf, sizeof(WiFiConf)); }
/*
Button
*/
//void onPressed_left();
void onPressed_ok();
void onPressed_right();
//void onPressedError();
void onPressedExit();
void onPressedPower();
/*
functions
*/
boolean compareWithZero(uint8_t* data);
uint8_t GetSttKhuVuc();
void SetTimeRtc(uint32_t timestampSave);
/*
MQTT
*/

void SendDataMqtt(char* topic, char* data);
void EncoderJsonMqtt();
void ConnectToMqtt();
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMqttPublish(uint16_t packetId);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void SendDataConfigMqtt(uint8_t loaiconfig, uint8_t maboxung);
void checkSendMQTTConfig();

/*
SD Card
*/
void SdOpenNextFile(File* fileMaster);
void SdReInit(File* masterFile);
void SdReadFile(fs::FS& fs, const char* path, uint32_t len);
void SdRenameFile(fs::FS& fs, const char* path1, const char* path2);
void SdReOpenFolder();
void SdWriteFile(fs::FS& fs, const char* path, const char* message);
void SdDeleteFile(fs::FS& fs, const char* path);
/*
File webserver
*/
void handleDoUpdate(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final);
String processor(const String& var);
void setupWiFiConf(void);
void getVariHtml(AsyncWebServerRequest* request, const __FlashStringHelper* id, char* dataget, size_t chieudai);
void getVariIntHtml(AsyncWebServerRequest* request, const __FlashStringHelper* id, uint8_t* dataget);
/*
trong File WifiSetting
*/
void WiFiEvent(WiFiEvent_t event);
void wifigotip();
void wifiOnDisconnect(WiFiEventInfo_t info);
void parseBytes1(const char* str, char sep, int address, int maxBytes, int base);
void wifi_connect(byte _mode, wifi_mode_t wifi_mode, char* ssid, char* password, char* ap_ssid, bool statusModbus);
void wifi_staticip(char* ip_in, char* gateway_in, char* subnet_in);
int32_t getRSSI(const char* target_ssid);

/*
*/
//void send485HMI(uint16_t address, uint16_t* kytu, uint8_t numByte);
void send485PageKg(uint16_t page, uint32_t kg);
void send485Kg(uint32_t kg);
void sendSSID485();
void send485PageAndData(uint16_t page, boolean dataSend = true);
void LcdSeclectMode(uint8_t modeDisplay, Data_TH* dataLCDTH);

void Check_button(void* pvParameters);
//void TaskRFID(void* pvParameters); // trong file JT2850
//void TaskCAN(void* pvParameters); // trong file Digi28ss
void Display(void* pvParameters);
//void http_re(void* pvParameters); // trong file TongHopData.cpp


size_t content_len;
void printProgress(size_t prg, size_t sz) {
#ifdef debug_UART
	printf("Progress: %d%%\n", (prg * 100) / content_len);
#endif
#ifdef debug_Web
	DebugData("Progress: %d%%", (prg * 100) / content_len);
#endif
}

/*
#ifndef WEBSOCKET_DISABLED // Only if Web socket enabled (RemoteDebugApp)
// If enabled, you can change the port here (8232 is default)
#define WEBSOCKET_PORT 83
#endif
*/
////////////////////////////////////////////////////////////////////
////// Khoi tao gia trị mac dinh //////////////////////////////////
//////////////////////////////////////////////////////////////////
void khoiTaoGiaTri(boolean firstTime = true) {
	if (firstTime) {
		inforServer.mqttConfig.setTopicACK((unsigned long)stateMachine.hardwareId);
		inforServer.mqttConfig.setTopicGetConfig((unsigned long)stateMachine.hardwareId);
#ifdef debug_UART
		Serial.println(inforServer.mqttConfig.topicGetStatusACK);
		Serial.println(inforServer.mqttConfig.topicGetConfig);
#endif
	}
	statusPeripheral.timeStampServer = 0;
	stateMachine.getGiaiDoan();
	stateMachine.getKV();
	if (stateMachine.giaidoanINOUT == cheDoIN) {
		strlcpy(inforServer.thanhPham.arrayName[0], ramChoDuLieu, sizeof(inforServer.thanhPham.arrayName[0]));
		strlcpy(inforServer.nhaCC.arrayName[0], ramChoDuLieu, sizeof(inforServer.nhaCC.arrayName[0]));
	}
	else {
		strlcpy(inforServer.thanhPham.arrayName[0], "Bỏ Qua", sizeof(inforServer.thanhPham.arrayName[0]));
		strlcpy(inforServer.nhaCC.arrayName[0], "Bỏ Qua", sizeof(inforServer.nhaCC.arrayName[0]));
	}
	stateMachine.deviceStatus = deviceSetting;
	stateMachine.bottonSelect = 0;
	inforServer.nhaCC.userSelect = 0;
	inforServer.thanhPham.userSelect = 0;
	inforServer.thanhPham.total = 0;
	inforServer.nhaCC.total = 0;
	inforServer.nhaCC.arrayType[0] = 0;
	inforServer.thanhPham.arrayType[0] = 0;
}
void setup()
{
	pinMode(pinPower, OUTPUT);
	digitalWrite(pinPower, HIGH);
	Serial.begin(187500, SERIAL_8N1);
	pinMode(pinBuzzer, OUTPUT);
	pinMode(pinLedGreen, OUTPUT);
	pinMode(pinLedRed, OUTPUT);
	digitalWrite(pinLedGreen, HIGH);
	digitalWrite(pinLedRed, HIGH);
	Queue_can = xQueueCreate(3, sizeof(dataCan));
	QueueRfidRo = xQueueCreate(3, sizeof(Data_RFID));
	QueueRfidNV = xQueueCreate(3, sizeof(Data_RFID));
	Queue_mqtt = xQueueCreate(5, sizeof(Data_TH));
	Queue_display = xQueueCreate(3, sizeof(Data_TH));
	Queue_Time_blink = xQueueCreate(3, sizeof(uint16_t));
	xCountingSemaphore = xSemaphoreCreateCounting(2, 0);
	xSignal_FromRFID = xSemaphoreCreateCounting(2, 0);
	xSignal_Display_check = xSemaphoreCreateCounting(2, 0);
	xSignal_Display_checkdone = xSemaphoreCreateCounting(2, 0);
	xreset_id_nv = xSemaphoreCreateCounting(2, 0);
	xResetRfidMaRo = xSemaphoreCreateCounting(2, 0);
	xMutexMQTT= xSemaphoreCreateMutex();

	settingEprom();
	stateMachine.getIdControl();
	stateMachine.getPowerRFID();
	// Doc Eprom va khoi tao gia tri
	//loadWiFiConf();
	getChart((char*)&WiFiConf,sizeof(WiFiConf));
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
	if (!rtc.begin()) {
#ifdef debug_UART
		Serial.println(F("Couldn't find RTC"));
#endif
	}
	else rtc.writeSqwPinMode(DS3231_OFF);
#ifdef using_sta
	wifi_connect(0, WIFI_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd, WiFiConf.ap_ssid,false);
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
#ifdef debug_Web
	MDNS.addService("telnet", "tcp", 23);
	Debug.begin("nhan");
	Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
	Debug.showColors(true); // Colors
#endif
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
		Display,   
		"Display", 
		4096,      
		NULL,       
		12,          
		NULL,       
		0);  
	xTaskCreatePinnedToCore(
		TaskCAN,   
		"TaskCAN",
		3072,     
		NULL,      
		13,         
		NULL,       
		1);  
	xTaskCreatePinnedToCore(
		http_re,   
		"http_re", 
		4096,     
		NULL,       
		14,         
		NULL,      
		1);  
	xTaskCreatePinnedToCore(
		TaskRFID,  
		"TaskRFID", 
		3072,      
		NULL,       
		15,         
		NULL,       
		1);  
	  //MQTT
	mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(ConnectToMqtt));
	mqttClient.onConnect(onMqttConnect);
	mqttClient.onDisconnect(onMqttDisconnect);
	mqttClient.onSubscribe(onMqttSubscribe);
	mqttClient.onUnsubscribe(onMqttUnsubscribe);
	mqttClient.onMessage(onMqttMessage);
	mqttClient.onPublish(onMqttPublish);
	mqttClient.setServer(WiFiConf.mqtt_server, atoi(WiFiConf.mqtt_port));
	mqttClient.setCredentials(WiFiConf.mqtt_user, WiFiConf.mqtt_pass);
	root_CMA = SD.open("/CMA");
	xLastWakeTimeLoop = xTaskGetTickCount();
}
/*
   Main Loop luÃ´n cháº¡y Core 1
*/
void loop()
{

#ifdef debug_Web
	Debug.handle();
#endif   
	vTaskDelayUntil(&xLastWakeTimeLoop, 20);
	////////////////////////////////////////////////////////////
	//////  Reconnect Wifi        /////////////////////////////
	//////////////////////////////////////////////////////////
	if (statusPeripheral.wifi.statusConnectAP == false) {
		if ((statusPeripheral.wifi.counterWifiDisconnect == 15) && (xTaskGetTickCount() - statusPeripheral.wifi.lastTimeConnect > 500)) {
			statusPeripheral.wifi.lastTimeConnect = xTaskGetTickCount();
			WiFi.disconnect(true);
			wifi_connect(2, WIFI_AP_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd, (char*)"esp32", false);
			statusPeripheral.wifi.counterWifiDisconnect++;
#ifdef debug_Web
			DebugData("Wifi 2 che do: AP va STA");
#endif
		}
		else if ((statusPeripheral.wifi.counterWifiDisconnect < 15) && (xTaskGetTickCount() - statusPeripheral.wifi.lastTimeConnect > 1000)) {
			statusPeripheral.wifi.lastTimeConnect = xTaskGetTickCount();
			statusPeripheral.wifi.counterWifiDisconnect = statusPeripheral.wifi.counterWifiDisconnect + 1;
			wifi_connect(0, WIFI_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd, WiFiConf.ap_ssid,false);
		}
	}
	/*
	   truyen mqtt nếu không truyền thì sẽ kiểm tra thẻ nhớ đọc và gửi
	*/
	if (xQueueReceive(Queue_mqtt, &dataEncoderJsonMqtt, (TickType_t)1) == pdPASS) {
		EncoderJsonMqtt();
		statusPeripheral.sdCard.lastTimeSendMQTT = xTaskGetTickCount();
	}
	else if ((statusPeripheral.mqtt.statusMqttConnect) && ((xTaskGetTickCount() - statusPeripheral.sdCard.lastTimeSendMQTT > statusPeripheral.mqtt.timeTruyenMQTT))) {
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////  Check nếu có file còn lưu trong thu muc SD card thì đọc và gửi MQTT den server        /////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		statusPeripheral.sdCard.lastTimeSendMQTT = xTaskGetTickCount();
		if ((statusPeripheral.sdCard.statusConnect == false) && (xTaskGetTickCount() - statusPeripheral.sdCard.lastTimeReInit > 5000)) { // ReInit SD card
			statusPeripheral.sdCard.lastTimeReInit = xTaskGetTickCount();
			SdReInit(&root_CMA);
		}
		else if (statusPeripheral.sdCard.statusGetAllFile == false) {// open next file
			SdOpenNextFile(&root_CMA);
		}
		else if ((xTaskGetTickCount() - statusPeripheral.sdCard.lastTimeReadEnd > 60 * 1000 * 5) && (statusPeripheral.sdCard.statusGetAllFile)) { //30 phut moi mo lai va
			SdReOpenFolder();
		}
	}

	//////////////////////////////////////////////////////////////////
	////// lasttime yêu cau server tra data /////////////////////////
	////////////////////////////////////////////////////////////////
	if ((statusPeripheral.mqtt.statusMqttConnect) && (stateMachine.deviceStatus == deviceSetting)) {
		if ((xTaskGetTickCount() - statusPeripheral.mqtt.lastTimeGetDataConfig > 1000) || (statusPeripheral.mqtt.lastTimeGetDataConfig == 0)) {
			statusPeripheral.mqtt.lastTimeGetDataConfig = xTaskGetTickCount();
			checkSendMQTTConfig();
		}
	}
	///////////////////////////////////////////////////////////////////////
	/// 1 giay nhan timestamp 1 lan //////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	if (xTaskGetTickCount() - statusPeripheral.RTC.lastTimeGetTimeStamp > 1000) {
		statusPeripheral.RTC.lastTimeGetTimeStamp = xTaskGetTickCount();
		timeStamp = rtc.now();
	}
}


/*
Wire.setClock(400000);  Cai Clock cho RTC
mac dinh RTC chỉ 100000khz

git stash
git pull <remote name> <remote branch name> (or) switch branch
git stash apply --index

 */
