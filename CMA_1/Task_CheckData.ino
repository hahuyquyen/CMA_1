char idRFID_OLD[25];


void http_re( void * pvParameters ){
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
    Data_CAN Data_CAN_TH;
    Data_RFID Data_RFID_TH;
    Data_RFID Data_RFID_NV;
    Data_TH Data_TH;
    unsigned long lastTimeGetQueueCan = 0;
    unsigned long lastTimeGetQueueRFID_Ro = 0;
    unsigned long lastTimeGetQueueRFID_NV = 0;
    unsigned long lastTimeGetData_RoVaCan = 0;
    unsigned long timeCompareMode1=10000;
    unsigned long timeCompareMode2=10000;
    unsigned long timeScheduTaskCheck=0;
    double canDataOutOld = 0;
    for (;;){

      /*
       * Nhận Cân
       */
    if(xQueueReceive( Queue_can, &Data_CAN_TH,  ( TickType_t ) 1 )== pdPASS ){
      lastTimeGetQueueCan=xTaskGetTickCount();
    }
    /*
     * Nhận mã RFID mã Rổ
     * Nếu khu fille thì không nhận mã rỗ.
     */
    if(xQueueReceive( Queue_RFID, &Data_RFID_TH,  ( TickType_t ) 1 )== pdPASS ){
      //if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){ 
      if (giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan] == kvSuaCa) {
      lastTimeGetQueueRFID_Ro=xTaskGetTickCount();
      }     
    }
    /*
     * Nhận mã RFID
     * Nếu là khu Filler chỉ nhận mã rỗ thì swap time tới mã rỗ để khỏi viết lại code
     */
    if(xQueueReceive( Queue_RFID_NV, &Data_RFID_NV,  ( TickType_t ) 1 )== pdPASS ){
           // if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){lastTimeGetQueueRFID_Ro=xTaskGetTickCount();}
            if (giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan]==kvFille){lastTimeGetQueueRFID_Ro=xTaskGetTickCount();}
            else lastTimeGetQueueRFID_NV=xTaskGetTickCount();
    }
    /*
     * Nếu là Khu Phi Le thì chỉ dùng mã NV để quét
     * state_Running
     */
    if (stateMachine.deviceStatus == deviceRunning){
          if ((lastTimeGetQueueCan > lastTimeGetQueueRFID_Ro + 300)&&(lastTimeGetQueueRFID_Ro > 0)){ // chỉ nhận khi dữ liệu cân lớn hơn dữ liệu rfid 500 stick
               timeCompareMode1 = lastTimeGetQueueCan - lastTimeGetQueueRFID_Ro;
              if (timeCompareMode1 < time_2_lan_nhan_data){ // 2 dữ liệu phải nhỏ hơn thời gian cài đặt mới là 1 cặp đúng
              //  if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){
               if (giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan] == kvFille){
                    strncpy( Data_TH.id_RFID_NV,Data_RFID_NV.id_RFID, sizeof(Data_RFID_NV.id_RFID));
                    strncpy( Data_TH.id_RFID,"x", sizeof("x"));     
                }
                else {
                      strncpy( Data_TH.id_RFID,Data_RFID_TH.id_RFID, sizeof(Data_RFID_TH.id_RFID));
                      strncpy( Data_TH.id_RFID_NV,"x", sizeof("x"));
                }
                Data_TH.data_weight=Data_CAN_TH.data_can;
                boolean tt = true;
                if ((giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan] == kvSuaCa)&&(giaiDoanCan.cheDoInOut == cheDoOut)){
               // if (inforServer.PhanLoaiKV == PhanLoai::LANG_OUT){
                  /*
                   * nêu khác mã rổ thì không cần check cân
                   */
                    tt = false;
                  if (strcmp(Data_TH.id_RFID,idRFID_OLD) != 0){strncpy( idRFID_OLD,Data_TH.id_RFID, sizeof(Data_TH.id_RFID));if (Data_CAN_TH.data_can >0.5) tt = true;}
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
                   
                   // if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){       
                   
                   /* if ((giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan] != kvSuaCa)||(giaiDoanCan.cheDoInOut == cheDoOut)){
                        printf("CHECK OUT: Time: %ld - Kg: %f - RFID: %s \n",timeCompareMode1,Data_TH.data_weight,Data_TH.id_RFID);
                        xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                    }
                    else xSemaphoreGive(xreset_id_nv);   */
                    if ((giaiDoanCan.cheDoInOut == cheDoIN)&& (giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan]  == kvSuaCa)) {   
                      xSemaphoreGive(xreset_id_nv);  
                    }
                    else {
                        printf("CHECK OUT: Time: %ld - Kg: %f - RFID: %s \n",timeCompareMode1,Data_TH.data_weight,Data_TH.id_RFID);
                        xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                       // xSemaphoreGive(xSignal_Display_check);  
                    }      
                }
              //  xSemaphoreGive(xSignal_Display_check);     
                timeCompareMode1 = 10000;
                lastTimeGetQueueCan = 0;
                lastTimeGetQueueRFID_Ro = 0;
                lastTimeGetData_RoVaCan=xTaskGetTickCount();
              }
          }
          
          else if ((lastTimeGetQueueRFID_Ro >0)&&(xTaskGetTickCount()- lastTimeGetQueueRFID_Ro > time_2_lan_nhan_data)){
              printf("Over time: Ma Ro va Can \n");     
                lastTimeGetQueueRFID_Ro=0;
                xSemaphoreGive(xreset_id_nv);    
                if ((giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan] != kvSuaCa)||(giaiDoanCan.cheDoInOut == cheDoOut)){xSemaphoreGive(xSignal_Display_checkdone);}              
          }
          if ((giaiDoanCan.maGiaiDoan[giaiDoanCan.userSelecGiaiDoan] == kvSuaCa)&&(giaiDoanCan.cheDoInOut == cheDoIN)){ 
              if ((lastTimeGetQueueRFID_NV > lastTimeGetData_RoVaCan)&& (lastTimeGetData_RoVaCan >0)){ 
                  timeCompareMode2 = lastTimeGetQueueRFID_NV - lastTimeGetData_RoVaCan;
                  if (timeCompareMode2 < time_cho_nhan_RFID_NV){
                    lastTimeGetData_RoVaCan=lastTimeGetQueueRFID_NV;
                    strncpy( Data_TH.id_RFID_NV,Data_RFID_NV.id_RFID, sizeof(Data_RFID_NV.id_RFID));
                    printf("CHECK IN: Time: %ld - Kg: %f - RFID: %s - RFID NV: %s\n",timeCompareMode2,Data_TH.data_weight,Data_TH.id_RFID,Data_TH.id_RFID_NV);
                    xQueueSend( Queue_display, &Data_TH, xTicksToWait );
                    xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
                    timeCompareMode2=10000;
                    lastTimeGetQueueRFID_NV=0;
                    lastTimeGetData_RoVaCan = 0;
                  }
                  else  {
                    xSemaphoreGive(xSignal_Display_checkdone);
                    lastTimeGetQueueRFID_NV=0;
                    printf("TIme out check rfid nv \n");
                  } 
              }
              else if ((lastTimeGetData_RoVaCan >0)&&(xTaskGetTickCount()- lastTimeGetData_RoVaCan > time_cho_nhan_RFID_NV)){
                 xSemaphoreGive(xResetRfidMaRo); // Gửi tín hiệu để reset mã rỗ
                lastTimeGetData_RoVaCan=0;
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
