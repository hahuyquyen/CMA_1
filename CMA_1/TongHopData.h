#ifndef TongHopData_H
#define TongHopData_H
#include "Arduino.h"
#include "StructCan.h"
#include "StructRFID.h"
#include "StructData.h"
#include "Config.h"
#include <RemoteDebug.h>

extern RemoteDebug Debug;
extern QueueHandle_t Queue_can;
extern QueueHandle_t QueueRfidRo;
extern QueueHandle_t QueueRfidNV;
extern QueueHandle_t Queue_display;
extern QueueHandle_t Queue_mqtt;
extern SemaphoreHandle_t xSignal_Display_checkdone;
extern SemaphoreHandle_t xResetRfidMaRo;
extern SemaphoreHandle_t xreset_id_nv;
//extern debugD();
extern uint8_t GetSttKhuVuc();
void http_re(void* pvParameters);
#endif