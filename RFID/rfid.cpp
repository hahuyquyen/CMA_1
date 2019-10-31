#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "rfid.h"

RFID::RFID(void)
{
  // Constructor
}
bool RFID::begin(Stream &serialPort)
{
  _RFIDSERIAL = &serialPort; //Grab which port the user wants us to use

}

uint8_t RFID::calculateCRC (uint8_t * uBuff, uint8_t uBuffLen)  
{  
  uint8_t i, uSum = 0;  
  for (i = 0; i <uBuffLen; i++)  
  {  
    uSum = uSum + uBuff [i];  
  }  
  uSum = (~ uSum) + 1;  
  return uSum;  
} 
void RFID::set_mode_timming (uint8_t mode)  
{ 
	uint8_t data[4]={0x00,0x00,0x70,0x02};
	if (mode ==1 )data[3]=0x01;
	else if(mode ==2 )data[3]=0x02;
	else data[3]=0x03;
	sendMessage(0x60,data,sizeof(data),1000,true);
} 
void set_timing_message(uint8_t timing){
	uint8_t data[4]={0x00,0x00,0x71,0x02};
	data[3]=timing;
	sendMessage(0x60,data,sizeof(data),1000,true);
}
void set_reset)reader(){
	uint8_t data[]={0x00};
	sendMessage(0x65,data,sizeof(data),1000,true);
}
/*Mode 0x01 0x02 0x03
*/
void set_link_selec(uint8_t mode){
	uint8_t data[4]={0x00,0x00,0x72,0x02};
	data[3]=mode;
	sendMessage(0x60,data,sizeof(data),1000,true);
}
void RFID::sendMessage(uint8_t opcode, uint8_t *data, uint8_t size, uint16_t timeOut, boolean waitForResponse)
{
  msg[1] = size+2; //Load the length of this operation into msg array
  msg[2] = opcode;

  //Copy the data into msg array
  for (uint8_t x = 0; x < size; x++)
    msg[3 + x] = data[x];

  sendCommand(timeOut, waitForResponse); //Send and wait for response
}

void RFID::sendCommand(uint16_t timeOut, boolean waitForResponse)
{
  msg[0] = 0xA0; //Universal header
  uint8_t messageLength = msg[1];

  uint8_t opcode = msg[2]; //Used to see if response from module has the same opcode

  //Attach CRC
  uint8_t crc = calculateCRC(&msg[0], messageLength+1); //Calc CRC starting from spot 1, not 0. Add 2 for LEN and OPCODE bytes.
  msg[messageLength + 1] = crc;
 // Doc het du lieu con sot lai
  while (_nanoSerial->available()) _RFIDSERIAL->read();

  //gui du lieu moi
  for (uint8_t x = 0; x < messageLength + 2; x++)
    _RFIDSERIAL->write(msg[x]);

  if (waitForResponse == false)
  {
    _RFIDSERIAL->flush(); //Wait for serial sending to complete
    return;
  }

  uint32_t startTime = xTaskGetTickCount();
  while (_RFIDSERIAL->available() == false)
  {
    if (xTaskGetTickCount() - startTime > timeOut)
    {
      msg[0] = ERROR_COMMAND_RESPONSE_TIMEOUT;
      return;
    }
    vTaskDelay(10);
  }

  // Layout of response in data array:
  // [0] [1] [2] [3]      [4]      [5] [6]  ... [LEN+4] [LEN+5] [LEN+6]
  // FF  LEN OP  STATUSHI STATUSLO xx  xx   ... xx      CRCHI   CRCLO
  messageLength = MAX_MSG_SIZE - 1; 
  uint8_t spot = 0;
  while (spot < messageLength)
  {
    if (xTaskGetTickCount() - startTime > timeOut)
    {
      msg[0] = ERROR_COMMAND_RESPONSE_TIMEOUT;
      return;
    }

    if (_RFIDSERIAL->available())
    {
      msg[spot] = _RFIDSERIAL->read();
      if (spot == 1)messageLength = msg[1] + 2; 
    }
  }
  crc = calculateCRC(&msg[0], messageLength - 2); 
  if ((msg[messageLength - 1] != crc )
  {
    msg[0] = ERROR_CORRUPT_RESPONSE;
    return;
  }
  if (msg[2] != opcode)
  {
    msg[0] = ERROR_WRONG_OPCODE_RESPONSE;
    return;
  }
  msg[0]=ALL_GOOD;
}
/*

  byte myEPC[12]; //Most EPCs are 12 bytes
  byte myEPClength;
  myEPClength = sizeof(myEPC); 
    responseType = nano.readTagEPC(myEPC, myEPClength, 500);
*/
uint8_t RFID::readTagEPC(uint8_t *epc, uint8_t &epcLength, uint16_t timeOut)
{
  uint8_t bank = 0x01;    //User data bank
  uint8_t address = 0x02; //Starts at 2

  return (readData(bank, address, epc, epcLength, timeOut));
}

uint8_t RFID::readData(uint8_t bank, uint32_t address, uint8_t *dataRead, uint8_t &dataLengthRead, uint16_t timeOut)
{
	uint8_t data[4]={0x00,0x01,0x02,0x01};
	data[2]=address;
	data[1]=bank;
	sendMessage(0x80, data, sizeof(data), timeOut);
  if (msg[0] == ALL_GOOD) //We received a good response
  {
      uint8_t responseLength = msg[1];

      if (responseLength < dataLengthRead) //User wants us to read more than we have available
        dataLengthRead = responseLength;
      for (uint8_t x = 0; x < dataLengthRead; x++)
        dataRead[x] = msg[5 + x];
      return (RESPONSE_SUCCESS);
  }
  dataLengthRead = 0; //Inform caller that we weren't able to read anything

  return (RESPONSE_FAIL);
}


bool RFID::check()
{
  while (_RFIDSERIAL->available())
  {
    uint8_t incomingData = _RFIDSERIAL->read();
	if (incomingData == 0x00 && _head == 0){
		msg[_head] = incomingData;
		_head++;
	}
	else if ( _head > 0 && incomingData != 0xFF){
		msg[_head++] = incomingData;
	}
	else{
		for (uint8_t x = _head; x < MAX_MSG_SIZE; x++) msg[x] = 0;
	    _head=0;
		return (true);
	}
    }
  

  return (false);
}
uint8_t RFID::parseResponse(void)
{
	
}