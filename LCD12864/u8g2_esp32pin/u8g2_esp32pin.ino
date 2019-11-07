#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* data=*/ 5, /* CS=*/ U8X8_PIN_NONE);



void setup(void) {
  u8g2.begin();
 // u8g2.setFlipMode(0);
/* u8g2.clearBuffer();
     u8g2.setFont(u8g2_font_5x7_tf); // u8g2_font_6x10_tf
    u8g2.drawStr(0,10,"192.168.1.10 - IN");
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0,25,"ID NV :");
    u8g2.drawStr(0,40,"ID:");
    u8g2.drawStr(0,55,"Kg:");
    u8g2.sendBuffer();  */
 // u8g2.setFont(u8g2_font_unifont_t_chinese2);  // use chinese2 for all the glyphs of "你好世界"
/*   u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_cu12_tr);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("Hello World!");
  u8g2.setCursor(0, 40);
  u8g2.sendBuffer();*/
  int bien = 100;
    do {
    u8g2.setFont(u8g2_font_5x7_tf); // u8g2_font_6x10_tf
    u8g2.drawStr(0,8,"192.168.1.10 - IN");
    u8g2.setFont(u8g2_font_5x8_tf );
    u8g2.drawStr(0,23,"ID NV :");
    u8g2.drawStr(0,35,"ID:");
    u8g2.drawStr(0,47,"Kg:");u8g2.setCursor(30, 47);u8g2.print(bien);
    u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
    u8g2.setCursor(0, 59);
    u8g2.drawUTF8(0,64,"Loại Cá:");
    //u8g2.print("Loại cá");
    //
  } while ( u8g2.nextPage() );
} 

void loop(void) {
  /*u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_cu12_tr); // choose a suitable font
  u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display*/
  
  delay(10000);  
}
