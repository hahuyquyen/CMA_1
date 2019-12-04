
/*
  Vào thu vien sua file u8g2.h chỗ khai bao 16 bit de xu lý chuoi lon

  The following macro enables 16 Bit mode.
  Without defining this macro all calulations are done with 8 Bit (1 Byte) variables.
  Especially on AVR architecture, this will save some space.
  If this macro is defined, then U8g2 will switch to 16 Bit mode.
  Use 16 Bit mode for any display with more than 240 pixel in one
  direction.
  thay doi delay trong thu vien U8x8lib.cpp cac dong delay  bang vTaskDelay(arg_int/portTICK_PERIOD_MS);
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
u8g2_uint_t offset;      // current offset for the scrolling text
u8g2_uint_t width;
int Scrolling_lcd = 0;
u8g2_uint_t title_scroll_x = 0;

U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0,/*CS=*/ U8X8_PIN_NONE,/*CS=*/ U8X8_PIN_NONE);//
void hienthiSetting(char* dataUserDisplay) {
  u8g2.setFont(u8g2_font_unifont_t_vietnamese1);
  Serial.print("begin ");
  Serial.println(millis());
  u8g2.setBufferCurrTileRow(3);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 48);
 // u8g2.setBitmapMode(1);
  //u8g2.setDrawColor(2);
  if (u8g2.getUTF8Width(dataUserDisplay) > 128) {
    u8g2.drawStr(Scrolling_lcd, 48, dataUserDisplay);
    Scrolling_lcd = Scrolling_lcd - 1 ;
    if (abs(Scrolling_lcd) > u8g2.getUTF8Width(dataUserDisplay))Scrolling_lcd = 0;
  }
  u8g2.setBufferCurrTileRow(3);
  u8g2.sendBuffer();
  Serial.print("end ");
  Serial.println(millis());
}
void setup(void) {
  Serial.begin(115200);
  //  SPI.setClockDivider( SPI_CLOCK_DIV32 );
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setBusClock(850000);
  u8g2.setContrast(255);
  u8g2.setFontMode(0);
u8g2.setDrawColor(1);

hienthiSetting("Cá Semi cua pham an nhan la goi ấ ahsdfa asdfaf afasgfqa fafa âf");
//u8g2.sendF("c",0x004 |0x001 );
//u8g2.sendF("a",0x004| 0x001);
//u8g2.sendF("d",0x004| 0x001);
/* Data |= 0x01;//
 *  
 *    uint8_t StartByte = 0xF8;
  if (!IsCmd)
    StartByte |= ST7920_StartByte_RSmask;

  ST7920_CS_HIGH;

  Spi_multi_send_8(SPIx, &StartByte, 1);    // Send Start Byte

  StartByte = data & 0xF0;
  Spi_multi_send_8(SPIx, &StartByte, 1);    // Send High Byte

  StartByte = data << 4;
  Spi_multi_send_8(SPIx, &StartByte, 1);    // Send Low Byte
  
  ST7920_CS_LOW;
  * /#define ST7920_CmdExt_StandBy                   0x01
#define ST7920_CmdExt_SelScrollOrRamAddr        0x02
#define ST7920_CmdExt_Reverse                   0x04
#define ST7920_CmdExt_FunctionSet               0x20
#define ST7920_CmdExt_SetIRAMOrSccrollAddr      0x40
#define ST7920_CmdExt_SetGDRAMAddr              0x80
 */
 
/********************************************************
 */
}

void loop(void) {
  //u8g2.sendF("c",(0x40 & 0x3F));
//hienthiSetting("Cá Semi cua pham an nhan la goi ấ ahsdfa asdfaf afasgfqa fafa âf");
  
  delay(50);                          // do some small delay
}
