

/*
   CPU cháº¡y 240mhz
   vTaskDelay (x)  x lÃ  sá»‘ stick muá»‘n trÃ¬ hoÃ£n
   Trong freertos nÃªn dÃ¹ng hÃ m delay cá»§a RTOS thay vÃ¬ cá»§a arduino
   VÃ¬ khi dÃ¹ng delay rtos thÃ¬ task sáº½ Ä‘Æ°á»£c chuyá»ƒn Ä‘á»•i trong delay


*/



const char* LCD_setting = "Cài Đặt";



void LCD_print_KV(uint8_t vitri = 48) {
  char stringdem1[] = "Fille-Đầu Vào";
  char stringdem2[] = "Fille-Đầu Ra";
  char stringdem3[] = "Sữa Cá -Đầu Vào";
  char stringdem4[] = "Sữa Cá-Đầu Ra";
  switch (inforServer.PhanLoaiKV) {
    case PhanLoai::Not_Choose : u8g2.setCursor(((128 - (u8g2.getUTF8Width(ramChuaChon))) / 2), vitri); u8g2.print(ramChuaChon); break;
    case PhanLoai::Fil_IN : u8g2.setCursor(((128 - (u8g2.getUTF8Width(stringdem1))) / 2), vitri); u8g2.print(stringdem1); break;
    case PhanLoai::Fil_OUT : u8g2.setCursor(((128 - (u8g2.getUTF8Width(stringdem2))) / 2), vitri); u8g2.print(stringdem2); break;
    case PhanLoai::LANG_IN : u8g2.setCursor(((128 - (u8g2.getUTF8Width(stringdem3))) / 2), vitri); u8g2.print(stringdem3); break;
    case PhanLoai::LANG_OUT : u8g2.setCursor(((128 - (u8g2.getUTF8Width(stringdem4))) / 2), vitri); u8g2.print(stringdem4); break;
  }
}
void hienthiSetting(char* dataDisplay = NULL, char* dataUserDisplay = NULL) {
  if (dataDisplay != NULL ) {
    u8g2.setDrawColor(0);
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(LCD_setting))) / 2), 16);
    u8g2.print(LCD_setting);
    u8g2.setDrawColor(1);
    u8g2.setCursor(((128 - (u8g2.getUTF8Width(dataDisplay))) / 2), 32);
    u8g2.print(dataDisplay);
  }
  if (u8g2.getUTF8Width(dataUserDisplay) > 128) {
    u8g2.drawUTF8(variLcdUpdate.numScroll, 48, dataUserDisplay);
    variLcdUpdate.numScroll = variLcdUpdate.numScroll - 15 ;
    if (abs(variLcdUpdate.numScroll) > u8g2.getUTF8Width(dataUserDisplay))variLcdUpdate.numScroll = 0;
  }
  else u8g2.drawUTF8(((128 - (u8g2.getUTF8Width(dataUserDisplay))) / 2), 48, dataUserDisplay);
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
  u8g2.sendF("c", 0x01 );
 // u8g2.clear();
  u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
  if (chedo_HT == 0) {

    u8g2.setCursor(0, 12);
    u8g2.print(F("NV:"));
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.print(Data_TH->id_RFID_NV[10]);
    u8g2.setCursor(0, 28);
    u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
    u8g2.print(F("Rổ:"));
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.print(Data_TH->id_RFID[10]);
    u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
    u8g2.setCursor(0, 44);
    u8g2.print(F("Kg:"));
    u8g2.print(Data_TH->data_weight);
  }
  else if (chedo_HT == 1) {
    LCD_print_KV(16);
    u8g2.setCursor(2, 32);
    u8g2.print(F("Chờ NV "));
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
    if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN ) || (inforServer.PhanLoaiKV == PhanLoai::LANG_IN )) {
      hienthiSetting(NULL, inforServer.nameNhaCC[inforServer.userSelectNhaCC]);
    }
    else {
      hienthiSetting(NULL, inforServer.nameThanhPham[inforServer.userSelectThanhPham]);
    }
  }
  else if (chedo_HT == 2) {
    char stringdem[] = "KHU VỰC CÂN";
    hienthiSetting(stringdem, " ");
    LCD_print_KV();
  }
  /*  else if (chedo_HT == 3) {
           char stringdem[] = "Loại Cá";
           hienthiSetting(stringdem,inforServer.nameLoaiCa[inforServer.userSelectLoaiCa]);
    }*/
  else if (chedo_HT == 4) {
    char stringdem[] = "Nhà Cup Cấp";
    hienthiSetting(stringdem," ca tao lao bi dao va ca da ci giet mot cach da man");
    //hienthiSetting(stringdem, inforServer.nameNhaCC[inforServer.userSelectNhaCC]);
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
    if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN ) || (inforServer.PhanLoaiKV == PhanLoai::LANG_IN )) {
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
  unsigned long getTimeBlinkLed = 0;
  unsigned long timeoutDisplay = 0;
  unsigned long _time_blink_LCD = 0;
  uint16_t Time_blink = 1000;
  uint16_t Time_check = 1500;
  // SPI.setClockDivider( SPI_CLOCK_DIV32 );
  u8g2.setBusClock(600000);
  u8g2.begin();
  u8g2.enableUTF8Print();

  LCD_thong_tin(2, &Data_TH);
  variLcdUpdate.stateDisplayLCD = 1;
  uint8_t daucham_lcd = 0;

  unsigned long timeoutLcdLangDaIn = 0;
  for (;;) {
    /*
       30ms
    */
    if (state_Running_conf::state_Running == state_Running_conf::Running) {
      if (xQueueReceive( Queue_display, &Data_TH,  ( TickType_t ) 1 ) == pdPASS ) {
        variLcdUpdate.stateDisplayLCD = 0;
        if ((strcmp(Data_TH.id_RFID_NV, "x") != 0) && (inforServer.PhanLoaiKV == PhanLoai::LANG_IN )) {
          timeoutLcdLangDaIn = xTaskGetTickCount();
        }
      }
      if (xSemaphoreTake(xSignal_Display_check, 1)) {
        timeoutDisplay = xTaskGetTickCount();
      }
      if (xSemaphoreTake(xSignal_Display_checkdone, 1)) { //Che do IN qua timeout se tat
        variLcdUpdate.stateDisplayLCD = 1;

      }
      switch (inforServer.PhanLoaiKV) { //inforServer.PhanLoaiKV == PhanLoai::LANG_OUT
        case PhanLoai::Not_Choose : if ((xTaskGetTickCount() - timeoutDisplay > Time_check) && (timeoutDisplay > 0 )) {
            digitalWrite(4, LOW);
            variLcdUpdate.stateDisplayLCD = 1;
            timeoutDisplay = 0;
          } break;
        case PhanLoai::Fil_OUT : if ((xTaskGetTickCount() - timeoutDisplay > Time_check) && (timeoutDisplay > 0 )) {
            digitalWrite(4, LOW);
            variLcdUpdate.stateDisplayLCD = 1;
            timeoutDisplay = 0;
          } break;
        case PhanLoai::Fil_IN : if ((xTaskGetTickCount() - timeoutDisplay > Time_check) && (timeoutDisplay > 0 )) {
            digitalWrite(4, LOW);
            variLcdUpdate.stateDisplayLCD = 1;
            timeoutDisplay = 0;
          } break;
        case PhanLoai::LANG_OUT : if ((xTaskGetTickCount() - timeoutDisplay > Time_check) && (timeoutDisplay > 0 )) {
            digitalWrite(4, LOW);
            variLcdUpdate.stateDisplayLCD = 1;
            timeoutDisplay = 0;
          } break;
        case PhanLoai::LANG_IN : if ((xTaskGetTickCount() - timeoutLcdLangDaIn > Time_check) && (timeoutLcdLangDaIn > 0 )) {
            digitalWrite(4, LOW);
            variLcdUpdate.stateDisplayLCD = 1;
            timeoutLcdLangDaIn = 0;
          } break;
        default: break;
      }

      xQueueReceive( Queue_Time_blink, &Time_blink,  ( TickType_t ) 1 );
      /*
         Time blink led
      */
      if (xTaskGetTickCount() - getTimeBlinkLed > Time_blink) {
        getTimeBlinkLed = xTaskGetTickCount();
        status_led = !status_led;
      }
      printDebugHeap();
      switch (variLcdUpdate.stateDisplayLCD) {
        case 0:

          LCD_thong_tin(0, &Data_TH, daucham_lcd);
          variLcdUpdate.stateDisplayLCD = 4;
          break;
        case 1:
          if (xTaskGetTickCount() - _time_blink_LCD > 800) {
            daucham_lcd ++ ;
            if (daucham_lcd > 3)daucham_lcd = 0;
            _time_blink_LCD = xTaskGetTickCount();
            LCD_thong_tin(1, &Data_TH, daucham_lcd);
          }
          break;
        case 2:
          variLcdUpdate.stateDisplayLCD = 4;
          break;
        default: break;
      }
    }
    else if ((xTaskGetTickCount() - _time_blink_LCD > 1000) || variLcdUpdate.updateLCD ) {
      variLcdUpdate.updateLCD = false;
      _time_blink_LCD = xTaskGetTickCount();
      LCD_thong_tin(Status_setting.state_select + 2, &Data_TH, daucham_lcd);
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
