#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0,/*CS=*/ U8X8_PIN_NONE,/*CS=*/ U8X8_PIN_NONE);// 
void setup(void) {
  Serial.begin(115200);
    SPI.setClockDivider( SPI_CLOCK_DIV32 );
    u8g2.begin();
    u8g2.enableUTF8Print();
 u8g2.clearBuffer();

  u8g2.firstPage();
  do {
     u8g2.setFont(u8g2_font_5x7_tf); // u8g2_font_6x10_tf
    u8g2.drawStr(0,10,"192.168.1.10 - IN");
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0,25,"ID NV :");
    u8g2.drawStr(0,40,"ID:");
    u8g2.drawStr(0,55,"Kg:");
  } while ( u8g2.nextPage() );
} 
u8g2_uint_t offset;      // current offset for the scrolling text
u8g2_uint_t width;  
int Scrolling_lcd=0;
u8g2_uint_t title_scroll_x = 0;
void hienthiSetting(char* dataUserDisplay){
  u8g2.clearBuffer();
   u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
   u8g2.setCursor(0, 48);
   uint16_t strLen = strlen(dataUserDisplay) * 8;
   char temstr[20];
   memset(temstr, '\0', sizeof(temstr));
   memcpy (temstr, dataUserDisplay + Scrolling_lcd, 16);
   Serial.println(strLen);
          if (strLen > 120){
                u8g2.print(temstr);
                Scrolling_lcd = Scrolling_lcd +1 ;
                if (Scrolling_lcd>strlen(dataUserDisplay))Scrolling_lcd=0;
          }
u8g2.sendBuffer();          
}
void loop(void) {
//    u8g2.clearBuffer();
    
 //   hienthiSetting("Pham An ");
    hienthiSetting("Cá Semi cua pham an nhan la goi ấ ahsdfa asdfaf afasgfqa fafa âf");
  //  u8g2.sendBuffer();
  
       delay(500);                          // do some small delay
}
