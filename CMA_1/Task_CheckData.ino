char idRFID_OLD[25];


void http_re( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
    Data_CAN Data_CAN_TH;
    Data_RFID Data_RFID_TH;
    Data_RFID Data_RFID_NV;
    Data_TH Data_TH;
    unsigned long timeGetQueueCan = 0;
    unsigned long timeGetQueueRfidRo = 0;
    unsigned long timeGetQueueRfidNV = 0;
    unsigned long _time_get_tam = 0;
    unsigned long timeCompareMode1=10000;
    unsigned long timeCompareMode2=10000;
    unsigned long timeScheduTaskCheck=0;
    double canDataOutOld = 0;
    for (;;){
      Serial.print("Time Task Check : ");
      Serial.println(millis());
      /*
       * Nhận Cân
       */
    if(xQueueReceive( Queue_can, &Data_CAN_TH,  ( TickType_t ) 1 )== pdPASS ){
      timeGetQueueCan=xTaskGetTickCount();
    }
    /*
     * Nhận mã RFID mã Rổ
     * Nếu khu fille thì không nhận mã rỗ.
     */
    if(xQueueReceive( Queue_RFID, &Data_RFID_TH,  ( TickType_t ) 1 )== pdPASS ){
      if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){
      timeGetQueueRfidRo=xTaskGetTickCount();
      }     
    }
    /*
     * Nhận mã RFID
     * Nếu là khu Filler chỉ nhận mã rỗ thì swap time tới mã rỗ để khỏi viết lại code
     */
    if(xQueueReceive( Queue_RFID_NV, &Data_RFID_NV,  ( TickType_t ) 1 )== pdPASS ){
            if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){timeGetQueueRfidRo=xTaskGetTickCount();}
            else timeGetQueueRfidNV=xTaskGetTickCount();
    }
    /*
     * Nếu là Khu Phi Le thì chỉ dùng mã NV để quét
     * state_Running
     */
    if (state_Running_conf::state_Running == state_Running_conf::Running){
          if ((timeGetQueueCan > timeGetQueueRfidRo + 500)&&(timeGetQueueRfidRo > 0)){ // chỉ nhận khi dữ liệu cân lớn hơn dữ liệu rfid 500 stick
               timeCompareMode1 = timeGetQueueCan - timeGetQueueRfidRo;
              if (timeCompareMode1 < time_2_lan_nhan_data){ // 2 dữ liệu phải nhỏ hơn thời gian cài đặt mới là 1 cặp đúng
                if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){
                    strncpy( Data_TH.id_RFID_NV,Data_RFID_NV.id_RFID, sizeof(Data_RFID_NV.id_RFID));
                    Data_TH.data_weight=Data_CAN_TH.data_can;
                    strncpy( Data_TH.id_RFID,"x", sizeof("x"));
                }
                else {
                      strncpy( Data_TH.id_RFID,Data_RFID_TH.id_RFID, sizeof(Data_RFID_TH.id_RFID));
                      Data_TH.data_weight=Data_CAN_TH.data_can;
                      strncpy( Data_TH.id_RFID_NV,"x", sizeof("x"));
                }
                boolean tt = true;
                if (inforServer.PhanLoaiKV == PhanLoai::LANG_OUT){
                  /*
                   * nêu khác mã rổ thì không cần check cân
                   */
                    tt = false;
                  if (strcmp(Data_TH.id_RFID,idRFID_OLD) != 0){
                    strncpy( idRFID_OLD,Data_TH.id_RFID, sizeof(Data_TH.id_RFID));
                    if (Data_CAN_TH.data_can >0.5) tt = true;
                  }
                  else {  
                        double tam = Data_CAN_TH.data_can > canDataOutOld ?Data_CAN_TH.data_can - canDataOutOld :canDataOutOld - Data_CAN_TH.data_can ;
                        if ((tam > 0.8)&&(Data_CAN_TH.data_can >0.5))tt = true;
                        canDataOutOld=Data_CAN_TH.data_can;
                  }
                }
                if(tt){
                    xQueueSend( Queue_display, &Data_TH, xTicksToWait );
                    /*
                     * Nếu là Phi le và lạng da ngõ ra thì ko cần mã NV nên sẽ truyền MQTT
                     */
                   
                    if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){          
                    printf("CHECK OUT: Time: %ld - Kg: %f - RFID: %s \n",timeCompareMode1,Data_TH.data_weight,Data_TH.id_RFID);
                    xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                    }
                    else {
                      xSemaphoreGive(xreset_id_nv);            
                    }
                }
                xSemaphoreGive(xSignal_Display_check);     
                timeCompareMode1 = 10000;
                timeGetQueueCan = 0;
                timeGetQueueRfidRo = 0;
                _time_get_tam=xTaskGetTickCount();
              }
          }
          else if ((timeGetQueueRfidRo >0)&&(xTaskGetTickCount()- timeGetQueueRfidRo > time_2_lan_nhan_data)){
              printf("Over time: Ma Ro va Can \n");     
                timeGetQueueRfidRo=0;
                xSemaphoreGive(xreset_id_nv);      
                xSemaphoreGive(xSignal_Display_checkdone);
          }
          if ( inforServer.PhanLoaiKV == PhanLoai::LANG_IN){   
              if ((timeGetQueueRfidNV > _time_get_tam)&& (_time_get_tam >0)){ 
                  timeCompareMode2 = timeGetQueueRfidNV - _time_get_tam;
                  if (timeCompareMode2 < time_cho_nhan_RFID_NV){
                    _time_get_tam=timeGetQueueRfidNV;
                    strncpy( Data_TH.id_RFID_NV,Data_RFID_NV.id_RFID, sizeof(Data_RFID_NV.id_RFID));
                    printf("CHECK IN: Time: %ld - Kg: %f - RFID: %s - RFID NV: %s\n",timeCompareMode2,Data_TH.data_weight,Data_TH.id_RFID,Data_TH.id_RFID_NV);
                    xQueueSend( Queue_display, &Data_TH, xTicksToWait );
                    xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                    timeCompareMode2=10000;
                    timeGetQueueRfidNV=0;
                    _time_get_tam = 0;
                  }
                  else  {
                    xSemaphoreGive(xSignal_Display_checkdone);
                    timeGetQueueRfidNV=0;
                    printf("TIme out check rfid nv \n");
                  } 
              }
              else if ((_time_get_tam >0)&&(xTaskGetTickCount()- _time_get_tam > time_cho_nhan_RFID_NV)){
                 xSemaphoreGive(xResetRfidMaRo); // Gửi tín hiệu để reset mã rỗ
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
