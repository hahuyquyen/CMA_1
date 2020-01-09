/*
   cma2018LHP515
*/

///////////////////////
//data Modbus /////////
///////////////////////
static struct modbusDataConf {
	uint16_t dataTruyen[20];
	unsigned long timeSendSSID;
	unsigned long timeSendKg;
	bool connectAP;
}modbusData = {
	0,
	0,
	0,
	false
};
//modbusData.connectAP

// struct statusPeripheralConf *poinstatusPeripheral = &statusPeripheral;

struct timeServerConf {
	unsigned long sdReadEnd = 0;
	unsigned long sdSendMQTT = 0;
	unsigned long mqttGetDataConf = 0;
	void updateTime(unsigned long* timeUpdate) {
		*timeUpdate = xTaskGetTickCount();
	}
}lastTimeServer;



//struct stateMachineConf *poinstateMachine = &stateMachine;
/////////////////////////////////////////////////////////////
///// Data trang thai LCD           //////////////////////
////////////////////////////////////////////////////////////
struct variLcdUpdateConf {
	//int numScroll;
	boolean updateLCD;
	uint8_t stateDisplayLCD;
//	boolean firstDisplay;
} variLcdUpdate = {
//  0,
  false,
  0
//  false
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
		char arrayName[15][80];
	}nhaCC;

	struct thanhPhamConf {
		uint8_t total;
		uint8_t userSelect;
		uint16_t arrayType[15];
		char arrayName[15][150];
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

//SemaphoreHandle_t xMutexRS485;
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

double can_data = 0;
