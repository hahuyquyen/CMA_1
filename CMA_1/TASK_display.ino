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
          u8g2.setFont(u8g2_font_5x7_tf); // u8g2_font_6x10_tf
          u8g2.setCursor(0, 10);
          if (status_IN_or_OUT) u8g2.print("Dau Ra");
          else u8g2.print("Dau vao");
          u8g2.print("-");
          if (status_wifi_connect_AP) u8g2.print(WiFi.localIP().toString().c_str());
          else u8g2.print("Not connect");
          u8g2.setFont(u8g2_font_6x12_tf); 
          u8g2.drawStr(0,25,"NV:");
          u8g2.setCursor(25, 25);
          u8g2.print(Data_TH->id_RFID_NV);
          u8g2.drawStr(0,40,"Ro:");
          u8g2.setCursor(25, 40);
          u8g2.print(Data_TH->id_RFID);
          u8g2.drawStr(0,55,"Kg:");
          u8g2.setCursor(25, 55);
          u8g2.print(Data_TH->data_weight);
  }
   else if (chedo_HT == 1) {
          u8g2.setFont(u8g2_font_5x7_tf); // u8g2_font_6x10_tf
          u8g2.setCursor(0, 10);
          if (status_IN_or_OUT) u8g2.print("Dau Ra");
          else u8g2.print("Dau vao");
          u8g2.print("-");
          if (status_wifi_connect_AP) u8g2.print(WiFi.localIP().toString().c_str());
          else u8g2.print("Not connect");
          u8g2.setFont(u8g2_font_6x12_tf); 
          u8g2.drawStr(35,25,"Waiting");
          u8g2.setCursor(35, 45);
          for (int i=0;i<daucham;i++){
            u8g2.print(".");
          }
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
    uint16_t Time_blink= 1000;
    uint16_t Time_check= 3000;  
    u8g2.begin();
    LCD_thong_tin(1,&Data_TH);
    uint8_t state_LCD_Display = 1;
    uint8_t daucham_lcd = 0;
    while(true){
      if(xSemaphoreTake(xSignal_Display_check, 10)){
        digitalWrite(Pin_Coi,HIGH);
        _time_out_display = xTaskGetTickCount();
      }
      if(xSemaphoreTake(xSignal_Display_checkdone, 10)){ //Che do IN qua timeout se tat 
        
        state_LCD_Display = 1;
        digitalWrite(Pin_Coi,LOW);
      }
    if (status_IN_or_OUT){  // Neu che do OUT thi tu tat sau 3s
        if (xTaskGetTickCount() - _time_out_display > Time_check){digitalWrite(4,LOW);state_LCD_Display = 0;_time_out_display=xTaskGetTickCount();}
    }
      if(xQueueReceive( Queue_display, &Data_TH,  ( TickType_t ) 2 )== pdPASS ){
        state_LCD_Display = 0;
        _time_out_display_LCD = xTaskGetTickCount();
      }

     xQueueReceive( Queue_Time_blink, &Time_blink,  ( TickType_t ) 2 );
      if (xTaskGetTickCount()- _time_counting_task_display > Time_blink){
        _time_counting_task_display = xTaskGetTickCount();
        status_led=!status_led;
        digitalWrite(2,status_led);
      }
      if (xTaskGetTickCount()- _time_counting_task_send_heap > 15000){
        _time_counting_task_send_heap = xTaskGetTickCount();
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
                if (daucham_lcd > 5)daucham_lcd=0;
                _time_blink_LCD = xTaskGetTickCount();
                 LCD_thong_tin(1,&Data_TH,daucham_lcd);
              }
              break;
        case 2:
              state_LCD_Display=4;
              break;
        default: break;
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

