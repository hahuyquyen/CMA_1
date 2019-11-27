#include "rfid.h"
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
void TaskRFID( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
    unsigned long _time_counting_task_rfid = 0;
    int i =0;
    Serial1.begin(9600, SERIAL_8N1, 26, 27); 
    RFID nano; 
    nano.begin(Serial1); 
    nano.set_mode_timming(2,5000); // Set mode eprom 0x70, timeout chÃ¡Â»ï¿½ kÃ¡ÂºÂ¿t quÃ¡ÂºÂ£ 5000ms
    nano.set_timing_message(0x64,5000);
    nano.set_power(0x64,5000);
    nano.set_out_mode(1,5000);
    nano.set_time_ner(0x05,5000);
    nano.set_reset_reader(2000);
    char idRfid[25];
    char idRfidOld[25];
    for (;;){

                if (xTaskGetTickCount()-_time_counting_task_rfid > 2000){
                    _time_counting_task_rfid = xTaskGetTickCount();
                    i=i+1; 
                  }
                  if (nano.check() == true){ 
                    myEPClength = sizeof(myEPC);
                    if (nano.parseResponse(myEPC,myEPClength)){
                             if (myEPC[0] == 0x00){ 
                                 array_to_string(&myEPC[0], 12, idRfid);
                                 if (strcmp(idRfid,idRfidOld) != 0){strncpy( idRfidOld,idRfid, sizeof(idRfid));xQueueSend( Queue_RFID, &idRfid, xTicksToWait );}
                              }
                    }
                  }
                vTaskDelay(20);
    }
    vTaskDelete(NULL) ;
}
