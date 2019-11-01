void http_re( void * pvParameters ){
    data_user Data_CAN;
    data_user Data_RF;
    data_user Data;
    long time_count1 = 0;
    long time_count2 = 0;
    long time_count=10000;
    long time_sche=0;
    while(true){
      
    /* check whether receiving is ok or not == pdPASS*/
    if(xQueueReceive( Queue_can, &Data_CAN,  ( TickType_t ) 2 )== pdPASS ){
      if(Data_CAN.id == 1){
        time_count1=xTaskGetTickCount();
        Data.data_weight=Data_CAN.data_weight;
        Data.data_tare = Data_CAN.data_tare;
      }
      else if(Data_CAN.id == 2){
        strcpy(Data.id_RFID, Data_CAN.id_RFID);
        Serial.println(Data.id_RFID);
        time_count2=xTaskGetTickCount();
      }
    }
    if (time_count2 > 5000){ time_count = time_count2 > time_count1?time_count2 - time_count1:time_count1 - time_count2;}
    if (time_count < 500){
      printf("TimeCount: %ld \n", time_count);
      printf("Can weight: %f \n",Data.data_weight);
      printf("Can tare: %f \n", Data.data_tare);
      printf("RFID: %s \n", Data.id_RFID);
      time_count = 10000;
      time_count1 = 0;
    }
    if (xTaskGetTickCount()-time_sche > 1000){
      time_sche=xTaskGetTickCount();
    }
      
      vTaskDelay(10);
    }
}
