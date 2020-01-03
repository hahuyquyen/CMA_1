
#ifndef STRUCTDATA_H
#define STRUCTDATA_H
	#include <EEPROM.h>

#define CONF_FORMAT {0, 0, 0, 1}
#define CONF_START 0
const uint8_t conf_format[4] = CONF_FORMAT;
/*
class epromsaveStruct :public EEPROMClass {
public:

	bool setChar(char* address, size_t chieudai) {
		for (unsigned int t = 0; t < chieudai; t++) {
			write(CONF_START + t, *(address + t));
		}
		return commit();
	};
	void getChart(char* address , size_t chieudai) {
		if (read(CONF_START + 0) == conf_format[0] &&
			read(CONF_START + 1) == conf_format[1] &&
			read(CONF_START + 2) == conf_format[2] &&
			read(CONF_START + 3) == conf_format[3])
		{
			for (unsigned int t = 0; t < chieudai; t++) {
				*(address + t) = read(CONF_START + t); //& lÃ  Ä‘á»‹a chá»‰  cá»§a biáº¿n Struc, *lÃ  data tá»©c lÃ  gÃ¡n data trong Ã´ nhá»› struc báº±ng eprom Ä‘á»�c dc (char*) lÃ  Ã©p kiá»ƒu dá»¯ liá»‡u
			}
		}
	};
protected:
private:

	const uint8_t conf_format[4] = CONF_FORMAT;
};
extern epromsaveStruct EpromClass;
*/

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
			*(address + t) = EEPROM.read(CONF_START + t); //& lÃ  Ä‘á»‹a chá»‰  cá»§a biáº¿n Struc, *lÃ  data tá»©c lÃ  gÃ¡n data trong Ã´ nhá»› struc báº±ng eprom Ä‘á»�c dc (char*) lÃ  Ã©p kiá»ƒu dá»¯ liá»‡u
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
		void getPowerRFID() { this->powerRFID = EEPROM.readInt(810);}
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
	};
	/*
	struct stateMachineSaveConf : stateMachineConf {
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
	};
	*/
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