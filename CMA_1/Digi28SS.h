#ifndef TASK_Can_H
#define TASK_Can_H
  #include "Arduino.h"
  #include "StructCan.h"
  #define canNOD1       0x40
  #define canNOD2       0x41
  #define canNOD3       0x44
  #define canNOD4       0x45
  #define canNOD5       0x48
  #define canOD1        0x42
  #define canOD2        0x43
  #define canOD3        0x47
  #define canOD4        0x46
  #define can_dau_cham  0x2C
  #define can_dau_phay  0x2E
  #define can_LF        0x0A
  #define can_CR        0x0D
  #define can_Minus     0x2D
  #define canStartByte  0x3D
  #define canStopByte   0x0D
  #define canNULLByte   0x20
  #define timeSche      1000   ///1000ms sẽ gửi data tới queue 1 lần  
  extern QueueHandle_t Queue_can;
  extern double can_data;
  static uint8_t UartCanData[11];
  //////////////////////////////////////////////////////////////////
  ////// Task so KG tu Data ////////////////////////////
  //////////////////////////////////////////////////////////////////
  boolean TachSoKg(double* soky);
  //////////////////////////////////////////////////////////////////
  ////// Task doc du lieu tu can DI28SS ////////////////////////////
  //////////////////////////////////////////////////////////////////
  void TaskCAN(void* pvParameters);
#endif
