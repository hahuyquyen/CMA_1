


/*
   CPU cháº¡y 240mhz
   vTaskDelay (x)  x lÃ  sá»‘ stick muá»‘n trÃ¬ hoÃ£n
   Trong freertos nÃªn dÃ¹ng hÃ m delay cá»§a RTOS thay vÃ¬ cá»§a arduino
   VÃ¬ khi dÃ¹ng delay rtos thÃ¬ task sáº½ Ä‘Æ°á»£c chuyá»ƒn Ä‘á»•i trong delay


*/



const char* LCD_setting = "Cài Đặt TRƯỜNG";
void LCD_print_KV(uint8_t vitri = 48) {
  u8g2.setCursor(5, vitri); u8g2.print(giaiDoanCan.nameGiaiDoan[giaiDoanCan.userSelecGiaiDoan]);
  u8g2.setCursor(60 , vitri); u8g2.print(" - ");
  u8g2.setCursor(66 , vitri); u8g2.print(cheDoInOutDis[giaiDoanCan.cheDoInOut]);
}
void hienthiSetting(char* dataDisplay = NULL, char* dataUserDisplay = NULL) {
  if (dataDisplay != NULL ) {
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(LCD_setting))) / 2), 16);
    u8g2.print(LCD_setting);
    u8g2.setDrawColor(1);
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(dataDisplay))) / 2), 32);
    u8g2.print(dataDisplay);
  }
  if (u8g2.getUTF8Width(dataUserDisplay) > 128) {
    u8g2.drawUTF8(variLcdUpdate.numScroll, 52, dataUserDisplay);
    variLcdUpdate.numScroll = variLcdUpdate.numScroll - 35 ;
    if (abs(variLcdUpdate.numScroll) > u8g2.getUTF8Width(dataUserDisplay))variLcdUpdate.numScroll = 0;
  }
  else u8g2.drawUTF8(((128 - (u8g2.getUTF8Width(dataUserDisplay))) / 2), 52, dataUserDisplay);
}
void hienthiRunning(char* dataUserDisplay) {
  if (u8g2.getUTF8Width(dataUserDisplay) > 128) {
    u8g2.drawUTF8(variLcdUpdate.numScroll, 48, dataUserDisplay);
    variLcdUpdate.numScroll = variLcdUpdate.numScroll - 15 ;
    if (abs(variLcdUpdate.numScroll) > u8g2.getUTF8Width(dataUserDisplay))variLcdUpdate.numScroll = 0;
  }
  else u8g2.drawUTF8(((128 - (u8g2.getUTF8Width(dataUserDisplay))) / 2), 48, dataUserDisplay);
}
void LCD_thong_tin(uint8_t chedo_HT, Data_TH* Data_TH  , uint8_t daucham = 0) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_vietnamese2);
  if (chedo_HT == 0) {
    char rfidDisplay[10];
    u8g2.setCursor(0, 12);
    u8g2.print(F("NV:"));
     memcpy(rfidDisplay, & Data_TH->id_RFID_NV[16], 8);
     rfidDisplay[9]='\0';
    if (Data_TH->id_RFID_NV[0] == 'x'){ u8g2.print("x");}
    else u8g2.print(rfidDisplay);
    u8g2.setCursor(0, 28);
    u8g2.setFont(u8g2_font_unifont_t_vietnamese2);
    u8g2.print(F("Rổ:"));
    memcpy(rfidDisplay, & Data_TH->id_RFID[16], 8);
    rfidDisplay[9]='\0';
    if (Data_TH->id_RFID[0] == 'x'){u8g2.print("x");}
    else u8g2.print(rfidDisplay);
    u8g2.setFont(u8g2_font_unifont_t_vietnamese2);
    u8g2.setCursor(0, 44);
    u8g2.print(F("Kg:"));
    u8g2.print(Data_TH->data_weight);
  }
  else if (chedo_HT == 1) {
    LCD_print_KV(16);
    u8g2.setCursor(2, 32);
    u8g2.print(F("Chờ  "));
    switch (daucham) {
      case 0: u8g2.print(F("|")); break;
      case 1: u8g2.print(F("/")); break;
      case 2: u8g2.print(F("-")); break;
      case 3: u8g2.print(F("\\")); break;
      default: break;
    }
    u8g2.setCursor(68, 32);
    u8g2.print(F("Kg:"));
    u8g2.print(can_data);
    u8g2.setCursor(2, 48);
    if (giaiDoanCan.cheDoInOut == cheDoIN) {
      hienthiSetting(NULL, inforServer.nameNhaCC[inforServer.userSelectNhaCC]);
    }
    else {
      hienthiSetting(NULL, inforServer.nameThanhPham[inforServer.userSelectThanhPham]);
    }
  }
  else if (chedo_HT == 2) {
    char stringdem[] = "Chế Độ";
    hienthiSetting(stringdem, cheDoInOutDis[giaiDoanCan.cheDoInOut]);
  }
  else if (chedo_HT == 3) {
    char stringdem[] = "KHU VỰC CÂN";
    hienthiSetting(stringdem, giaiDoanCan.nameGiaiDoan[giaiDoanCan.userSelecGiaiDoan]);
  }
  else if (chedo_HT == 4) {
    char stringdem[] = "Nhà Cup Cấp";
  //  hienthiSetting(stringdem,"Cá Fillet Còn Da, Còn Mỡ, Dè 1Cm, Còn mỡ eo lưng, xử lý nhẹ");
    hienthiSetting(stringdem, inforServer.nameNhaCC[inforServer.userSelectNhaCC]);
  }
  else if (chedo_HT == 5) {
    char stringdem[] = "Loại Thành Phẩm";
    hienthiSetting(stringdem, inforServer.nameThanhPham[inforServer.userSelectThanhPham]);
  }
  else if (chedo_HT == 6) {
    char xacnhan[] = "Xác Nhận";
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(xacnhan))) / 2), 16);
    u8g2.print(xacnhan);
    LCD_print_KV(32);
    u8g2.setCursor(2, 48);
   // if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN ) || (inforServer.PhanLoaiKV == PhanLoai::LANG_IN )) {
   if (giaiDoanCan.cheDoInOut == cheDoIN) {
      hienthiSetting(NULL, inforServer.nameNhaCC[inforServer.userSelectNhaCC]);
    }
    else {
      hienthiSetting(NULL, inforServer.nameThanhPham[inforServer.userSelectThanhPham]);
    }
  }
  u8g2.sendBuffer();
}


