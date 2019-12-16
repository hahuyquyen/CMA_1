#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "rfid.h"
/*
 * '\0'
 * change memset(a,0x00,x) to memset(a,'\0',x)
 */
RFID::RFID(void)
{
  // Constructor
}
void RFID::begin(Stream &serialPort)
{
  _RFIDSERIAL = &serialPort;
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
void RFID::set_mode_timming (uint8_t mode,uint16_t timeout)  
{ 
	uint8_t data[4]={0x00,0x00,0x70,0x02};
	if (mode ==1 )data[3]=0x01;
	else if(mode ==2 )data[3]=0x02;
	else data[3]=0x03;
	sendMessage(0x60,data,sizeof(data),timeout,true);
} 
void RFID::set_OFFdelaytime (uint8_t mode,uint16_t timeout)  {
  uint8_t data[4]={0x00,0x00,0x84,0x02};
  data[3]=mode;
  sendMessage(0x60,data,sizeof(data),timeout,true);
}
void RFID::set_time_ner(uint8_t mode,uint16_t timeout)  {
  uint8_t data[4]={0x00,0x00,0x7A,0x02};
  data[3]=mode;
  sendMessage(0x60,data,sizeof(data),timeout,true);
}
void RFID::set_out_mode (uint8_t mode,uint16_t timeout)  
{ 
  uint8_t data[4]={0x00,0x00,0x7B,0x02};
  if (mode ==1 )data[3]=0x01;
  else data[3]=0x02;
  sendMessage(0x60,data,sizeof(data),timeout,true);
} 
void RFID::set_buzzer(uint8_t mode,uint16_t timeout)
{ 
  uint8_t data[2]={0x00,0x00};
  if (mode ==0 )data[1]=0x00;
  else data[1]=0x01;
  sendMessage(0xB0,data,sizeof(data),timeout,true);
} 
void RFID::set_relay(uint8_t statu,uint16_t timeout)
{ 
  uint8_t data[2]={0x00,0x00};
  if (statu ==0 )data[1]=0x00;
  else data[1]=0x01;
  sendMessage(0xB1,data,sizeof(data),timeout,true);
} 
void RFID::set_timing_message(uint8_t timing,uint16_t timeout){
	uint8_t data[4]={0x00,0x00,0x71,0x64};
	data[3]=timing;
	sendMessage(0x60,data,sizeof(data),timeout,true);
}
void RFID::set_reset_reader(uint16_t timeout){
	uint8_t data[]={0x00};
	sendMessage(0x65,data,sizeof(data),timeout,true);
}
/*Mode 0x01 0x02 0x03
*/
void RFID::set_link_selec(uint8_t mode,uint16_t timeout){
	uint8_t data[4]={0x00,0x00,0x72,0x02};
	data[3]=mode;
	sendMessage(0x60,data,sizeof(data),timeout,true);
}
/*
 * power tá»« 0 - 150 Transmit power 0 - 150 Power analog
 */
void RFID::set_power(uint8_t mode,uint16_t timeout){
  uint8_t data[4]={0x00,0x00,0x65,0x02}; 
  data[3]=mode;
  sendMessage(0x60,data,sizeof(data),timeout,true);
}
void RFID::sendMessage(uint8_t opcode, uint8_t *data, uint8_t size, uint16_t timeOut, boolean waitForResponse)
{
  msg[1] = size+2; //Load the length of this operation into msg array
  msg[2] = opcode;
  for (uint8_t x = 0; x < size; x++) msg[3 + x] = data[x];

  sendCommand(timeOut, waitForResponse); //Send and wait for response
}

void RFID::sendCommand(uint16_t timeOut, boolean waitForResponse)
{
  msg[0] = 0xA0; //Universal header
  uint8_t messageLength = msg[1];
  uint8_t opcode = msg[2]; 
  uint8_t crc = calculateCRC(&msg[0], messageLength+1); 
  msg[messageLength + 1] = crc;
  if (messageLength > 29) messageLength = 29; //tràn bộ nhớ
  while ( _RFIDSERIAL->available()) _RFIDSERIAL->read();
  for (uint8_t x = 0; x < messageLength + 2; x++) { 
      _RFIDSERIAL->write(msg[x]);
       // Serial.print(msg[x],HEX);
      //  Serial.print("-");
  }
 // Serial.println("");
  if (waitForResponse == false)
  { _RFIDSERIAL->flush(); 
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
    vTaskDelay(2);
  }
  messageLength = MAX_MSG_SIZE - 1; 
  uint8_t spot = 0;
  while (spot < messageLength)
  {
    if (xTaskGetTickCount() - startTime > timeOut)
    { 
      msg[0] = ERROR_COMMAND_RESPONSE_TIMEOUT;
      return;
    }
    if ( _RFIDSERIAL->available())
    {
      msg[spot] = _RFIDSERIAL->read();
      //    Serial.print(msg[spot],HEX);
   // Serial.print("-");
      spot++;
      if (spot == 2) messageLength = msg[1] + 2; 
      else if (spot == MAX_MSG_SIZE) {return;} // Chống tràn bộ nhớ gây reset
    }
    vTaskDelay(1);
  }
  crc = calculateCRC(&msg[0], messageLength-1); 
  if (msg[messageLength - 1] != crc )
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


bool IRAM_ATTR RFID::check()
{
  if ( _RFIDSERIAL->available())
  { 
    uint8_t incomingData = _RFIDSERIAL->read();
    //Serial.print(incomingData,HEX);
    //Serial.print("-");
  	if (incomingData == 0x00 && _head == 0){
  		msg[_head++] = incomingData;
  	}
  	else if ( _head > 0 && incomingData != 0xFF){
      
  		msg[_head++] = incomingData;
      if( _head == 29) _head = 0; // Chống tràn bộ nhớ gây reset
  	}
  	else if (incomingData == 0xFF){
  		msg[_head]=0xFF;
  		for (uint8_t x = _head; x < MAX_MSG_SIZE; x++) msg[x] = 0x00;
  		_head_par = _head;
  	  _head=0;
  		return (true);
  	}
    if ( _head > 25){_head =0;memset(msg,'\0',sizeof(msg));} //
  }
  return (false);
}
uint8_t IRAM_ATTR RFID::parseResponse(uint8_t* datareturn, uint8_t &dataLengthRead)
{/* 
14 byte(0-13)
00 00 08 E2 00 10 71 00 00 52 6F 01 D3 FF
08 label lenghth
byte 3-10:   E2 00 10 71 00 00 52 6F ID
01 Antena  D3 Checksum  FF logo

17 byte
00 00 E3 00 60 19 D2 6D 1C E9 AA BB CC DD 01 51 FF
byte2-13 E3 00 60 19 D2 6D 1C E9 AA BB CC DD lÃ  ID
01 lÃ  anten

20 byte
00 00 12 1F 15 05 8D 48 29 4E D9 00 D9 00 00 00 00 05 01 8A FF

12  datalenght
1F RSSI
15 nhiá»‡t Ä‘á»™ giÃ¡ trá»‹, giÃ¡ trá»‹ lÃ  hex chuyá»ƒn qua dec
05  nhiá»‡t Ä‘á»™ label
byte 6-17 8D 48 29 4E D9 00 D9 00 00 00 00 05 ID
*/
	uint8_t crc = calculateCRC(&msg[0], _head_par-1);
	uint8_t tam = 0;
	if (dataLengthRead < 8) return false;
	else if (dataLengthRead < 12) tam=8;
	else tam=12;
  //Serial.printf("\n so phan tu %d \n", _head_par);
	if (crc == msg[ _head_par-1 ]){
    memset(datareturn ,'\0', dataLengthRead);
		if ( _head_par == 14){
			for (uint8_t x = 0; x < 8; x++)datareturn[x]=msg[x+3];
		}
		/*else if ( _head_par == 17){
			for (uint8_t x = 0; x < tam; x++)
				datareturn[x]=msg[x+3];
		}*/
   else if ( _head_par == 17){
     for (uint8_t x = 0; x < tam; x++)
        datareturn[x]=msg[x+3];
    }
		else if ( _head_par == 20){
			for (uint8_t x = 0; x < tam; x++)
				datareturn[x]=msg[x+6];
		}
   _head_par = 0;
		return true;
	}
	return false;
}

/*
 * E0 06 60 00 71 02
 * A0 05 61 00 00 65 95
 * 00 00 E3 00 60 19 D2 6D 1C E9 AA BB CC DD 01 51 FF
 */
