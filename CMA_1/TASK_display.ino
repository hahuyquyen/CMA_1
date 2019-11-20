/*
 * CPU cháº¡y 240mhz
 * vTaskDelay (x)  x lÃ  sá»‘ stick muá»‘n trÃ¬ hoÃ£n
 * Trong freertos nÃªn dÃ¹ng hÃ m delay cá»§a RTOS thay vÃ¬ cá»§a arduino
 * VÃ¬ khi dÃ¹ng delay rtos thÃ¬ task sáº½ Ä‘Æ°á»£c chuyá»ƒn Ä‘á»•i trong delay
 * 
 * 
 */
 #include <EasyButton.h>
#define button_left_pin 32
#define button_right_pin 34
#define button_ok_pin 35
#define button_error_pin 36
#define button_du_phong_pin 39
 boolean update_lcd_setting = false;
 int Scrolling_lcd = 0;
EasyButton button_left(button_left_pin,200,true);
EasyButton button_right(button_right_pin,200,true);
EasyButton button_ok(button_ok_pin,200,true);
EasyButton button_error(button_error_pin,200,true);
EasyButton button_du_phong(button_du_phong_pin,1000,true);
void onPressed_left() {
  if (Status_setting.state_select == 0){
    if (chonloaica.PhanLoaiKV > 0){
    chonloaica.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((chonloaica.PhanLoaiKV - 1) % (PhanLoai::LANG_OUT+1));
    }
  }
  else  if (Status_setting.state_select == 1){
    if (chonloaica.STT_user_choose > 0){
    chonloaica.STT_user_choose -- ;
    if (chonloaica.STT_user_choose > chonloaica.SL_LoaiCa) {chonloaica.STT_user_choose = 0;}
    }
  }
   else if (Status_setting.state_select == 2){
    
  }
   else if (Status_setting.state_select == 3){
    
  }
  Serial.println("onPressed_left");
  Scrolling_lcd = 0;
 update_lcd_setting = true;
}
void onPressed_right() {
  if (Status_setting.state_select == 0){
    chonloaica.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((chonloaica.PhanLoaiKV + 1) % (PhanLoai::LANG_OUT+1));
  }
  else  if (Status_setting.state_select == 1){
    chonloaica.STT_user_choose ++ ;
    if (chonloaica.STT_user_choose > chonloaica.SL_LoaiCa) {chonloaica.STT_user_choose = 0;}
  }
   else if (Status_setting.state_select == 2){
    chonloaica.STT_user_choose_NhaCC ++ ;
    if (chonloaica.STT_user_choose_NhaCC > chonloaica.SL_NhaCC) {chonloaica.STT_user_choose_NhaCC = 0;}
  }
   else if (Status_setting.state_select == 3){
    chonloaica.STT_user_choose_ThanhPham ++ ;
    if (chonloaica.STT_user_choose_ThanhPham > chonloaica.SL_ThanhPham) {chonloaica.STT_user_choose_ThanhPham = 0;}
  }
  Scrolling_lcd = 0;
  update_lcd_setting = true;
}
void onPressed_ok() {
Serial.println("onPressed_OK");
  if ((Status_setting.state_select == 0)&& (chonloaica.PhanLoaiKV == 0)) return;
  if ((Status_setting.state_select == 1)&& (chonloaica.STT_user_choose == 0)) return;
  if ((Status_setting.state_select == 2)&& (chonloaica.STT_user_choose_NhaCC == 0)) return;
  if ((Status_setting.state_select == 3)&& (chonloaica.STT_user_choose_ThanhPham == 0)) return;
  if  ((Status_setting.state_select == 0)&& ((chonloaica.PhanLoaiKV == 2 )||(chonloaica.PhanLoaiKV == 4 ))) {
    /*
     * đẩu ra nên bỏ qua chọn 
     */
     Status_setting.state_select = 3;
  }
  else{
  Status_setting.state_select = Status_setting.state_select + 1;
  if (Status_setting.state_select == 3 ){Status_setting.state_select =4;}
  }
  if (Status_setting.state_select > 3){state_Running_conf::state_Running = state_Running_conf::Running;}
  Scrolling_lcd = 0;
  update_lcd_setting = true;
}
void onPressed_error() {
  Status_setting.state_select = 0 ;
  state_Running_conf::state_Running = state_Running_conf::Setting;
Serial.println("onPressed_vitri");
}
void onPressed() {
Serial.println("onPressed");
}

