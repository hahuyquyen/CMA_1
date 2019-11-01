#include <HardwareSerial.h>
#include "rfid.h" //Library for controlling the M6E Nano module
RFID nano; //Create instance
byte myEPC[12]; //Most EPCs are 12 bytes
byte myEPClength;
void setup()
{
  Serial.begin(115200);
  nano.begin(Serial); 
  nano.set_mode_timming(1,5000); // Set mode eprom 0x70, timeout chờ kết quả 5000ms
}
unsigned long time_gg=0;
void loop()
{
  if (nano.check() == true){ 
    myEPClength = sizeof(myEPC);
    if (nano.parseResponse(myEPC,myEPClength)){
        for (byte x = 0 ; x < myEPClength ; x++)
        {
          Serial.print(myEPC[x], HEX);
          Serial.print(" ");
        }  
        Serial.println(" ");  
    }
  }  
}