void Display( void * pvParameters ) {
  boolean status_led = true;
  Data_TH Data_TH;
  unsigned long lastTimeBlinkLed = 0;
  unsigned long timeoutDisplay = 0;
  unsigned long timeoutLcdLangDaIn = 0;
  unsigned long lastBlinkLCD = 0;
  uint16_t Time_blink = 1000;
  uint16_t Time_check = 2500;
  // SPI.setClockDivider( SPI_CLOCK_DIV32 );
  u8g2.setBusClock(850000);
  u8g2.begin();
  u8g2.enableUTF8Print();
  LCD_thong_tin(2, &Data_TH);
  variLcdUpdate.stateDisplayLCD = 1;
  uint8_t daucham_lcd = 0;
  for (;;) {
    /*
       30ms
    */
    if (stateMachine.deviceStatus == deviceRunning) {
      if (xQueueReceive( Queue_display, &Data_TH,  ( TickType_t ) 1 ) == pdPASS ) {
        variLcdUpdate.stateDisplayLCD = 0;
      //  if ((strcmp(Data_TH.id_RFID_NV, "x") != 0) && (inforServer.PhanLoaiKV == PhanLoai::LANG_IN )) { //if (giaiDoanCan.cheDoInOut == cheDoIN) {
        if ((Data_TH.id_RFID_NV[0] == 'x') && (giaiDoanCan.cheDoInOut == cheDoIN)&& (giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan]  == kvSuaCa)) {
        //  timeoutLcdLangDaIn = xTaskGetTickCount();
        Serial.println(" Da Nhan Ro va can GD1");
        }
        else if ((giaiDoanCan.cheDoInOut == cheDoIN)&& (giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan]  == kvSuaCa)){
          Serial.println(" Da Nhan du thong tin");
          timeoutDisplay = xTaskGetTickCount();
        }
        else timeoutDisplay = xTaskGetTickCount();
      }
     /* if (xSemaphoreTake(xSignal_Display_check, 1)) {
        timeoutDisplay = xTaskGetTickCount();
      }*/
      if (xSemaphoreTake(xSignal_Display_checkdone, 1)) { //Che do IN qua timeout se tat
        variLcdUpdate.stateDisplayLCD = 1;
        digitalWrite(4, LOW);
        timeoutLcdLangDaIn = 0;
        timeoutDisplay = 0;
      }
    if ((xTaskGetTickCount() - timeoutDisplay > Time_check) && (timeoutDisplay > 0 )) {
            digitalWrite(4, LOW);
            variLcdUpdate.stateDisplayLCD = 1;
            timeoutDisplay = 0;timeoutLcdLangDaIn = 0; 
     }
      xQueueReceive( Queue_Time_blink, &Time_blink,  ( TickType_t ) 1 );
      /*
         Time blink led
      */
      if (xTaskGetTickCount() - lastTimeBlinkLed > Time_blink) {
        lastTimeBlinkLed = xTaskGetTickCount();
        status_led = !status_led;
      }
      printDebugHeap();
      switch (variLcdUpdate.stateDisplayLCD) {
        case 0:
          LCD_thong_tin(0, &Data_TH, daucham_lcd);
          variLcdUpdate.stateDisplayLCD = 4;
          break;
        case 1:
          if (xTaskGetTickCount() - lastBlinkLCD > 800) {
            daucham_lcd ++ ;
            if (daucham_lcd > 3)daucham_lcd = 0;
            lastBlinkLCD = xTaskGetTickCount();
            LCD_thong_tin(1, &Data_TH, daucham_lcd);
          }
          break;
        case 2:
          variLcdUpdate.stateDisplayLCD = 4;
          break;
        default: break;
      }
    }
    else if ((xTaskGetTickCount() - lastBlinkLCD > 1000) || variLcdUpdate.updateLCD ) {
      variLcdUpdate.updateLCD = false;
      lastBlinkLCD = xTaskGetTickCount();
      LCD_thong_tin(stateMachine.bottonSelect + 2, &Data_TH, daucham_lcd);
    }
    printDebugHeap();
    vTaskDelay(20);
  }
  vTaskDelete(NULL) ;
}
unsigned long getTimeSendHeapDebug = 0;
void printDebugHeap() {
  if (xTaskGetTickCount() - getTimeSendHeapDebug > 15000) {
    DateTime now = rtc.now();
    getTimeSendHeapDebug = xTaskGetTickCount();
    printf("Time 1/1/1970 = %lu ,StackHigh %d, Free Heap = %d\n", (unsigned long )now.unixtime(), uxTaskGetStackHighWaterMark(NULL), ESP.getFreeHeap());
  }
}
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
