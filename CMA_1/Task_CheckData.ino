void http_re( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(2);
    Data_CAN Data_CAN_TH;
    Data_RFID Data_RFID_TH;
    Data_TH Data_TH;
    unsigned long _time_get_data_can = 0;
    unsigned long _time_get_data_rfid = 0;
    unsigned long _time_get_data_rfid_nv = 0;
    unsigned long _time_get_tam = 0;
    unsigned long _time_timeout_data=10000;
    unsigned long _time_counting_task_check=0;
    while(true){
      
    /* 
     *  Kiểm tra có tín hiệu queue từ 2 task
     *  sẽ ghi lại thời gian nhận tín hiệu 2 task
     *  nếu thời gian nhận dữ liệu cân và rfid quá xa nhau thì sẽ bỏ . Tính là data lôiz
     *  2 dữ liệu phải nhận trong vòng 2s
    */
    if(xQueueReceive( Queue_can, &Data_CAN_TH,  ( TickType_t ) 2 )== pdPASS ){
      _time_get_data_can=xTaskGetTickCount();
    }
    if(xQueueReceive( Queue_RFID, &Data_RFID_TH,  ( TickType_t ) 2 )== pdPASS ){
      _time_get_data_rfid=xTaskGetTickCount();
    }
    if(xQueueReceive( Queue_RFID_NV, &Data_RFID_TH,  ( TickType_t ) 2 )== pdPASS ){
      _time_get_data_rfid_nv=xTaskGetTickCount();
    }
    if (_time_get_data_can > _time_get_data_rfid + 200){
        if (_time_get_data_rfid > time_2_lan_nhan_data){ _time_timeout_data = _time_get_data_rfid > _time_get_data_can?_time_get_data_rfid - _time_get_data_can:_time_get_data_can - _time_get_data_rfid;}
        if (_time_timeout_data < time_2_lan_nhan_data){
          strncpy( Data_TH.id_RFID,Data_RFID_TH.id_RFID, sizeof(Data_RFID_TH.id_RFID));
          Data_TH.data_weight=Data_CAN_TH.data_can;
#ifndef USING_IN          
          printf("TimeCount: %ld \n", _time_timeout_data);
          printf("CAN : %f \n",Data_TH.data_weight);
          printf("RFID: %s \n", Data_TH.id_RFID);
          //bat coi
#else
          //bat coi, hien thi LCD so can, reset ID NV to 0
#endif    
          xSemaphoreGive(xSignal_Display_check);
          xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
          _time_timeout_data = 10000;
          _time_get_data_can = 0;
          _time_get_tam=xTaskGetTickCount();
        }
    }
#ifdef USING_IN
    if ((_time_get_data_rfid_nv > _time_get_tam)&& (_time_get_tam >0)){ _time_timeout_data = _time_get_data_rfid_nv - _time_get_tam;
        if (_time_timeout_data < time_2_lan_nhan_data){
           xSemaphoreGive(xSignal_Display_checkdone);
          _time_get_tam=_time_get_data_rfid_nv;
          strncpy( Data_TH.id_RFID_NV,Data_RFID_TH.id_RFID, sizeof(Data_RFID_TH.id_RFID));
          printf("CAN : %f \n",Data_TH.data_weight);
          printf("RFID: %s \n", Data_TH.id_RFID);
          printf("RFID NV: %s \n", Data_TH.id_RFID_NV);
        }
           
    }
#endif
    if (xTaskGetTickCount()- _time_counting_task_check > 1000){
      _time_counting_task_check=xTaskGetTickCount();
    }
      
      vTaskDelay(10);
    }
    vTaskDelete(NULL) ;
}