void LCD_thong_tin(uint8_t chedo_HT,Data_TH* Data_TH  , uint8_t daucham = 0){
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
          switch (daucham){
            case 0:u8g2.print('|');break;
            case 1:u8g2.print('/');break;
            case 2:u8g2.print('-');break;
            case 3:u8g2.print('\\');break;
            default: break;
          }
          u8g2.setCursor(68, 32);  
          u8g2.print("Kg:"); 
        //  u8g2.setCursor(72, 32);  
          u8g2.print(can_data);   
           if ((chonloaica.PhanLoaiKV == PhanLoai::Fil_IN ) || (chonloaica.PhanLoaiKV == PhanLoai::LANG_IN )){
             u8g2.setCursor(2, 48); 
             u8g2.print(Nha_SX.So_Lo[chonloaica.STT_user_choose_NhaCC]);  
             u8g2.setCursor(2, 60);
             u8g2.print(Nha_SX.Loai_ca[chonloaica.STT_user_choose]);  
           }
           else {
            u8g2.setCursor(2, 48); 
             u8g2.print(Nha_SX.Thanh_Pham[chonloaica.STT_user_choose_ThanhPham]);      
           }
          
          
          
   }
      else if (chedo_HT == 2) {
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(2, 16);
          u8g2.print("Cài Đặt");
          u8g2.setCursor(2, 32);
          u8g2.print("KHU VỰC CÂN");
          u8g2.setCursor(2, 48);
          switch (chonloaica.PhanLoaiKV){
            case PhanLoai::Not_Choose : u8g2.print("Chưa Chọn");break;
            case PhanLoai::Fil_IN : u8g2.print("FILLER-Vào");break;
            case PhanLoai::Fil_OUT : u8g2.print("FILLER-Ra");break;
            case PhanLoai::LANG_IN : u8g2.print("LẠNG Da-Vào");break;
            case PhanLoai::LANG_OUT : u8g2.print("LẠNG Da-Ra");break;
          }
     
   }
   else if (chedo_HT == 3) {
          u8g2.setCursor(2, 16);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(2, 16);
          u8g2.print("Cài Đặt");
          u8g2.setCursor(2, 32);
          u8g2.print("Loại Cá");
          u8g2.setCursor(2, 48);
          u8g2.print(Nha_SX.Loai_ca[chonloaica.STT_user_choose]);     
   }
    else if (chedo_HT == 4) {
          u8g2.setCursor(2, 16);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(2, 16);
          u8g2.print("Cài Đặt");
          u8g2.setCursor(2, 32);
          u8g2.print("Nhà Cup Cấp");
          
          u8g2.setCursor(2, 48);
          
          u8g2.print(Nha_SX.So_Lo[chonloaica.STT_user_choose_NhaCC]);       
   }
       else if (chedo_HT == 5) {
          u8g2.setCursor(2, 16);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(2, 16);
          u8g2.print("Cài Đặt");
          u8g2.setCursor(2, 32);
          u8g2.print("Loại Thành Phẩm");
          if (strlen(Nha_SX.Thanh_Pham[chonloaica.STT_user_choose_ThanhPham])>20){
                u8g2.drawUTF8(Scrolling_lcd, 48, Nha_SX.Thanh_Pham[chonloaica.STT_user_choose_ThanhPham]);
                Scrolling_lcd = Scrolling_lcd - 10 ;
                if (Scrolling_lcd>128)Scrolling_lcd=0;
          }
          else u8g2.drawUTF8(Scrolling_lcd, 48, Nha_SX.Thanh_Pham[chonloaica.STT_user_choose_ThanhPham]);
          
          //u8g2.setCursor(Scrolling_lcd, 48);
          //u8g2.print(Nha_SX.Thanh_Pham[chonloaica.STT_user_choose_ThanhPham]);       
   }
          else if (chedo_HT == 6) {
          u8g2.setCursor(2, 16);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(2, 16);
          u8g2.print("Tong Hop");    
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
   // SPI.setFrequency(1000000);
   SPI.setClockDivider( SPI_CLOCK_DIV32 );
    u8g2.begin();
    u8g2.enableUTF8Print();
    LCD_thong_tin(2,&Data_TH);
    uint8_t state_LCD_Display = 1;
    uint8_t daucham_lcd = 0;
          if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  
  }
  
    if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    //nam,thang,ngay,gio,phut,giay
     rtc.adjust(DateTime(2019, 11, 21,11, 20, 0));
  }
      button_left.begin();
    button_right.begin();
    button_error.begin();
    button_ok.begin();
    button_du_phong.begin();
    button_left.onPressed(onPressed_left);
    button_right.onPressed(onPressed_right);
    button_error.onPressed(onPressed_error);
    button_ok.onPressed(onPressed_ok);
    button_du_phong.onPressed(onPressed);
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
                }
                switch (state_LCD_Display){
                  case 0:
                        
                        LCD_thong_tin(0,&Data_TH,daucham_lcd);
                        state_LCD_Display=4;
                        break;
                  case 1:
                        if (xTaskGetTickCount()- _time_blink_LCD > 800){
                           daucham_lcd ++ ;
                          if (daucham_lcd > 3)daucham_lcd=0;
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
      else if ((xTaskGetTickCount()- _time_blink_LCD > 800)|| update_lcd_setting ){
        update_lcd_setting = false;
               _time_blink_LCD = xTaskGetTickCount();
               LCD_thong_tin(Status_setting.state_select + 2,&Data_TH,daucham_lcd);
      }
      if (xTaskGetTickCount()- _time_counting_send_heap > 5000){
                  DateTime now = rtc.now();
                  Serial.print(" since midnight 1/1/1970 = ");
                  Serial.println(now.unixtime());
                  _time_counting_send_heap = xTaskGetTickCount();
                   printf("Free Heap %d\n",ESP.getFreeHeap());
          
      }
      if (state_Running_conf::state_Running == state_Running_conf::Setting){
        button_left.read();
        button_right.read();
        button_ok.read();
      }
      button_error.read();
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
