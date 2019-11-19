void http_re( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(2);
    Data_CAN Data_CAN_TH;
    Data_RFID Data_RFID_TH;
    Data_RFID Data_RFID_NV;
    Data_TH Data_TH;
    unsigned long _time_get_data_can = 0;
    unsigned long _time_get_data_rfid = 0;
    unsigned long _time_get_data_rfid_nv = 0;
    unsigned long _time_get_tam = 0;
    unsigned long _time_timeout_data=10000;
    unsigned long _time_timeout_data_VAO=10000;
    unsigned long _time_counting_task_check=0;
    for (;;){
      /*
       * Nhận Cân
       */
    if(xQueueReceive( Queue_can, &Data_CAN_TH,  ( TickType_t ) 2 )== pdPASS ){
      _time_get_data_can=xTaskGetTickCount();
    }
    /*
     * Nhận mã RFID mã Rổ
     */
    if(xQueueReceive( Queue_RFID, &Data_RFID_TH,  ( TickType_t ) 2 )== pdPASS ){
      _time_get_data_rfid=xTaskGetTickCount();
    }
    /*
     * Nếu là Khu Phi Le thì chỉ dùng mã NV để quét
     * state_Running
     */
    if (state_Running_conf::state_Running == state_Running_conf::Running){
          if(xQueueReceive( Queue_RFID_NV, &Data_RFID_NV,  ( TickType_t ) 2 )== pdPASS ){
            if ((chonloaica.PhanLoaiKV == PhanLoai::Fil_IN)||(chonloaica.PhanLoaiKV == PhanLoai::Fil_OUT)){
              _time_get_data_rfid=xTaskGetTickCount();
            }
            else _time_get_data_rfid_nv=xTaskGetTickCount();
          }
          /*
           * 
           */
          if (chonloaica.PhanLoaiKV == PhanLoai::LANG_OUT){
            /*
             * Cân 2 lần
             */
          }
          else if ((_time_get_data_can > _time_get_data_rfid + 1000)&&(_time_get_data_rfid > 0)){
               _time_timeout_data = _time_get_data_can - _time_get_data_rfid;
              if (_time_timeout_data < time_2_lan_nhan_data){
                strncpy( Data_TH.id_RFID,Data_RFID_TH.id_RFID, sizeof(Data_RFID_TH.id_RFID));
                Data_TH.data_weight=Data_CAN_TH.data_can;
                strncpy( Data_TH.id_RFID_NV,"x", sizeof("x"));
                xQueueSend( Queue_display, &Data_TH, xTicksToWait );
                /*
                 * Nếu là Phi le và lạng da ngõ ra thì ko cần mã NV nên sẽ truyền MQTT
                 */
                if ((chonloaica.PhanLoaiKV == PhanLoai::LANG_OUT)||(chonloaica.PhanLoaiKV == PhanLoai::Fil_IN)||(chonloaica.PhanLoaiKV == PhanLoai::Fil_OUT)){          
                printf("CHECK OUT: Time: %ld - Kg: %f - RFID: %s \n",_time_timeout_data,Data_TH.data_weight,Data_TH.id_RFID);
                xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                }
                else {
                  xSemaphoreGive(xreset_id_nv);            
                }
                xSemaphoreGive(xSignal_Display_check);     
                _time_timeout_data = 10000;
                _time_get_data_can = 0;
                _time_get_data_rfid = 0;
                _time_get_tam=xTaskGetTickCount();
              }
          }
          else if ((_time_get_data_rfid >0)&&(xTaskGetTickCount()- _time_get_data_rfid > time_2_lan_nhan_data)){
                /*     
                 *      cần 
                 *      reset ID mã rổ
                 */
                _time_get_data_rfid=0;
                printf("Over time: GET Ma Ro va Can \n"); 
                xSemaphoreGive(xreset_id_nv);      
                xSemaphoreGive(xSignal_Display_checkdone);
          }
          if ( chonloaica.PhanLoaiKV == PhanLoai::LANG_IN){   
              if ((_time_get_data_rfid_nv > _time_get_tam)&& (_time_get_tam >0)){ 
                  _time_timeout_data_VAO = _time_get_data_rfid_nv - _time_get_tam;
                  if (_time_timeout_data_VAO < time_cho_nhan_RFID_NV){
                    _time_get_tam=_time_get_data_rfid_nv;
                    strncpy( Data_TH.id_RFID_NV,Data_RFID_NV.id_RFID, sizeof(Data_RFID_NV.id_RFID));
                    printf("CHECK IN: Time: %ld - Kg: %f - RFID: %s - RFID NV: %s\n",_time_timeout_data_VAO,Data_TH.data_weight,Data_TH.id_RFID,Data_TH.id_RFID_NV);
                    xQueueSend( Queue_display, &Data_TH, xTicksToWait );
                    xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                    _time_timeout_data_VAO=10000;
                    _time_get_data_rfid_nv=0;
                  }
                  else  {
                    xSemaphoreGive(xSignal_Display_checkdone);
                    _time_get_data_rfid_nv=0;
                    printf("TIme out check rfid nv \n");
                  } 
              } 
              else if ((_time_get_tam >0)&&(xTaskGetTickCount()- _time_get_tam > time_cho_nhan_RFID_NV)){
                /*     
                 *      cần 
                 *      reset ID mã rổ
                 */
                _time_get_tam=0;
                printf("Over time: Ma NV va \n");      
                xSemaphoreGive(xSignal_Display_checkdone);
              }
          }
    }
    if (xTaskGetTickCount()- _time_counting_task_check > 1000){
      _time_counting_task_check=xTaskGetTickCount();
    }
      
      vTaskDelay(10);
    }
    vTaskDelete(NULL) ;
}
