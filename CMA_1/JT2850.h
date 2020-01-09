#include "Arduino.h"
#include "rfid.h"
#include "StructRFID.h"
#include "StructData.h"
#include "Config.h"

#define MaRo_RFID		0xE2
#define MaNV_RFID		0x00
#define MaLoaiCa_RFID	0x01
#define rfidTimeOut		1000


extern QueueHandle_t QueueRfidNV;
extern QueueHandle_t QueueRfidRo;
extern SemaphoreHandle_t xreset_id_nv;
extern SemaphoreHandle_t xResetRfidMaRo;
extern uint8_t GetSttKhuVuc();
void IRAM_ATTR array_to_string(byte* array, unsigned int len, char* buffer);
//////////////////////////////////////////////////////////////////
////// Task doc du lieu tu RFID  ////////////////////////////
//////////////////////////////////////////////////////////////////
void TaskRFID(void* pvParameters);
