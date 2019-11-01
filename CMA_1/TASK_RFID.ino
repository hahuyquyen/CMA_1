/*
 * RFID mình dùng module JT-2850, giao tiếp uart
 */
//Calculate the checksum of the message and added as the last byte before sending it over
//to RFID reader
/*
 * ESP-> 0xA0-lenght
 * Reader->ESP: 0xE4-lenght hoặc 0xE0-lenght
 */

#include "rfid.h"
static byte myEPC[12]; //Most EPCs are 12 bytes
static byte myEPClength;
data_user Data_Task_RFID;
RFID nano; 
void array_to_string(byte* array, unsigned int len, char* buffer)
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
void TaskRFID( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(2);
    Data_Task_RFID.id = 2;
    printf("Waiting for card..\n");
    long time_sche = 0;
    int i =0;
    Serial2.begin(115200);
    nano.begin(Serial2); 
    nano.set_mode_timming(1,1000);
    while(true){
                if (xTaskGetTickCount()-time_sche > 2000){
                    time_sche = xTaskGetTickCount();
                    i=i+1;
                    //Data.id_RFID = i;
                    
                  }
                  if (nano.check() == true){ 
                    myEPClength = sizeof(myEPC);
                    if (nano.parseResponse(myEPC,myEPClength)){
                        array_to_string(myEPC, 12, Data_Task_RFID.id_RFID);
                        xQueueSend( Queue_can, &Data_Task_RFID, xTicksToWait );
                    }
                  }
                vTaskDelay(20);
    }
    vTaskDelete(NULL) ;
}
