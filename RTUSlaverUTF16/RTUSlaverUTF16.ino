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
char hienthiutf[100]; 
wchar_t hienthiascii[100];
char16_t output[100];

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1);
  mb.begin(&Serial2);
  mb.slave(SLAVE_ID);
  mb.addHreg(REGN,0x0000,100);
  mb.addHreg(30,0x0A00,1);
}
int i = 0;
int scenes = 10;
void loop() {
  if (Serial.available()){
    hienthiutf[i] = Serial.read();
    /*if (tams == 'Z'){
      //chuyển trang
      if (scenes == 10 ){
        mb.Hreg(300, 0x0B00);
      }
      else mb.Hreg(300, 0x0A00);
    }*/

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
      for (int j =0 ; j < sizeof(hienthiascii)/2 ; j++){        
        mb.Hreg(REGN + j, hienthiascii[j]);
      }
      /*for (int j = valuelen ; j < valuelen ; j++){        
        mb.Hreg(REGN + j, hienthiascii[j]);
      }*/
      memset(hienthiutf, '\0', sizeof(hienthiutf));
    }
    else if ( i > 99) {i=0;}
    else i = i+ 1;
  }
  mb.task();
  yield();
}
