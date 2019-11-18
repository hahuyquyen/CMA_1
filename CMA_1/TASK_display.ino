/*
 * CPU cháº¡y 240mhz
 * vTaskDelay (x)  x lÃ  sá»‘ stick muá»‘n trÃ¬ hoÃ£n
 * Trong freertos nÃªn dÃ¹ng hÃ m delay cá»§a RTOS thay vÃ¬ cá»§a arduino
 * VÃ¬ khi dÃ¹ng delay rtos thÃ¬ task sáº½ Ä‘Æ°á»£c chuyá»ƒn Ä‘á»•i trong delay
 * 
 * 
 */
void LCD_thong_tin(uint8_t chedo_HT,Data_TH* Data_TH , uint8_t daucham = 0){
    u8g2.clearBuffer();
  if (chedo_HT == 0){
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(0, 12);
          u8g2.print("NV:");
          u8g2.setFont(u8g2_font_6x12_tf); 
          u8g2.print(Data_TH->id_RFID_NV);
          u8g2.setCursor(0, 28);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.print("Rổ:");
          u8g2.setFont(u8g2_font_6x12_tf); 
          u8g2.print(Data_TH->id_RFID);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(0, 44);
          u8g2.print("Kg:");
          u8g2.print(Data_TH->data_weight);
          u8g2.setCursor(0, 60);
          u8g2.print("Loại: cá tra");
  }
   else if (chedo_HT == 1) {
          u8g2.setCursor(2, 16);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          switch (chonloaica.PhanLoaiKV){
            case PhanLoai::Not_Choose : u8g2.print("Chưa Chọn");break;
            case PhanLoai::Fil_IN : u8g2.print("FILLER-Đầu Vào");break;
            case PhanLoai::Fil_OUT : u8g2.print("FILLER-Đầu Ra");break;
            case PhanLoai::LANG_IN : u8g2.print("LẠNG Da-Đầu Vào");break;
            case PhanLoai::LANG_OUT : u8g2.print("LẠNG Da-Đầu Ra");break;
          }
         // if (status_IN_or_OUT) 
         // else u8g2.print("Đầu Vào");
          u8g2.setCursor(2, 32);
          u8g2.print("Chờ Thẻ");
          u8g2.setCursor(68, 32);  
          u8g2.print("Kg:"); 
        //  u8g2.setCursor(72, 32);  
          u8g2.print(can_data);     
          u8g2.setCursor(2, 48);  
          for (int i=0;i<daucham;i++){
            u8g2.print(".");
          }
          u8g2.setCursor(2, 60); 
          u8g2.print(Nha_SX.Loai_ca[chonloaica.STT_LoaiCa[chonloaica.STT_user_choose]]);
          
   }
      else if (chedo_HT == 2) {
          u8g2.setCursor(2, 16);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(2, 16);
          u8g2.print("Cài Đặt");
                    u8g2.setCursor(2, 32);
          u8g2.print("KV:");
          switch (chonloaica.PhanLoaiKV){
            case PhanLoai::Not_Choose : u8g2.print("Chưa Chọn");break;
            case PhanLoai::Fil_IN : u8g2.print("FILLER-Vào");break;
            case PhanLoai::Fil_OUT : u8g2.print("FILLER-Ra");break;
            case PhanLoai::LANG_IN : u8g2.print("LẠNG Da-Vào");break;
            case PhanLoai::LANG_OUT : u8g2.print("LẠNG Da-Ra");break;
          }
          u8g2.setCursor(2, 48);
          u8g2.print("Cá:");
          u8g2.print(Nha_SX.Loai_ca[chonloaica.STT_LoaiCa[chonloaica.STT_user_choose]]);
          u8g2.setCursor(2, 60); 
          u8g2.print("CC:");
          u8g2.print(Nha_SX.Loai_ca[chonloaica.STT_LoaiCa[chonloaica.STT_user_choose]]);       
   }
    u8g2.sendBuffer();  
}


