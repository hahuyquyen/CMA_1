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
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
    unsigned long TaskRFID_lastTimeSche = 0;
    int i =0;
    Serial2.begin(uart_rfid_baud_rate);
    nano.begin(Serial2); 
    nano.set_mode_timming(2,1000); // Set mode eprom 0x70, mode timming
    nano.set_timing_message(0x05,1000); //0x00 -> 0x64
    nano.set_power(0x10,1000); // 00 -> 96
    nano.set_out_mode(1,1000);
    nano.set_time_ner(0x05,1000); // tna so gui 1->255s
    nano.set_reset_reader(1000);
    for (;;){
      /*
       * Chuyển task 24ms
       */
                if (xTaskGetTickCount()-TaskRFID_lastTimeSche > 2000){
                    TaskRFID_lastTimeSche = xTaskGetTickCount();
                    i=i+1; 
                  }
                  if (nano.check() == true){ 
                    myEPClength = sizeof(myEPC);
                    if (nano.parseResponse(myEPC,myEPClength)){
                              if (myEPC[0] == MaRo_RFID){ 
                                array_to_string(&myEPC[0], 12, Data_rfid.id_RFID); //0->12 5->7
                                /*
                                 * nếu là khu vực cân 2 lần thì sẽ lúc nào cũng gửi về
                                 */
                               // if (inforServer.PhanLoaiKV == PhanLoai::LANG_OUT){
                                 if ((giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan] == kvSuaCa)&&(giaiDoanCan.cheDoInOut == cheDoOut)){ 
                                    strncpy( Data_rfid.id_RFID_Old,Data_rfid.id_RFID, sizeof(Data_rfid.id_RFID));
                                   // printf("So TAB: %s\n",Data_rfid.id_RFID);
                                    xQueueSend( Queue_RFID, &Data_rfid, xTicksToWait );
                                }
                                 else if (strcmp(Data_rfid.id_RFID,Data_rfid.id_RFID_Old) != 0){
                                    strncpy( Data_rfid.id_RFID_Old,Data_rfid.id_RFID, sizeof(Data_rfid.id_RFID));
                                   // printf("So TAB: %s\n",Data_rfid.id_RFID);
                                   // xSemaphoreGive(xSignal_FromRFID);
                                    xQueueSend( Queue_RFID, &Data_rfid, xTicksToWait );
                                 }
                              }
                              
                              else if (myEPC[0] == MaNV_RFID){ 
                                 array_to_string(&myEPC[0], 12, Data_rfid_nv.id_RFID);
                                 if (strcmp(Data_rfid_nv.id_RFID,Data_rfid_nv.id_RFID_Old) != 0){
                                    strncpy( Data_rfid_nv.id_RFID_Old,Data_rfid_nv.id_RFID, sizeof(Data_rfid_nv.id_RFID));
                                    xQueueSend( Queue_RFID_NV, &Data_rfid_nv, xTicksToWait );
                                 }
                              }
                    }
                  }
                  if(xSemaphoreTake(xreset_id_nv, 1)){
                        strncpy( Data_rfid_nv.id_RFID_Old,"", sizeof(""));
                  }
                  if(xSemaphoreTake(xResetRfidMaRo, 1)){
                       strncpy( Data_rfid.id_RFID_Old,"", sizeof(""));
                  }
                vTaskDelay(20);
                     // vTaskDelay(5000); 
     // printf("Task RFID StackHigh %d, Free Heap = %d\n",uxTaskGetStackHighWaterMark(NULL),ESP.getFreeHeap());    
    }
    vTaskDelete(NULL) ;
}
