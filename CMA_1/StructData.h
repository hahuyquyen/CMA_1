
#ifndef STRUCTDATA_H
#define STRUCTDATA_H
//#define NO_GLOBAL_EEPROM
#include <EEPROM.h>

#define CONF_FORMAT {0, 0, 0, 1}
#define CONF_START 0
#define addressEpromPowerRFID 810
#define addressEpromIDDevice 800
#define addressEpromKV 812
#define addressEpromGiaiDoan 811

const uint8_t conf_format[4] = CONF_FORMAT;

static bool settingEprom() {
	EEPROM.begin(1024);
 return false;
};

static bool setChar(char* address, size_t chieudai) {
	for (unsigned int t = 0; t < chieudai; t++) {
		EEPROM.write(CONF_START + t, *(address + t));
	}
	return EEPROM.commit();
};
static void getChart(char* address, size_t chieudai) {
	if (EEPROM.read(CONF_START + 0) == conf_format[0] &&
		  EEPROM.read(CONF_START + 1) == conf_format[1] &&
		  EEPROM.read(CONF_START + 2) == conf_format[2] &&
		  EEPROM.read(CONF_START + 3) == conf_format[3])
	{
		for (unsigned int t = 0; t < chieudai; t++) {
			*(address + t) = EEPROM.read(CONF_START + t); 
		}
	}
};
/////////////////////////////////////////////////////////////
///// Data status Button nhan           //////////////////////
////////////////////////////////////////////////////////////
struct stateMachineConf {
		uint8_t bottonSelect;
		uint8_t deviceStatus;
		uint32_t hardwareId;
		uint8_t powerRFID;
		uint8_t giaidoanINOUT;
		uint8_t giaidoanKV;
		void getPowerRFID() { this->powerRFID = EEPROM.readInt(addressEpromPowerRFID);}
		void setPowerRFID() { setdata8Bit(addressEpromPowerRFID, &this->powerRFID); }
		void getIdControl() { this->hardwareId = EEPROM.readUInt(addressEpromIDDevice); }
		void setIdControl() { setdata32Bit(addressEpromIDDevice, &this->hardwareId); }
		void getKV() { this->giaidoanKV = EEPROM.readInt(addressEpromKV); }
		void getGiaiDoan() { this->giaidoanINOUT = EEPROM.readInt(addressEpromGiaiDoan); }
		void setKV() { setdata8Bit(addressEpromKV, &this->giaidoanKV); }
		void setGiaiDoan() { setdata8Bit(addressEpromGiaiDoan, &this->giaidoanINOUT); }
		void setdata32Bit(uint16_t address, uint32_t* value) {
			EEPROM.writeUInt(address, *value);
			EEPROM.commit();
		}
		void setdata8Bit(uint16_t address, uint8_t* value) {
			EEPROM.writeInt(address, *value);
			EEPROM.commit();
			
		}
};
struct statusPeripheralConf {
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
};

extern struct stateMachineConf stateMachine;
extern struct statusPeripheralConf statusPeripheral;

#endif
