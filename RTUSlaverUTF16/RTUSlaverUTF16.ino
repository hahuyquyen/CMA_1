/*
  ModbusRTU ESP8266/ESP32
  Simple slave example
  
  (c)2019 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266
*/

#include <ModbusRTU.h>
#include <string> 
#include <locale> 
#include <codecvt> 
#define REGN 0
#define SLAVE_ID 1

ModbusRTU mb;
char hienthiutf[100]; 
char hienthiascii[100];
char16_t output[100];
std::u16string dataRTU;
std::u16string convertutf8toutf16(char* input , char16_t* out){
 // std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert; 
  //output = convert.from_bytes(hienthiutf);
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert; 
  std::u16string dest = convert.from_bytes(input); 
//  strcpy(out,convert.from_bytes(input).c_str());
  //out = convert.from_bytes(input).c_str(); 
  return dest;
}
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8E1);
  mb.begin(&Serial2);
  mb.slave(SLAVE_ID);
  mb.addHreg(REGN,0x0000,100);
}
int i = 0;
void loop() {
  if (Serial.available()){
    hienthiutf[i] = Serial.read();
    if (hienthiutf[i] == '\n'){
      i = 0;
      std::u16string dataRTU1 = convertutf8toutf16(hienthiutf,output);
      Serial.print("LEN ");
      Serial.println(dataRTU1.length());
      for (int j =0 ; j < dataRTU1.length() ; j++){ 
          
        mb.Hreg(REGN + j, dataRTU1[j]);
      }
    }
    i = i+ 1;
  }
  mb.task();
  yield();
}
