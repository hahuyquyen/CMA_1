ModbusRTU mb;
  

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}
void LcdDisplayKhuVuc(uint8_t vitri = 48) {
}
void LcdDisplayTurnOffPower() {
}
void LcdDisplaySetting(char* dataDisplay = NULL, char* dataUserDisplay = NULL) {
}
void LcdDisplayInfor(){
}

void LcdSeclectMode(uint8_t chedo_HT, Data_TH* Data_TH  , uint8_t daucham = 0) {
  if (chedo_HT == 0) {
      
   /*   mb.writeHreg(1,(uint16_t) 7910, (uint16_t)*Data_TH->data_weight, 1, cb); //ID, Offsrt, Data,SL
      mb.writeHreg(1,(uint16_t) 7911, (uint16_t)(*Data_TH->data_weight >> 16), 1, cb); //ID, Offsrt, Data,SL
      wmemset(modbusData.nameNvUtf16, 0x0000, sizeof(modbusData.nameNvUtf16) / 2);
      size_t valuelen = utf8towchar(Data_TH->id_RFID_NV, SIZE_MAX, modbusData.nameNvUtf16, sizeof(modbusData.nameNvUtf16));
      mb.writeHreg(1,(uint16_t) 8150, (uint16_t)modbusData.nameNvUtf16, sizeof(modbusData.nameNvUtf16) / 2 , cb);
      mb.writeHreg(1,(uint16_t) 7900, 16, 1, cb); //ID, Offsrt, Data,SL cai dat id man hinh
      yield();*/
  }
  else if (chedo_HT == 1) {
     /*     mb.writeHreg(1, (uint16_t)7910, (uint16_t)*Data_TH->data_weight, 1, cb); //ID, Offsrt, Data,SL
      mb.writeHreg(1, (uint16_t)7911, (uint16_t)(*Data_TH->data_weight >> 16), 1, cb); //ID, Offsrt, Data,SL
      wmemset(modbusData.nameNvUtf16, 0x0000, sizeof(modbusData.nameNvUtf16) / 2);
      size_t valuelen = utf8towchar(Data_TH->id_RFID_NV, SIZE_MAX, modbusData.nameNvUtf16, sizeof(modbusData.nameNvUtf16));
      mb.writeHreg(1, (uint16_t)8150, (uint16_t*)modbusData.nameNvUtf16, sizeof(modbusData.nameNvUtf16) / 2 , cb);
      mb.writeHreg(1, (uint16_t)7900, 15, 1, cb); //ID, Offsrt, Data,SL cai dat id man hinh
      yield();*/
  }
  else if (chedo_HT == 2) {
  }
  else if (chedo_HT == 10) {
  }
  else if (chedo_HT == 3) {
  }
  else if (chedo_HT == 4) { // nha cung cap
       /*   mb.writeHreg(1, (uint16_t)7910, (uint16_t)Data_TH->data_weight, 1, cb); //ID, Offsrt, Data,SL
      mb.writeHreg(1, (uint16_t)7911, (uint16_t)(Data_TH->data_weight >> 16), 1, cb); //ID, Offsrt, Data,SL
      wmemset(modbusData.nameNvUtf16, 0x0000, sizeof(modbusData.nameNvUtf16) / 2);
      size_t valuelen = utf8towchar(inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], SIZE_MAX, modbusData.nameNvUtf16, 32);
      mb.writeHreg(1,(uint16_t) 8013, modbusData.nameNvUtf16, 32 , cb);
      mb.writeHreg(1, (uint16_t)7900, 12, 1, cb); //ID, Offsrt, Data,SL cai dat id man hinh
      yield();*/
  }
  else if (chedo_HT == 5) { //loai thanh pham
    /*      mb.writeHreg(1,(uint16_t) 7910, (uint16_t)Data_TH->data_weight, 1, cb); //ID, Offsrt, Data,SL
      mb.writeHreg(1,(uint16_t) 7911, (uint16_t)(Data_TH->data_weight >> 16), 1, cb); //ID, Offsrt, Data,SL
      wmemset(modbusData.nameNvUtf16, 0x0000, sizeof(modbusData.nameNvUtf16) / 2);
      size_t valuelen = utf8towchar(inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], SIZE_MAX, modbusData.nameNvUtf16, 64);
      mb.writeHreg(1,(uint16_t) 8045, modbusData.nameNvUtf16, 64, cb);
      mb.writeHreg(1,(uint16_t) 7900, 13, 1, cb); //ID, Offsrt, Data,SL cai dat id man hinh
      yield();*/
  }
  else if (chedo_HT == 6) { //xac nhan
     // mb.writeHreg(1,(uint16_t) 7910, (Data_TH->data_weight), 1, cb); //ID, Offsrt, Data,SL
     // mb.writeHreg(1,(uint16_t) 7911, (Data_TH->data_weight) >> 16, 1, cb); //ID, Offsrt, Data,SL
  /*    wmemset(modbusData.nameNvUtf16, 0x0000, sizeof(modbusData.nameNvUtf16) / 2);
      size_t valuelen = utf8towchar(inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], SIZE_MAX, modbusData.nameNvUtf16, 64);
      mb.writeHreg(1, (uint16_t)8045,(uint16_t*) modbusData.nameNvUtf16, 64, cb);
      wmemset(modbusData.nameNvUtf16, 0x0000, sizeof(modbusData.nameNvUtf16) / 2);
      valuelen = utf8towchar(inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], SIZE_MAX, modbusData.nameNvUtf16, 32);
      mb.writeHreg(1,(uint16_t) 8013, (uint16_t*)modbusData.nameNvUtf16, 32 , cb);
      mb.writeHreg(1,(uint16_t) 7900, (uint16_t*) 14, 1, cb); //ID, Offsrt, Data,SL cai dat id man hinh
      yield();*/
  }
}


