#include <HardwareSerial.h>
#include "rfid.h" //Library for controlling the M6E Nano module
RFID nano; //Create instance
byte myEPC[12]; //Most EPCs are 12 bytes
byte myEPClength;
char id_RFID[25];
char id_RFID_old[25];
void IRAM_ATTR array_to_string(byte* array, unsigned int len, char* buffer)
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}
void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600);
  nano.begin(Serial2); 
  nano.set_mode_timming(2,5000); // Set mode eprom 0x70, timeout chờ kết quả 5000ms
  nano.set_timing_message(0x64,5000);
  nano.set_power(0x20,5000);
  nano.set_out_mode(1,5000);
  nano.set_time_ner(0x05,5000);
 // nano.set_OFFdelaytime(0x60,5000);
  nano.set_reset_reader(2000);
  
}
unsigned long time_gg=0;
void loop()
{
  if (nano.check() == true){ 
    myEPClength = sizeof(myEPC);
    if (nano.parseResponse(myEPC,myEPClength)){
      array_to_string(myEPC, 12, id_RFID);
     // if (strcmp(id_RFID, id_RFID_old) != 0){
        strncpy(id_RFID_old, id_RFID, sizeof(id_RFID_old));
        Serial.print("So TAB: ");
        Serial.println(id_RFID);
    //    }
    }
  }  
}
