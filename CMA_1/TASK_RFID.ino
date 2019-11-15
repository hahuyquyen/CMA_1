/*
 * RFID mÃ¬nh dÃ¹ng module JT-2850, giao tiáº¿p uart
 */
//Calculate the checksum of the message and added as the last byte before sending it over
//to RFID reader
/*
 * ESP-> 0xA0-lenght
 * Reader->ESP: 0xE4-lenght hoáº·c 0xE0-lenght
 */

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
    Data_RFID Data_rfid;
    Data_RFID Data_rfid_nv;
    RFID nano; 
    const TickType_t xTicksToWait = pdMS_TO_TICKS(2);
    unsigned long _time_counting_task_rfid = 0;
    int i =0;
    Serial2.begin(uart_rfid_baud_rate);
    nano.begin(Serial2); 
    nano.set_mode_timming(2,5000); // Set mode eprom 0x70, timeout chá»� káº¿t quáº£ 5000ms
    nano.set_timing_message(0x64,5000);
    nano.set_power(0x20,5000);
    nano.set_out_mode(1,5000);
    nano.set_time_ner(0x05,5000);
    nano.set_reset_reader(2000);
    for (;;){
                if (xTaskGetTickCount()-_time_counting_task_rfid > 2000){
                    _time_counting_task_rfid = xTaskGetTickCount();
                    i=i+1; 
                  }
                  if (nano.check() == true){ 
                    myEPClength = sizeof(myEPC);
                    if (nano.parseResponse(myEPC,myEPClength)){
                              if (myEPC[0] == MaRo_RFID){ //NHAN VIEN
                                array_to_string(&myEPC[5], 7, Data_rfid.id_RFID); //0->12 5->7
                                 if (strcmp(Data_rfid.id_RFID,Data_rfid.id_RFID_Old) != 0){
                                    strncpy( Data_rfid.id_RFID_Old,Data_rfid.id_RFID, sizeof(Data_rfid.id_RFID));
                                    printf("So TAB: %s\n",Data_rfid.id_RFID);
                                   // xSemaphoreGive(xSignal_FromRFID);
                                    xQueueSend( Queue_RFID, &Data_rfid, xTicksToWait );
                                 }
                              }
                              else if (myEPC[0] == MaNV_RFID){ 
                                 array_to_string(&myEPC[5], 7, Data_rfid_nv.id_RFID);
                                 if (strcmp(Data_rfid_nv.id_RFID,Data_rfid_nv.id_RFID_Old) != 0){
                                    strncpy( Data_rfid_nv.id_RFID_Old,Data_rfid_nv.id_RFID, sizeof(Data_rfid_nv.id_RFID));
                                    xQueueSend( Queue_RFID_NV, &Data_rfid_nv, xTicksToWait );
                                 }
                              }
                    }
                  }
                        if(xSemaphoreTake(xreset_id_nv, 10)){
                        strncpy( Data_rfid_nv.id_RFID,"", sizeof(""));
                        strncpy( Data_rfid_nv.id_RFID_Old,"", sizeof(""));
                         strncpy( Data_rfid.id_RFID,"", sizeof(""));
                        strncpy( Data_rfid.id_RFID_Old,"", sizeof(""));
                              }
                vTaskDelay(20);
    }
    vTaskDelete(NULL) ;
}