void Display( void * pvParameters ){
    display_NV Display_NV_TASK;
    boolean status_led= true;
    Data_TH Data_TH;
    pinMode(2, OUTPUT);
    pinMode(Pin_Coi, OUTPUT);
    uint8_t mode_ht = 0;
    unsigned long _time_counting_task_display=0;
    unsigned long _time_counting_task_send_heap=0;
    unsigned long _time_out_display =0;
    unsigned long _time_out_display_LCD=0;
    unsigned long _time_blink_LCD=0;
    unsigned long _time_counting_send_heap=0;
    uint16_t Time_blink= 1000;
    uint16_t Time_check= 3000;  
 //   spi.setup(1, spi.MASTER, spi.CPOL_LOW, spi.CPHA_LOW, U8X8_PIN_NONE, 80);
    //u8g2.st7920_s_128x64(bus, cs, dc, res);
    u8g2.begin();
    u8g2.enableUTF8Print();
    LCD_thong_tin(1,&Data_TH);
    uint8_t state_LCD_Display = 1;
    uint8_t daucham_lcd = 0;
   for (;;){
      if (state_Running_conf::state_Running == state_Running_conf::Running){
                if(xSemaphoreTake(xSignal_Display_check, 10)){
                  digitalWrite(Pin_Coi,HIGH);
                  _time_out_display = xTaskGetTickCount();
                }
                if(xSemaphoreTake(xSignal_Display_checkdone, 10)){ //Che do IN qua timeout se tat 
                  state_LCD_Display = 1;
                  digitalWrite(Pin_Coi,LOW);
                }
                switch (chonloaica.PhanLoaiKV){ //chonloaica.PhanLoaiKV == PhanLoai::LANG_OUT
                      case PhanLoai::Not_Choose : if (xTaskGetTickCount() - _time_out_display > Time_check){digitalWrite(4,LOW);state_LCD_Display = 1;_time_out_display=xTaskGetTickCount();}break;
                      case PhanLoai::Fil_OUT : if (xTaskGetTickCount() - _time_out_display > Time_check){digitalWrite(4,LOW);state_LCD_Display = 1;_time_out_display=xTaskGetTickCount();}break;
                      case PhanLoai::LANG_OUT : if (xTaskGetTickCount() - _time_out_display > Time_check){digitalWrite(4,LOW);state_LCD_Display = 1;_time_out_display=xTaskGetTickCount();}break;
                      default: break;
                }
                if(xQueueReceive( Queue_display, &Data_TH,  ( TickType_t ) 2 )== pdPASS ){
                  state_LCD_Display = 0;
                  _time_out_display_LCD = xTaskGetTickCount();
                }
          
                xQueueReceive( Queue_Time_blink, &Time_blink,  ( TickType_t ) 2 );
                /*
                 * Time blink led
                 */
          
                if (xTaskGetTickCount()- _time_counting_task_display > Time_blink){
                  _time_counting_task_display = xTaskGetTickCount();
                  status_led=!status_led;
                  digitalWrite(2,status_led);
                }
                if (xTaskGetTickCount()- _time_counting_task_send_heap > 15000){
                  _time_counting_task_send_heap = xTaskGetTickCount();
                  printf("Free Heap %d\n",ESP.getFreeHeap());
          
                // printf("Free Heap %d\n",ESP.getFreeHeap());
                }
                /*
                 * time send heap free
                 */
                if (xTaskGetTickCount()- _time_counting_send_heap > 5000){
                  _time_counting_send_heap = xTaskGetTickCount();
                   printf("Free Heap %d\n",ESP.getFreeHeap());
          
                }
                
                switch (state_LCD_Display){
                  case 0:
                        
                        LCD_thong_tin(0,&Data_TH,daucham_lcd);
                        state_LCD_Display=4;
                        break;
                  case 1:
                        if (xTaskGetTickCount()- _time_blink_LCD > 1000){
                           daucham_lcd ++ ;
                          if (daucham_lcd > 8)daucham_lcd=0;
                          _time_blink_LCD = xTaskGetTickCount();
                           LCD_thong_tin(1,&Data_TH,daucham_lcd);
                        }
                        break;
                  case 2:
                        state_LCD_Display=4;
                        break;
                  default: break;
                }
      }
      else if (xTaskGetTickCount()- _time_blink_LCD > 1000){
                           daucham_lcd ++ ;
                          if (daucham_lcd > 8)daucham_lcd=0;
                          _time_blink_LCD = xTaskGetTickCount();
                           LCD_thong_tin(2,&Data_TH,daucham_lcd);
      }
      vTaskDelay(30);
    }
    vTaskDelete(NULL) ;
}

/*
 * 
 * 
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
