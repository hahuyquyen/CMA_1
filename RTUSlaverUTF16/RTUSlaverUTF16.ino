/*
  ModbusRTU ESP8266/ESP32
  Simple slave example
  
  (c)2019 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266
*/
#include "cutf.h"
#include <ModbusRTU.h>
/*#include <string> 
#include <locale> 
#include <codecvt> */
/*
std::u16string dataRTU;
std::u16string convertutf8toutf16(char* input , char16_t* out){
std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert; 
std::u16string dest = convert.from_bytes(input); 
return dest;
}
     // std::u16string dataRTU1 = convertutf8toutf16(hienthiutf,output);
 */
#define REGN 0
#define SLAVE_ID 1

ModbusRTU mb;
char hienthiutf[200]; 
wchar_t hienthiascii[150];
//char16_t output[100];
uint32_t sogui = 0;
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1);
  mb.begin(&Serial2);
  mb.slave(SLAVE_ID);
  mb.addHreg(REGN,0x0000,305);
  mb.Hreg(300, 10);
}
int i = 0;
int scenes = 10;
long time_cho = 0;
long delay_ms = 0;
void loop() {
  if (Serial.available()){
    hienthiutf[i] = Serial.read();
    if (hienthiutf[i] == '\n'){
      i = 0;
      wmemset(hienthiascii,0x0000, sizeof(hienthiascii)/2);
      size_t valuelen = utf8towchar(hienthiutf,SIZE_MAX, hienthiascii, sizeof(hienthiascii));
      Serial.print("LEN ");
      Serial.print(valuelen);
      Serial.print("  Hienthi ");
      Serial.print(sizeof(hienthiascii));
      Serial.print("  Hienthi0 ");
      Serial.println(hienthiascii[1],HEX);
      for (int j =0 ; j < sizeof(hienthiascii)/2 ; j++){mb.Hreg(REGN + j, hienthiascii[j]);}
      mb.Hreg(300, 11);
      time_cho = millis();
      scenes = 11;
      mb.Hreg(300, 11);
      /*for (int j = valuelen ; j < valuelen ; j++){        
        mb.Hreg(REGN + j, hienthiascii[j]);
      }*/
      memset(hienthiutf, '\0', sizeof(hienthiutf));
    }
    else if ( i > 200) {i=0;}
    else i = i+ 1;
  }
  if((millis() - time_cho > 5000)&&(scenes == 11)){
    scenes = 10;
    mb.Hreg(300, 10);
  }
  if(millis() - delay_ms > 1000){
    delay_ms = millis();
    sogui = sogui+1;
    Serial.print(" So :");
    Serial.print(sogui,HEX);
    Serial.println(sogui>>16, HEX);
    mb.Hreg(301, sogui);
    mb.Hreg(302, sogui >> 16);
  }
  mb.task();
  yield();
}
