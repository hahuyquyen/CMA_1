

/*
 * CPU cháº¡y 240mhz
 * vTaskDelay (x)  x lÃ  sá»‘ stick muá»‘n trÃ¬ hoÃ£n
 * Trong freertos nÃªn dÃ¹ng hÃ m delay cá»§a RTOS thay vÃ¬ cá»§a arduino
 * VÃ¬ khi dÃ¹ng delay rtos thÃ¬ task sáº½ Ä‘Æ°á»£c chuyá»ƒn Ä‘á»•i trong delay
 * 
 * 
 */
#include <EasyButton.h>
boolean update_lcd_setting = false;
int Scrolling_lcd = 0;
const char* LCD_setting = "Cài Đặt";
EasyButton button_left(button_left_pin,200,true);
EasyButton button_right(button_right_pin,200,true);
EasyButton button_ok(button_ok_pin,200,true);
EasyButton button_error(button_error_pin,200,true);
EasyButton button_du_phong(button_du_phong_pin,1000,true);
void onPressed_left() {
  if ((Status_setting.state_select == 0)&&(inforServer.PhanLoaiKV > 0)){ inforServer.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((inforServer.PhanLoaiKV - 1) % (PhanLoai::LANG_OUT+1));}
  else  if ((Status_setting.state_select == 1)&&(inforServer.userSelectLoaiCa > 0)){ inforServer.userSelectLoaiCa = inforServer.userSelectLoaiCa - 1 ;}
  else if ((Status_setting.state_select == 2)&&(inforServer.userSelectNhaCC > 0)){inforServer.userSelectNhaCC = inforServer.userSelectNhaCC-1  ;}
  else if ((Status_setting.state_select == 3)&&(inforServer.userSelectThanhPham > 0)){inforServer.userSelectThanhPham = inforServer.userSelectThanhPham - 1; }
  Scrolling_lcd = 0;
  update_lcd_setting = true;
}
void onPressed_right() {
  if (Status_setting.state_select == 0){inforServer.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((inforServer.PhanLoaiKV + 1) % (PhanLoai::LANG_OUT+1));}
  else if ((Status_setting.state_select == 1)&&(inforServer.tongLoaiCa > 0)){inforServer.userSelectLoaiCa = (inforServer.userSelectLoaiCa > inforServer.tongLoaiCa )? 0 : (inforServer.userSelectLoaiCa + 1); }
  else if ((Status_setting.state_select == 2)&&(inforServer.tongNhaCC > 0)){inforServer.userSelectNhaCC = (inforServer.userSelectNhaCC > inforServer.tongNhaCC) ? 0 : (inforServer.userSelectNhaCC + 1);}
  else if ((Status_setting.state_select == 3)&&(inforServer.tongThanhPham > 0)){inforServer.userSelectThanhPham = (inforServer.userSelectThanhPham > inforServer.tongThanhPham) ? 0 : (inforServer.userSelectThanhPham + 1);}
  Scrolling_lcd = 0;
  update_lcd_setting = true;
}
void onPressed_ok() {
  if ((Status_setting.state_select == 0)&& (inforServer.PhanLoaiKV == 0)) return;
  if ((Status_setting.state_select == 1)&& (inforServer.userSelectLoaiCa == 0)) return;
  if ((Status_setting.state_select == 2)&& (inforServer.userSelectNhaCC == 0)) return;
  if ((Status_setting.state_select == 3)&& (inforServer.userSelectThanhPham == 0)) return;
  if  ((Status_setting.state_select == 0)&& ((inforServer.PhanLoaiKV == PhanLoai::Fil_OUT )||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT ))) {
    /*
     * đẩu ra nên bỏ qua chọn 
     */
     Status_setting.state_select = 3;
  }
  else{Status_setting.state_select = (Status_setting.state_select == 2) ? 4:Status_setting.state_select + 1;}
  if (Status_setting.state_select > 4){state_Running_conf::state_Running = state_Running_conf::Running;}
  Scrolling_lcd = 0;
  update_lcd_setting = true;
  Serial.println("ket thuc");
}
void onPressed_error() {
  Status_setting.state_select = 0 ;
  state_Running_conf::state_Running = state_Running_conf::Setting;
  Serial.println("onPressed_vitri");
}
void onPressed() {
Serial.println("onPressed");
}
void LCD_print_KV(uint8_t vitri = 48){
          char stringdem1[] = "FILLET-Đầu Vào";
          char stringdem2[] = "FILLET-Đầu Ra";
          char stringdem3[] = "LẠNG Da-Đầu Vao";
          char stringdem4[] = "LẠNG Da-Đầu Ra";
          switch (inforServer.PhanLoaiKV){
            case PhanLoai::Not_Choose : u8g2.setCursor(((128 - (u8g2.getUTF8Width(ramChuaChon))) / 2), vitri);u8g2.print(ramChuaChon);break;
            case PhanLoai::Fil_IN :u8g2.setCursor(((128 - (u8g2.getUTF8Width(stringdem1))) / 2), vitri); u8g2.print(stringdem1);break;
            case PhanLoai::Fil_OUT :u8g2.setCursor(((128 - (u8g2.getUTF8Width(stringdem2))) / 2), vitri); u8g2.print(stringdem2);break;
            case PhanLoai::LANG_IN : u8g2.setCursor(((128 - (u8g2.getUTF8Width(stringdem3))) / 2), vitri);u8g2.print(stringdem3);break;
            case PhanLoai::LANG_OUT :u8g2.setCursor(((128 - (u8g2.getUTF8Width(stringdem4))) / 2), vitri); u8g2.print(stringdem4);break;
          }
}
void hienthiSetting(char* dataDisplay,char* dataUserDisplay){
          u8g2.setCursor(((128 - (u8g2.getUTF8Width(LCD_setting))) / 2), 16);
          u8g2.print(LCD_setting);
          char stringdem[] = "Loại Cá";
          u8g2.setCursor(((128 - (u8g2.getUTF8Width(dataDisplay))) / 2), 32);
          u8g2.print(dataDisplay);
          if (u8g2.getUTF8Width(dataUserDisplay) > 128){
                u8g2.drawUTF8(Scrolling_lcd, 48, dataUserDisplay);
                Scrolling_lcd = Scrolling_lcd - 10 ;
                if (Scrolling_lcd>128)Scrolling_lcd=0;
          }
          else u8g2.drawUTF8(((128 - (u8g2.getUTF8Width(dataUserDisplay))) / 2), 48, dataUserDisplay);  
}
void LCD_thong_tin(uint8_t chedo_HT,Data_TH* Data_TH  , uint8_t daucham = 0){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
  if (chedo_HT == 0){
         
          u8g2.setCursor(0, 12);
          u8g2.print(F("NV:"));
          u8g2.setFont(u8g2_font_6x12_tf); 
          u8g2.print(Data_TH->id_RFID_NV);
          u8g2.setCursor(0, 28);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.print(F("Rổ:"));
          u8g2.setFont(u8g2_font_6x12_tf); 
          u8g2.print(Data_TH->id_RFID);
          u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
          u8g2.setCursor(0, 44);
          u8g2.print(F("Kg:"));
          u8g2.print(Data_TH->data_weight);
  }
   else if (chedo_HT == 1) {
          LCD_print_KV(16);
          u8g2.setCursor(2, 32);
          u8g2.print(F("Chờ NV "));
          switch (daucham){
            case 0:u8g2.print(F("|"));break;
            case 1:u8g2.print(F("/"));break;
            case 2:u8g2.print(F("-"));break;
            case 3:u8g2.print(F("\\"));break;
            default: break;
          }
          u8g2.setCursor(68, 32);  
          u8g2.print(F("Kg:"));   
          u8g2.print(can_data);   
           if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN ) || (inforServer.PhanLoaiKV == PhanLoai::LANG_IN )){
             u8g2.setCursor(2, 48); 
             u8g2.print(inforServer.nameSoLo[inforServer.userSelectNhaCC]);  
             u8g2.setCursor(2, 60);
             u8g2.print(inforServer.nameLoaiCa[inforServer.userSelectLoaiCa]);  
           }
           else {
            u8g2.setCursor(2, 48); 
             u8g2.print(inforServer.nameThanhPham[inforServer.userSelectThanhPham]);      
           }               
   }
   else if (chedo_HT == 2) {
          char stringdem[] = "KHU VỰC CÂN";
          hienthiSetting(stringdem,"");
          LCD_print_KV();  
   }
   else if (chedo_HT == 3) {
          char stringdem[] = "Loại Cá";
          hienthiSetting(stringdem,inforServer.nameLoaiCa[inforServer.userSelectLoaiCa]); 
   }
    else if (chedo_HT == 4) {
         char stringdem[] = "Nhà Cup Cấp";
         hienthiSetting(stringdem,inforServer.nameSoLo[inforServer.userSelectNhaCC]); 
   }
    else if (chedo_HT == 5) {
          char stringdem[] = "Loại Thành Phẩm";
          hienthiSetting(stringdem,inforServer.nameThanhPham[inforServer.userSelectThanhPham]);     
   }
   else if (chedo_HT == 6) {
          char xacnhan[]="Xác Nhận";
          u8g2.setCursor(((128 - (u8g2.getUTF8Width(xacnhan))) / 2), 16);
          u8g2.print(xacnhan);
          LCD_print_KV(32);
           if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN ) || (inforServer.PhanLoaiKV == PhanLoai::LANG_IN )){
             u8g2.setCursor(2, 48); 
             u8g2.print(inforServer.nameSoLo[inforServer.userSelectNhaCC]);  
             u8g2.setCursor(2, 60);
             u8g2.print(inforServer.nameLoaiCa[inforServer.userSelectLoaiCa]);  
           }
           else {
            u8g2.setCursor(2, 48); 
             u8g2.print(inforServer.nameThanhPham[inforServer.userSelectThanhPham]);      
           }    
   }
    u8g2.sendBuffer();  
}


