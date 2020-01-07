#include "Digi28SS.h"

void TaskCAN( void * pvParameters ) {
  const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
  static struct dataCan dataCanTaskCan;
  int tam = 0;
  unsigned long lastTimeSendCan = 0;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  Serial1.begin(9600, SERIAL_8N1, 26, 12); //12 tx 13 lÃ  rx(bau,se,rx,tx)
  for (;;) {
    if (Serial1.available()) {
      uint8_t incomingData = Serial1.read();
      if ( incomingData == canStartByte) {
        tam = 0;
      }
      else if ( incomingData == canStopByte) {
        if (TachSoKg(&can_data)) {
          if (can_data != dataCanTaskCan.data_can) {
              dataCanTaskCan.data_can = can_data;

          if (can_data > 0.5) {
              xQueueSend( Queue_can, &dataCanTaskCan, xTicksToWait );
            }
          }
          else if (xTaskGetTickCount() - lastTimeSendCan > timeSche) {
            lastTimeSendCan = xTaskGetTickCount();
            if (can_data > 0.5) {
              xQueueSend( Queue_can, &dataCanTaskCan, xTicksToWait );
            }
          }
        }
      }
      else {
        UartCanData[tam++] = incomingData;
        if (tam > 10)tam = 0;
      }
    }
    vTaskDelayUntil(&xLastWakeTime, 25);
  }
  vTaskDelete(NULL) ;
}
//////////////////////////////////////////////////////////////////
////// Task so KG tu Data ////////////////////////////
//////////////////////////////////////////////////////////////////
boolean TachSoKg( double* soky) {
  int tam1 = 0;
  int hangtram = 0;
  double soam = 1;
  *soky = 0;
  for (int j = 0; j < sizeof(UartCanData); j++) {
    if (UartCanData[j] != canNULLByte) {
      tam1 = j;
      break;
    }
  }
  for (int j = tam1; j < sizeof(UartCanData); j++) {
    if (UartCanData[j] == can_dau_phay) {
      hangtram = (j - tam1) - 1;
      break;
    }
  }
  for (int j = tam1; j < sizeof(UartCanData); j++) {
    if ((UartCanData[j] == canNOD1) || (UartCanData[j] == canNOD2) || (UartCanData[j] == canNOD3) || (UartCanData[j] == canNOD4) || (UartCanData[j] == canNOD5)) {
      *soky = *soky * soam;
      return false;
      break;
    }
    if ((UartCanData[j] == canOD1) || (UartCanData[j] == canOD2) || (UartCanData[j] == canOD3) || (UartCanData[j] == canOD4)) {
      *soky = *soky * soam;  // so on dinh
      return true;
      break;
    }
    else if (UartCanData[j] == can_Minus) {
      soam = -1;
    }
    else if (UartCanData[j] == can_dau_phay) {
      hangtram = hangtram + 1;
    }
    else *soky += (double)((UartCanData[j] - 48) * pow(10, hangtram - (j - tam1)));
  }
  return false;
}