void Display( void * pvParameters ) {
  boolean status_led = true;
  Data_TH Data_TH;
  unsigned long lastTimeBlinkLed = 0;
  unsigned long timeoutDisplay = 0;
  //  unsigned long timeoutLcdLangDaIn = 0;
  unsigned long lastBlinkLCD = 0;
  uint16_t Time_blink = 1000;
  uint16_t Time_check = 2500;
  Serial1.begin(115200, SERIAL_8N1, 26, 25); //12 tx 13 lÃ  rx(bau,se,rx,tx)
  mb.begin(&Serial1);
  mb.master();
  LcdSeclectMode(2, &Data_TH);
  variLcdUpdate.stateDisplayLCD = 1;
  uint8_t daucham_lcd = 0;
  boolean statusBuzzer = false ;
  digitalWrite(pinLedGreen, HIGH);
  digitalWrite(pinLedRed, HIGH);
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
 // mb.writeHreg(1, (uint16_t)8013, (uint16_t*)modbusData.nameNvUtf16, 32, cb);
 // mb.writeHreg(1, (uint16_t)7900, (uint16_t*)10, 1, cb); //ID man hinh

  uint16_t mahinh = 10;
  for (;;) {
    //Serial.println("qưeqwwwwwwww");
    if (!mb.slave()) {
      Serial.println("Modbus");
      mb.writeHreg(1, &(uint16_t)7900, (uint16_t)11, 1, cb); //ID man hinh
   //     mb.writeHreg(1, (uint16_t)7901, (uint16_t*)stateMachine.idDevice, 1, cb); //ID man hinh
 // mb.writeHreg(1, (uint16_t)7902, (uint16_t) stateMachine.idDevice >>16, 1, cb); //ID man hinh
 // mb.writeHreg(1, (uint16_t)7903, (uint16_t*)stateMachine.giaidoanINOUT, 1, cb); //ID man hinh
 // mb.writeHreg(1, (uint16_t)7904, (uint16_t*)stateMachine.giaidoanKV, 1, cb); //ID man hinh
      
    }
    mb.task();
    switch (stateMachine.deviceStatus) {
      //////////////////////////////////////////////////////////////////
      case deviceRunning: //////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////
        if (xQueueReceive( Queue_display, &Data_TH,  ( TickType_t ) 1 ) == pdPASS ) {
          variLcdUpdate.stateDisplayLCD = 0;
          if ((Data_TH.id_RFID_NV[0] == 'x') && (GetSttKhuVuc() ==  sttKvSuaCaIN)) {
#ifdef debug_UART
            Serial.println("Data : Ro & Can KV Sua Ca - IN");
#endif
#ifdef debug_Web
            DebugData("Data : Ro & Can KV Sua Ca - IN"); 
#endif
            digitalWrite(pinLedGreen, LOW);
          }
          else if (GetSttKhuVuc() ==  sttKvSuaCaIN) {
#ifdef debug_UART   
            Serial.println(" Da Nhan du thong tin");
#endif
#ifdef debug_Web
              DebugData("Da Nhan du thong tin");
#endif
            timeoutDisplay = xTaskGetTickCount();
            statusBuzzer = true ;
            digitalWrite(pinBuzzer, statusBuzzer);
          }
          else {
            timeoutDisplay = xTaskGetTickCount();
            statusBuzzer = true ;
            digitalWrite(pinBuzzer, statusBuzzer);
          }
        }
        // Turn Off Buzzer, LED
        if (xSemaphoreTake(xSignal_Display_checkdone, 1)) { //Che do IN qua timeout se tat
          variLcdUpdate.stateDisplayLCD = 1;
          statusBuzzer = false ;
          digitalWrite(pinBuzzer, statusBuzzer);
          digitalWrite(pinLedGreen, HIGH);
          timeoutDisplay = 0;
        }
        if ((xTaskGetTickCount() - timeoutDisplay > 500) && (timeoutDisplay > 0 ) && (statusBuzzer)) {
          statusBuzzer = false ;
          digitalWrite(pinBuzzer, statusBuzzer);
          digitalWrite(pinLedGreen, HIGH);
        }
        if ((xTaskGetTickCount() - timeoutDisplay > Time_check) && (timeoutDisplay > 0 )) {
          variLcdUpdate.stateDisplayLCD = 1;
          timeoutDisplay = 0;
        }
        xQueueReceive( Queue_Time_blink, &Time_blink,  ( TickType_t ) 1 );
        if (xTaskGetTickCount() - lastTimeBlinkLed > Time_blink) {
          lastTimeBlinkLed = xTaskGetTickCount();
          status_led = !status_led;
        }
        printDebugHeap();
        switch (variLcdUpdate.stateDisplayLCD) {
          case 0:
            LcdSeclectMode(0, &Data_TH, daucham_lcd);
            variLcdUpdate.stateDisplayLCD = 4;
            break;
          case 1:
            if (xTaskGetTickCount() - lastBlinkLCD > 150) {
            //  daucham_lcd ++ ;
             // if (daucham_lcd > 3)daucham_lcd = 0;
              lastBlinkLCD = xTaskGetTickCount();
              LcdSeclectMode(1, &Data_TH, daucham_lcd);
            }
            break;
          case 2:
            LcdSeclectMode(10, &Data_TH, daucham_lcd);
            variLcdUpdate.stateDisplayLCD = 5;
            break;
          default: break;
        }
        break;
      //////////////////////////////////////////////////////////////////
      case deviceSetting: ///////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////
          switch (variLcdUpdate.stateDisplayLCD) {
            case 1:
                if ((xTaskGetTickCount() - lastBlinkLCD > 100) || variLcdUpdate.updateLCD ) {
                  variLcdUpdate.updateLCD = false;
                  lastBlinkLCD = xTaskGetTickCount();
                  LcdSeclectMode(stateMachine.bottonSelect + 2, &Data_TH, daucham_lcd);
                }
              break;
            case 2:
              LcdSeclectMode(10, &Data_TH, daucham_lcd);
              variLcdUpdate.stateDisplayLCD = 5;
              break;
            default: break;
          }
        break;
      //////////////////////////////////////////////////////////////////
      case deviceTurnOff: /////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////
        if ((xTaskGetTickCount() - lastBlinkLCD > 5000)|| variLcdUpdate.updateLCD ) {
                  LcdDisplayTurnOffPower();
                  lastBlinkLCD = xTaskGetTickCount();
        }
        break;
      default : 
        break;
    }
    printDebugHeap();
    vTaskDelayUntil(&xLastWakeTime, 50);
  }
  vTaskDelete(NULL) ;
}