void Display( void * pvParameters ){
    boolean status_led= true;
    Data_TH Data_TH;
    pinMode(2, OUTPUT);
    pinMode(Pin_Coi, OUTPUT);
    unsigned long getTimeBlinkLed=0;
    unsigned long timeoutDisplay =0;
    unsigned long _time_blink_LCD=0;
    uint16_t Time_blink= 1000;
    uint16_t Time_check= 1500;  
    SPI.setClockDivider( SPI_CLOCK_DIV32 );
    u8g2.begin();
    u8g2.enableUTF8Print();
    LCD_thong_tin(2,&Data_TH);
    uint8_t state_LCD_Display = 1;
    uint8_t daucham_lcd = 0;
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
    unsigned long timeoutLcdLangDaIn = 0;
   for (;;){Serial.print("Time Task Display : ");
      Serial.println(millis());
      /*
       * 30ms
       */
      if (state_Running_conf::state_Running == state_Running_conf::Running){
                if(xQueueReceive( Queue_display, &Data_TH,  ( TickType_t ) 2 )== pdPASS ){ state_LCD_Display = 0;if ((strcmp(Data_TH.id_RFID_NV,"x") != 0)&& (inforServer.PhanLoaiKV ==PhanLoai::LANG_IN )) {timeoutLcdLangDaIn= xTaskGetTickCount();}}     
                if(xSemaphoreTake(xSignal_Display_check, 5)){
                  digitalWrite(Pin_Coi,HIGH); timeoutDisplay = xTaskGetTickCount();
               }
                if(xSemaphoreTake(xSignal_Display_checkdone, 5)){ //Che do IN qua timeout se tat 
                  state_LCD_Display = 1;
                  digitalWrite(Pin_Coi,LOW);
                }
                switch (inforServer.PhanLoaiKV){ //inforServer.PhanLoaiKV == PhanLoai::LANG_OUT
                      case PhanLoai::Not_Choose : if ((xTaskGetTickCount() - timeoutDisplay > Time_check)&&(timeoutDisplay > 0 )){digitalWrite(4,LOW);state_LCD_Display = 1;timeoutDisplay=0;}break;
                      case PhanLoai::Fil_OUT : if ((xTaskGetTickCount() - timeoutDisplay > Time_check)&&(timeoutDisplay > 0 )){digitalWrite(4,LOW);state_LCD_Display = 1;timeoutDisplay=0;}break;
                      case PhanLoai::Fil_IN : if ((xTaskGetTickCount() - timeoutDisplay > Time_check)&&(timeoutDisplay > 0 )){digitalWrite(4,LOW);state_LCD_Display = 1;timeoutDisplay=0;}break;
                      case PhanLoai::LANG_OUT : if((xTaskGetTickCount() - timeoutDisplay > Time_check)&&(timeoutDisplay > 0 )){digitalWrite(4,LOW);state_LCD_Display = 1;timeoutDisplay=0;}break;
                      case PhanLoai::LANG_IN : if ((xTaskGetTickCount() - timeoutLcdLangDaIn > Time_check)&&(timeoutLcdLangDaIn > 0 )){digitalWrite(4,LOW);state_LCD_Display = 1;timeoutLcdLangDaIn=0;}break;
                      default: break;
                }
                    
                xQueueReceive( Queue_Time_blink, &Time_blink,  ( TickType_t ) 2 );
                /*
                 * Time blink led
                 */      
                if (xTaskGetTickCount()- getTimeBlinkLed > Time_blink){
                  getTimeBlinkLed = xTaskGetTickCount();
                  status_led=!status_led;
                  digitalWrite(2,status_led);
                }
                printDebugHeap();
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
      printDebugHeap();
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
unsigned long getTimeSendHeapDebug=0;
void printDebugHeap(){
        if (xTaskGetTickCount()- getTimeSendHeapDebug > 15000){
                  DateTime now = rtc.now();
                  getTimeSendHeapDebug = xTaskGetTickCount();
                  printf("Time 1/1/1970 = %lu ,Free Heap = %d\n",now.unixtime(),ESP.getFreeHeap());    
      }
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
