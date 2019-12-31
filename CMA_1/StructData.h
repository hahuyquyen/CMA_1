
#ifndef STRUCTDATA_H
#define STRUCTDATA_H
	#include <EEPROM.h>

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