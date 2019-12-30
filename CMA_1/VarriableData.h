/*
   cma2018LHP515
*/
#include "StructCan.h"

///////////////////////
//data Modbus /////////
///////////////////////
static struct modbusDataConf {
	wchar_t nameNvUtf16[64];
	uint16_t dataTruyen[20];
	unsigned long timeSendSSID;
	unsigned long timeSendKg;
}modbusData;

static struct statusPeripheralConf {
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
}statusPeripheral;

struct timeServerConf {
	unsigned long sdReadEnd = 0;
	unsigned long sdSendMQTT = 0;
	unsigned long mqttGetDataConf = 0;
	void updateTime(unsigned long* timeUpdate) {
		*timeUpdate = xTaskGetTickCount();
	}
}lastTimeServer;


/////////////////////////////////////////////////////////////
///// Data status Button nhan           //////////////////////
////////////////////////////////////////////////////////////
static struct stateMachineConf {
	uint8_t bottonSelect;
	uint8_t deviceStatus;
	uint32_t hardwareId;
	uint8_t powerRFID;
	uint8_t giaidoanINOUT;
	uint8_t giaidoanKV;
	void getPowerRFID() { this->powerRFID = EEPROM.readInt(810); }
	void setPowerRFID() { setdata8Bit(810, &this->powerRFID); }
	void getIdControl() { this->hardwareId = EEPROM.readUInt(800); }
	void setIdControl() { setdata32Bit(800, &this->hardwareId); }
	void getKV() { this->giaidoanKV = EEPROM.readInt(812); }
	void getGiaiDoan() { this->giaidoanINOUT = EEPROM.readInt(811); }
	void setKV() { setdata8Bit(812, &this->giaidoanKV); }
	void setGiaiDoan() { setdata8Bit(811, &this->giaidoanINOUT); }
	void setdata32Bit(uint16_t address, uint32_t* value) {
		EEPROM.writeUInt(address, *value);
		EEPROM.commit();
	}
	void setdata8Bit(uint16_t address, uint8_t* value) {
		EEPROM.writeInt(address, *value);
		EEPROM.commit();
	}
} stateMachine;
/////////////////////////////////////////////////////////////
///// Data trang thai LCD           //////////////////////
////////////////////////////////////////////////////////////
struct variLcdUpdateConf {
	int numScroll;
	boolean updateLCD;
	uint8_t stateDisplayLCD;
	boolean firstDisplay;
} variLcdUpdate = {
  0,
  false,
  0
};
/////////////////////////////////////////////////////////////
///// Data luu du lieu nhan tu server //////////////////////
////////////////////////////////////////////////////////////
static struct inforServerStruct {
	char nameCheDoInOut[3][30] = { "Chưa Chọn", "Đầu Vào", "Đầu Ra" };

	struct nhaCCConf {
		uint8_t total = 0;
		uint8_t userSelect;
		uint16_t arrayType[15];
		char arrayName[15][120];
	}nhaCC;

	struct thanhPhamConf {
		uint8_t total;
		uint8_t userSelect;
		uint16_t arrayType[15];
		char arrayName[15][120];
	}thanhPham;
	/*
	struct giaiDoanConf{
		uint8_t cheDoInOut;
		uint8_t total;
		uint8_t userSelect;
		uint8_t arrayType[10];
		char arrayName[10][30];
	}giaiDoan;
	*/
	struct mqttConfigConf {
		char topicGetStatusACK[25];
		char topicGetConfig[25];
		char topicSenData[6] = "/data";
		char topicSenConfig[8] = "/config";
		void setTopicACK(unsigned long id_device) {
			sprintf(this->topicGetStatusACK, "/data/ack/%lu", id_device);
		}
		void setTopicGetConfig(unsigned long id_device) {
			sprintf(this->topicGetConfig, "/config/%lu", id_device);
		}

	}mqttConfig;
	void changeData(boolean chedo, uint8_t* userSelect, uint8_t totaldata = 0) {
		if (chedo) { *userSelect = (*userSelect > (totaldata - 1)) ? 0 : (*userSelect + 1); }
		else *userSelect = *userSelect - 1;
	}
	//char ssssssss[6533];inforServer.mqttConfig.topicSenData
} inforServer;

/////////////////////////////////////////////////////////////
///// Data Queue  FreeRTOS          //////////////////////
////////////////////////////////////////////////////////////

SemaphoreHandle_t xMutexRS485;
SemaphoreHandle_t xMutexMQTT;
SemaphoreHandle_t xCountingSemaphore;
SemaphoreHandle_t xSignal_FromRFID;
SemaphoreHandle_t xSignal_Display_check;
SemaphoreHandle_t xSignal_Display_checkdone;
SemaphoreHandle_t xreset_id_nv;
SemaphoreHandle_t xResetRfidMaRo;
QueueHandle_t Queue_can;
QueueHandle_t QueueRfidRo;
QueueHandle_t QueueRfidNV;
QueueHandle_t Queue_mqtt;
QueueHandle_t Queue_display;
QueueHandle_t Queue_Time_blink;

/////////////////////////////////////////////////////////////
///// Data RFID           //////////////////////
////////////////////////////////////////////////////////////
struct Data_RFID {
	char id_RFID[25];
	char id_RFID_Old[25];
};

/////////////////////////////////////////////////////////////
///// Data Tong Hop Task Checkdata           //////////////////////
////////////////////////////////////////////////////////////
typedef struct Data_TH {
	char id_RFID[25];
	char id_RFID_NV[25];
	double data_weight;

} Data_TH;