/*#include "Arduino.h"
#include "rfid.h"
#include "StructRFID.h"
#include "Config.h"





extern QueueHandle_t QueueRfidNV;
extern QueueHandle_t QueueRfidRo;
extern SemaphoreHandle_t xreset_id_nv;
extern SemaphoreHandle_t xResetRfidMaRo;
extern uint8_t GetSttKhuVuc();
extern struct statusPeripheralConf {
	struct mqttConf {
		unsigned long lastTimeGetDataConfig;
		unsigned long  timeTruyenMQTT;
		boolean statusMqttConnect;
		boolean updateName;
		char nameNhanVien[64];
	}mqtt = { 0,1000,false,false,"" };
	struct wifiConf {
		boolean ApConnect;
		unsigned long lastTimeConnect;
		uint8_t counterWifiDisconnect;
		boolean statusConnectAP;
	}wifi = { false,0,0,true };
	struct rtcConf {
		boolean statusConnect;
		unsigned long  lastTimeGetTimeStamp;
	}RTC = { false,0 };
	struct sdCardConf {
		unsigned long lastTimeReadEnd;
		unsigned long lastTimeReInit;
		unsigned long lastTimeSendMQTT;
		boolean statusConnect;
		boolean statusGetAllFile;
	}sdCard = { 0,0,false,false };
	int rssiWifi;
	int powerValue;
	uint32_t timeStampServer = 0;
	boolean updateRFID = false;
} statusPeripheral;
extern struct stateMachineConf *poinstateMachine;
void IRAM_ATTR array_to_string(byte* array, unsigned int len, char* buffer);
//////////////////////////////////////////////////////////////////
////// Task doc du lieu tu RFID  ////////////////////////////
//////////////////////////////////////////////////////////////////
void TaskRFID(void* pvParameters);*/