unsigned long getTimeSendHeapDebug = 0;
void printDebugHeap() {
}


/*

const char LCD_setting[] = "Cài Đặt";
void LcdDisplayKhuVuc(uint8_t vitri = 48) {
  u8g2.setCursor(5, vitri);
  u8g2.print(inforServer.giaiDoan.arrayName[inforServer.giaiDoan.userSelect]);
  u8g2.setCursor(60 , vitri);
  u8g2.print(" - ");
  u8g2.setCursor(66 , vitri);
  u8g2.print(inforServer.nameCheDoInOut[inforServer.giaiDoan.cheDoInOut]);
}
void LcdDisplayTurnOffPower() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_vietnamese2);
  u8g2.setCursor(((128 - (u8g2.getUTF8Width("Tắt Thiết Bị"))) / 2), 32);
  u8g2.print("Tắt Thiết Bị");
  u8g2.sendBuffer();
}
void LcdDisplaySetting(char* dataDisplay = NULL, char* dataUserDisplay = NULL) {
  if (dataDisplay != NULL ) {
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(LCD_setting))) / 2), 16);
    u8g2.print(LCD_setting);
    u8g2.setDrawColor(1);
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(dataDisplay))) / 2), 32);
    u8g2.print(dataDisplay);
  }
  if (u8g2.getUTF8Width(dataUserDisplay) > 128) {
    u8g2.drawUTF8(variLcdUpdate.numScroll, 52, dataUserDisplay);
    variLcdUpdate.numScroll = variLcdUpdate.numScroll - 1 ;
    if (abs(variLcdUpdate.numScroll) > u8g2.getUTF8Width(dataUserDisplay))variLcdUpdate.numScroll = 0;
  }
  else u8g2.drawUTF8(((128 - (u8g2.getUTF8Width(dataUserDisplay))) / 2), 52, dataUserDisplay);
}
void LcdDisplayInfor(){
    u8g2.setCursor(((128 - (u8g2.getUTF8Width("Thông Tin"))) / 2), 16);
    u8g2.print("Thông Tin");
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(WiFi.localIP().toString().c_str()))) / 2), 32);
    u8g2.print(WiFi.localIP().toString().c_str());
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(WiFiConf.sta_ssid))) / 2), 48);
    u8g2.print(WiFiConf.sta_ssid);
}

void LcdSeclectMode(uint8_t chedo_HT, Data_TH* Data_TH  , uint8_t daucham = 0) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_vietnamese2);
  char textTam[ 40 ];
  if (chedo_HT == 0) {
    uint8_t vitri = 0;
    memcpy(textTam, & Data_TH->id_RFID_NV[16], 8);
    textTam[9] = '\0';
    if ((Data_TH->id_RFID_NV[0] == 'x') && ( GetSttKhuVuc() ==  sttKvSuaCaIN )) {
      vitri = 18;
      u8g2.setCursor(10, vitri);
      u8g2.print(F("NV: x"));
    }
    else if (Data_TH->id_RFID_NV[0] != 'x') {
      vitri = 18;
      u8g2.setCursor(10, vitri);
      u8g2.print(F("NV: "));
      u8g2.print(textTam);
    }
    memcpy(textTam, & Data_TH->id_RFID[16], 8);
    textTam[9] = '\0';
    if (Data_TH->id_RFID[0] != 'x') {
      if (vitri == 18)vitri = 36;
      else vitri = 18;
      u8g2.setCursor(10, vitri);
      u8g2.print(F("Rổ: "));
      u8g2.print(textTam);
    }
    if (vitri == 18) {
      vitri = 36;
    }
    else if (vitri == 36) {
      vitri = 54;
    }
    u8g2.setCursor(10, vitri);
    u8g2.print(F("Kg: "));
    u8g2.print(Data_TH->data_weight);

  }
  else if (chedo_HT == 1) {
    LcdDisplayKhuVuc(16);
    sprintf(textTam, "Kg: %.3lf", can_data);
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(textTam))) / 2), 32);
    u8g2.print(textTam);
    u8g2.setCursor(2, 48);
    if (inforServer.giaiDoan.cheDoInOut == cheDoIN) {
      LcdDisplaySetting(NULL, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect]);
    }
    else {
      LcdDisplaySetting(NULL, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect]);
    }
  }
  else if (chedo_HT == 2) {
 //   char stringdem[] = ;
    memcpy(textTam, "Chế Độ\0", sizeof(textTam));
    LcdDisplaySetting(textTam, inforServer.nameCheDoInOut[inforServer.giaiDoan.cheDoInOut]);
  }
  else if (chedo_HT == 10) {
    LcdDisplayInfor();
  }
  else if (chedo_HT == 3) {
    memcpy(textTam, "Khu Vực Cân\0", sizeof(textTam));
    //char stringdem[] = "Khu Vực Cân";
    LcdDisplaySetting(textTam, inforServer.giaiDoan.arrayName[inforServer.giaiDoan.userSelect]);
  }
  else if (chedo_HT == 4) {
    memcpy(textTam, "Nhà Cung Cấp\0", sizeof(textTam));
    //char stringdem[] = "Nhà Cung Cấp";
    LcdDisplaySetting(textTam, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect]);
  }
  else if (chedo_HT == 5) {
      memcpy(textTam, "Loại Thành Phẩm\0", sizeof(textTam));
  //  char stringdem[] = "Loại Thành Phẩm";
    LcdDisplaySetting(textTam, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect]);
  }
  else if (chedo_HT == 6) {
   // char xacnhan[] = "Xác Nhận";
    memcpy(textTam, "Xác Nhận\0", sizeof(textTam));
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(textTam))) / 2), 16);
    u8g2.print(textTam);
    LcdDisplayKhuVuc(32);
    u8g2.setCursor(2, 48);
    if (inforServer.giaiDoan.cheDoInOut == cheDoIN) {
      LcdDisplaySetting(NULL, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect]);
    }
    else {
      LcdDisplaySetting(NULL, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect]);
    }
  }
  u8g2.setFont(u8g2_font_5x8_tr);
  sprintf(textTam, "%ddB SD:%s P:%d%%\n", statusPeripheral.rssiWifi, (statusPeripheral.sdCard.statusConnect == true) ? "ok" : "fail", statusPeripheral.powerValue);
  u8g2.setCursor(((128 - (u8g2.getUTF8Width(textTam))) / 2), 64);
  u8g2.print(textTam);
  u8g2.sendBuffer();
}


void Display( void * pvParameters ) {
  boolean status_led = true;
  Data_TH Data_TH;
  unsigned long lastTimeBlinkLed = 0;
  unsigned long timeoutDisplay = 0;
  //  unsigned long timeoutLcdLangDaIn = 0;
  unsigned long lastBlinkLCD = 0;
  uint16_t Time_blink = 1000;
  uint16_t Time_check = 2500;
  // SPI.setClockDivider( SPI_CLOCK_DIV32 );
 // u8g2.setBusClock(700000);
 // u8g2.begin();
//  u8g2.enableUTF8Print();
  LcdSeclectMode(2, &Data_TH);
  variLcdUpdate.stateDisplayLCD = 1;
  uint8_t daucham_lcd = 0;
 // u8g2.setAutoPageClear(1);
  boolean statusBuzzer = false ;
  digitalWrite(pinLedGreen, HIGH);
  digitalWrite(pinLedRed, HIGH);
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    switch (stateMachine.deviceStatus) {
      //////////////////////////////////////////////////////////////////
      case deviceRunning: //////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////
        if (xQueueReceive( Queue_display, &Data_TH,  ( TickType_t ) 1 ) == pdPASS ) {
          variLcdUpdate.stateDisplayLCD = 0;
          if ((Data_TH.id_RFID_NV[0] == 'x') && (GetSttKhuVuc() ==  sttKvSuaCaIN)) {
#ifdef debug_UART
            Serial.println("Data : Ro & Can KV Sua Ca - IN");
#endif
#ifdef debug_Web
            DebugData("Data : Ro & Can KV Sua Ca - IN"); 
#endif
            digitalWrite(pinLedGreen, LOW);
          }
          else if (GetSttKhuVuc() ==  sttKvSuaCaIN) {
#ifdef debug_UART   
            Serial.println(" Da Nhan du thong tin");
#endif
#ifdef debug_Web
              DebugData("Da Nhan du thong tin");
#endif
            timeoutDisplay = xTaskGetTickCount();
            statusBuzzer = true ;
            digitalWrite(pinBuzzer, statusBuzzer);
          }
          else {
            timeoutDisplay = xTaskGetTickCount();
            statusBuzzer = true ;
            digitalWrite(pinBuzzer, statusBuzzer);
          }
        }
        // Turn Off Buzzer, LED
        if (xSemaphoreTake(xSignal_Display_checkdone, 1)) { //Che do IN qua timeout se tat
          variLcdUpdate.stateDisplayLCD = 1;
          statusBuzzer = false ;
          digitalWrite(pinBuzzer, statusBuzzer);
          digitalWrite(pinLedGreen, HIGH);
          timeoutDisplay = 0;
        }
        if ((xTaskGetTickCount() - timeoutDisplay > 500) && (timeoutDisplay > 0 ) && (statusBuzzer)) {
          statusBuzzer = false ;
          digitalWrite(pinBuzzer, statusBuzzer);
          digitalWrite(pinLedGreen, HIGH);
        }
        if ((xTaskGetTickCount() - timeoutDisplay > Time_check) && (timeoutDisplay > 0 )) {
          variLcdUpdate.stateDisplayLCD = 1;
          timeoutDisplay = 0;
        }
        xQueueReceive( Queue_Time_blink, &Time_blink,  ( TickType_t ) 1 );
        if (xTaskGetTickCount() - lastTimeBlinkLed > Time_blink) {
          lastTimeBlinkLed = xTaskGetTickCount();
          status_led = !status_led;
        }
        printDebugHeap();
        switch (variLcdUpdate.stateDisplayLCD) {
          case 0:
            LcdSeclectMode(0, &Data_TH, daucham_lcd);
            variLcdUpdate.stateDisplayLCD = 4;
            break;
          case 1:
            if (xTaskGetTickCount() - lastBlinkLCD > 150) {
            //  daucham_lcd ++ ;
             // if (daucham_lcd > 3)daucham_lcd = 0;
              lastBlinkLCD = xTaskGetTickCount();
              LcdSeclectMode(1, &Data_TH, daucham_lcd);
            }
            break;
          case 2:
            LcdSeclectMode(10, &Data_TH, daucham_lcd);
            variLcdUpdate.stateDisplayLCD = 5;
            break;
          default: break;
        }
        break;
      //////////////////////////////////////////////////////////////////
      case deviceSetting: ///////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////
          switch (variLcdUpdate.stateDisplayLCD) {
            case 1:
                if ((xTaskGetTickCount() - lastBlinkLCD > 100) || variLcdUpdate.updateLCD ) {
                  variLcdUpdate.updateLCD = false;
                  lastBlinkLCD = xTaskGetTickCount();
                  LcdSeclectMode(stateMachine.bottonSelect + 2, &Data_TH, daucham_lcd);
                }
              break;
            case 2:
              LcdSeclectMode(10, &Data_TH, daucham_lcd);
              variLcdUpdate.stateDisplayLCD = 5;
              break;
            default: break;
          }
        break;
      //////////////////////////////////////////////////////////////////
      case deviceTurnOff: /////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////
        if ((xTaskGetTickCount() - lastBlinkLCD > 5000)|| variLcdUpdate.updateLCD ) {
                  LcdDisplayTurnOffPower();
                  lastBlinkLCD = xTaskGetTickCount();
        }
        break;
      default : 
        break;
    }
    printDebugHeap();
    vTaskDelayUntil(&xLastWakeTime, 50);
  }
  vTaskDelete(NULL) ;
}

unsigned long getTimeSendHeapDebug = 0;
void printDebugHeap() {
  if (xTaskGetTickCount() - getTimeSendHeapDebug > 15000) {
    getTimeSendHeapDebug = xTaskGetTickCount();

#ifdef debug_UART
    Serial.print (statusPeripheral.rssiWifi);
    Serial.print (" - " );
    Serial.print (GetSttKhuVuc());
    Serial.print (" - " );
    
    //printf("%lu , %d, Heap %d\n", (unsigned long )timeStamp.unixtime(), uxTaskGetStackHighWaterMark(NULL), ESP.getFreeHeap());
#endif
#ifdef debug_Web
    DebugData("%lu , %d, Heap %d", (unsigned long)timeStamp.unixtime(), uxTaskGetStackHighWaterMark(NULL), ESP.getFreeHeap());
#endif
  }
}
*/
/*


  short int 2 -32,768 to 32,767 %hd
  unsigned short int  2 0 to 65,535 %hu
  unsigned int  4 0 to 4,294,967,295  %u
  int 4 -2,147,483,648 to 2,147,483,647 %d
  long int  4 -2,147,483,648 to 2,147,483,647 %ld
  unsigned long int 4 0 to 4,294,967,295  %lu
  long long int 8 -(2^63) to (2^63)-1 %lld
  unsigned long long int  8 0 to 18,446,744,073,709,551,615 %llu
  signed char 1 -128 to 127 %c
  unsigned char 1 0 to 255  %c
  float 4   %f
  double  8   %lf
  long double 12    %Lf
  We can use the sizeof() operator to check the size of a variable. See the following C program for
*/



