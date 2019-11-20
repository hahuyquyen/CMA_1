void http_re( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(2);
    Data_CAN Data_CAN_TH;
    Data_RFID Data_RFID_TH;
    Data_RFID Data_RFID_NV;
    Data_TH Data_TH;
    unsigned long timeGetQueueCan = 0;
    unsigned long timeGetQueueRfidRo = 0;
    unsigned long timeGetQueueRfidNV = 0;
    unsigned long _time_get_tam = 0;
    unsigned long _time_timeout_data=10000;
    unsigned long _time_timeout_data_VAO=10000;
    unsigned long timeScheduTaskCheck=0;
    double canDataOutOld = 0;
    for (;;){
      /*
       * Nhận Cân
       */
    if(xQueueReceive( Queue_can, &Data_CAN_TH,  ( TickType_t ) 2 )== pdPASS ){
      timeGetQueueCan=xTaskGetTickCount();
    }
    /*
     * Nhận mã RFID mã Rổ
     */
    if(xQueueReceive( Queue_RFID, &Data_RFID_TH,  ( TickType_t ) 2 )== pdPASS ){
      timeGetQueueRfidRo=xTaskGetTickCount();
    }
    /*
     * Nếu là Khu Phi Le thì chỉ dùng mã NV để quét
     * state_Running
     */
    if (state_Running_conf::state_Running == state_Running_conf::Running){
          if(xQueueReceive( Queue_RFID_NV, &Data_RFID_NV,  ( TickType_t ) 2 )== pdPASS ){
            if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){
              timeGetQueueRfidRo=xTaskGetTickCount();
            }
            else timeGetQueueRfidNV=xTaskGetTickCount();
          }
          /*
           * 
           */
          if ((timeGetQueueCan > timeGetQueueRfidRo + 1000)&&(timeGetQueueRfidRo > 0)){
               _time_timeout_data = timeGetQueueCan - timeGetQueueRfidRo;
              if (_time_timeout_data < time_2_lan_nhan_data){

                strncpy( Data_TH.id_RFID,Data_RFID_TH.id_RFID, sizeof(Data_RFID_TH.id_RFID));
                Data_TH.data_weight=Data_CAN_TH.data_can;
                strncpy( Data_TH.id_RFID_NV,"x", sizeof("x"));
                boolean tt = true;
                if (inforServer.PhanLoaiKV == PhanLoai::LANG_OUT){tt = false;
                  double tam = Data_CAN_TH.data_can > canDataOutOld ?Data_CAN_TH.data_can - canDataOutOld :canDataOutOld - Data_CAN_TH.data_can ;
                  if (tam > 0.5){
                    tt = true;
                  }
                  canDataOutOld=Data_CAN_TH.data_can;
                }
                if(tt){
                    xQueueSend( Queue_display, &Data_TH, xTicksToWait );
                    /*
                     * Nếu là Phi le và lạng da ngõ ra thì ko cần mã NV nên sẽ truyền MQTT
                     */
                    if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){          
                    printf("CHECK OUT: Time: %ld - Kg: %f - RFID: %s \n",_time_timeout_data,Data_TH.data_weight,Data_TH.id_RFID);
                    xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                    }
                    else {
                      xSemaphoreGive(xreset_id_nv);            
                    }
                }
                xSemaphoreGive(xSignal_Display_check);     
                _time_timeout_data = 10000;
                timeGetQueueCan = 0;
                timeGetQueueRfidRo = 0;
                _time_get_tam=xTaskGetTickCount();
              }
          }
          else if ((timeGetQueueRfidRo >0)&&(xTaskGetTickCount()- timeGetQueueRfidRo > time_2_lan_nhan_data)){
                /*     
                 *      cần 
                 *      reset ID mã rổ
                 */
                timeGetQueueRfidRo=0;
                printf("Over time: GET Ma Ro va Can \n"); 
                xSemaphoreGive(xreset_id_nv);      
                xSemaphoreGive(xSignal_Display_checkdone);
          }
          if ( inforServer.PhanLoaiKV == PhanLoai::LANG_IN){   
              if ((timeGetQueueRfidNV > _time_get_tam)&& (_time_get_tam >0)){ 
                  _time_timeout_data_VAO = timeGetQueueRfidNV - _time_get_tam;
                  if (_time_timeout_data_VAO < time_cho_nhan_RFID_NV){
                    _time_get_tam=timeGetQueueRfidNV;
                    strncpy( Data_TH.id_RFID_NV,Data_RFID_NV.id_RFID, sizeof(Data_RFID_NV.id_RFID));
                    printf("CHECK IN: Time: %ld - Kg: %f - RFID: %s - RFID NV: %s\n",_time_timeout_data_VAO,Data_TH.data_weight,Data_TH.id_RFID,Data_TH.id_RFID_NV);
                    xQueueSend( Queue_display, &Data_TH, xTicksToWait );
                    xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                    _time_timeout_data_VAO=10000;
                    timeGetQueueRfidNV=0;
                  }
                  else  {
                    xSemaphoreGive(xSignal_Display_checkdone);
                    timeGetQueueRfidNV=0;
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
    if (xTaskGetTickCount()- timeScheduTaskCheck > 1000){
      timeScheduTaskCheck=xTaskGetTickCount();
    }
      
      vTaskDelay(10);
    }
    vTaskDelete(NULL) ;
}
