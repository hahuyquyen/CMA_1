//#include <stdlib.h>
//#include "Arduino.h"
//#include "pins_arduino.h"
//#include "esp32-hal-uart.h"
#ifndef RFID_H
#define RFID_H
#define ALL_GOOD						0
#define ERROR_COMMAND_RESPONSE_TIMEOUT	1
#define ERROR_CORRUPT_RESPONSE			2
#define ERROR_WRONG_OPCODE_RESPONSE		3
#define RESPONSE_SUCCESS				11
#define RESPONSE_FAIL					12

#define MAX_MSG_SIZE		30
#define COMMAND_TIME_OUT	2000 
class RFID
{
public:
	RFID(void);
	void begin(Stream &serialPort = Serial2);
	uint8_t calculateCRC (uint8_t * uBuff, uint8_t uBuffLen);  
	uint8_t msg[MAX_MSG_SIZE];
	void sendMessage(uint8_t opcode, uint8_t *data = 0, uint8_t size = 0, uint16_t timeOut = COMMAND_TIME_OUT, boolean waitForResponse = true);
	void sendCommand(uint16_t timeOut = COMMAND_TIME_OUT, boolean waitForResponse = true);
	void set_mode_timming(uint8_t mode,uint16_t timeout);
	void set_timing_message(uint8_t mode,uint16_t timeout);
  	void set_out_mode(uint8_t mode,uint16_t timeout);
  	void set_time_ner(uint8_t mode,uint16_t timeout);
  	void set_OFFdelaytime(uint8_t mode,uint16_t timeout);
	void set_reset_reader(uint16_t timeout);
	void set_link_selec(uint8_t mode,uint16_t timeout);
  	void set_buzzer(uint8_t mode,uint16_t timeout);
  	void set_relay(uint8_t statu,uint16_t timeout);
  	void set_power(uint8_t mode,uint16_t timeout);
	uint8_t readData(uint8_t bank, uint32_t address, uint8_t *dataRead, uint8_t &dataLengthRead, uint16_t timeOut);
	uint8_t readTagEPC(uint8_t *epc, uint8_t &epcLength, uint16_t timeOut);
	bool IRAM_ATTR check();
	uint8_t IRAM_ATTR parseResponse(uint8_t* datareturn, uint8_t &dataLengthRead);
	
private:
	Stream *_RFIDSERIAL;
	uint8_t _head = 0;
	uint8_t _head_par = 0;
};
#endif