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


void TaskRFID( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(50);
    data_user Data;
    RFID nano; //Create instance
    Data.id = 2;
    byte myEPC[12]; //Most EPCs are 12 bytes
    byte myEPClength;
    printf("Waiting for card..\n");
    long time_sche = 0;
    int i =0;
    Serial2.begin(115200);
    nano.begin(Serial2); 
    nano.set_mode_timming(1,5000);
    while(true){
                if (xTaskGetTickCount()-time_sche > 2000){
                    time_sche = xTaskGetTickCount();
                    i=i+1;
                    Data.id_RFID = i;
                    xQueueSend( Queue_can, &Data, xTicksToWait );
                  }
                  if (nano.check() == true){ 
                    myEPClength = sizeof(myEPC);
                    if (nano.parseResponse(myEPC,myEPClength)){
                        for (byte x = 0 ; x < myEPClength ; x++)
                        {
                          Serial2.print(myEPC[x], HEX);
                          Serial2.print(" ");
                        }  
                        Serial2.println("");
                    }
                  }
                vTaskDelay(20);
    }
    vTaskDelete(NULL) ;
}