/*
  Chinh cgho phu hop ESP32 FreeRTOS
  uint8_t u8x8_gpio_and_delay_espidf(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
  {
   uint8_t i;
   switch(msg)
   {
   case U8X8_MSG_GPIO_AND_DELAY_INIT:
       for( i = 0; i < U8X8_PIN_CNT; i++ )
           if ( u8x8->pins[i] != U8X8_PIN_NONE )
           {
           bool success;
               if ( i < U8X8_PIN_OUTPUT_CNT )
               {
         success = u8x8_setpinoutput_espidf(u8x8->pins[i]);
               }
               else
               {
         success = u8x8_setpininput_espidf(u8x8->pins[i]);
               }
               if (!success) return 0;
           }

       break;

   case U8X8_MSG_DELAY_NANO:
       ets_delay_us(arg_int==0?0:1);
       break;

   case U8X8_MSG_DELAY_10MICRO:
       break;

   case U8X8_MSG_DELAY_100NANO:
       break;

   case U8X8_MSG_DELAY_MILLI:
       vTaskDelay(arg_int);
       break;
   case U8X8_MSG_DELAY_I2C:
       ets_delay_us(arg_int<=2?5:2);
       break;
   case U8X8_MSG_GPIO_I2C_CLOCK:
   case U8X8_MSG_GPIO_I2C_DATA:
   auto pin = u8x8_GetPinValue(u8x8, msg);
       if ( arg_int == 0 )
       {
           if (
           (!u8x8_setpinoutput_espidf(pin)) ||
           (gpio_set_level(pin, 0) != ESP_OK)
           ) return 0;
       }
       else
       {
           if (!u8x8_setpininput_espidf(pin)) return 0;
       }
       break;
   default:
       if ( msg >= U8X8_MSG_GPIO(0) )
       {
           i = u8x8_GetPinValue(u8x8, msg);
           if ( i != U8X8_PIN_NONE )
           {
               if ( u8x8_GetPinIndex(u8x8, msg) < U8X8_PIN_OUTPUT_CNT )
               {
                   gpio_set_level(i, arg_int);
               }
               else
               {
                   u8x8_SetGPIOResult(u8x8, gpio_get_level(i));
               }
           }
           break;
       }

       return 0;
   }
   return 1;
  }
*/
