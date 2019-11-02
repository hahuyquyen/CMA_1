void http_re( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(2);
    data_user Data_CAN;
    data_user Data;
    unsigned long _time_get_data_can = 0;
    unsigned long _time_get_data_rfid = 0;
    unsigned long _time_timeout_data=10000;
    unsigned long _time_counting_task_check=0;
    while(true){
      
    /* 
     *  Kiểm tra có tín hiệu queue từ 2 task
     *  sẽ ghi lại thời gian nhận tín hiệu 2 task
     *  nếu thời gian nhận dữ liệu cân và rfid quá xa nhau thì sẽ bỏ . Tính là data lôiz
     *  2 dữ liệu phải nhận trong vòng 2s
    */
    if(xQueueReceive( Queue_can, &Data_CAN,  ( TickType_t ) 2 )== pdPASS ){
      if(Data_CAN.id == 1){
        _time_get_data_can =xTaskGetTickCount();
        Data.data_weight=Data_CAN.data_weight;
        Data.data_tare = Data_CAN.data_tare;
      }
      else if(Data_CAN.id == 2){
        strcpy(Data.id_RFID, Data_CAN.id_RFID);
        _time_get_data_rfid=xTaskGetTickCount();
      }
    }
    if (_time_get_data_rfid > time_2_lan_nhan_data){ _time_timeout_data = _time_get_data_rfid > _time_get_data_can?_time_get_data_rfid - _time_get_data_can:_time_get_data_can - _time_get_data_rfid;}
    if (_time_timeout_data < time_2_lan_nhan_data){
      printf("TimeCount: %ld \n", _time_timeout_data);
      printf("Can weight: %f \n",Data.data_weight);
      printf("Can tare: %f \n", Data.data_tare);
      printf("RFID: %s \n", Data.id_RFID);
      xQueueSend( Queue_mqtt, &Data, xTicksToWait );
      _time_timeout_data = 10000;
      _time_get_data_can = 0;
    }
    if (xTaskGetTickCount()- _time_counting_task_check > 1000){
      _time_counting_task_check=xTaskGetTickCount();
    }
      
      vTaskDelay(10);
    }
    vTaskDelete(NULL) ;
}
