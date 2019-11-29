
/*
  Vào thu vien sua file u8g2.h chỗ khai bao 16 bit de xu lý chuoi lon

  The following macro enables 16 Bit mode.
  Without defining this macro all calulations are done with 8 Bit (1 Byte) variables.
  Especially on AVR architecture, this will save some space.
  If this macro is defined, then U8g2 will switch to 16 Bit mode.
  Use 16 Bit mode for any display with more than 240 pixel in one
  direction.
*/
//#define U8G2_16BIT






#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0,/*CS=*/ U8X8_PIN_NONE,/*CS=*/ U8X8_PIN_NONE);//
//U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* data=*/ 23, /* CS=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);
void setup(void) {
  Serial.begin(115200);
  //  SPI.setClockDivider( SPI_CLOCK_DIV32 );
  u8g2.begin();
  u8g2.enableUTF8Print();
 // u8g2.setBusClock(600000);
  u8g2.clearBuffer();

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_5x7_tf); // u8g2_font_6x10_tf
    u8g2.drawStr(0, 10, "192.168.1.10 - IN");
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 25, "ID NV :");
    u8g2.drawStr(0, 40, "ID:");
    u8g2.drawStr(0, 55, "Kg:");
  } while ( u8g2.nextPage() );
}
u8g2_uint_t offset;      // current offset for the scrolling text
u8g2_uint_t width;
int Scrolling_lcd = 0;
u8g2_uint_t title_scroll_x = 0;
void hienthiSetting(char* dataUserDisplay) {
  u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
  Serial.print("begin ");
  Serial.println(millis());
  u8g2.clearBuffer();
  u8g2.setCursor(0, 48);
  if (u8g2.getUTF8Width(dataUserDisplay) > 128) {
    u8g2.drawStr(Scrolling_lcd, 48, dataUserDisplay);
    Scrolling_lcd = Scrolling_lcd - 12 ;
    if (abs(Scrolling_lcd) > u8g2.getUTF8Width(dataUserDisplay))Scrolling_lcd = 0;
  }
  Serial.print("end ");
  u8g2.sendBuffer();
  //u8g2.updateDisplayArea(10,48,20,20);
  Serial.println(millis());
}
void loop(void) {
  //    u8g2.clearBuffer();

  //   hienthiSetting("Pham An ");
  hienthiSetting("Cá Semi cua pham an nhan la goi ấ ahsdfa asdfaf afasgfqa fafa âf");
  //  u8g2.sendBuffer();

  delay(800);                          // do some small delay
}
