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
    while(true){
    if(xQueueReceive( Queue_can, &Data_CAN_TH,  ( TickType_t ) 2 )== pdPASS ){
      _time_get_data_can=xTaskGetTickCount();
    }
    if(xQueueReceive( Queue_RFID, &Data_RFID_TH,  ( TickType_t ) 2 )== pdPASS ){
      _time_get_data_rfid=xTaskGetTickCount();
    }
    if(xQueueReceive( Queue_RFID_NV, &Data_RFID_NV,  ( TickType_t ) 2 )== pdPASS ){
      _time_get_data_rfid_nv=xTaskGetTickCount();
    }
    if (_time_get_data_can > _time_get_data_rfid + 1000){
        if (_time_get_data_rfid > 0){ _time_timeout_data = _time_get_data_rfid > _time_get_data_can?_time_get_data_rfid - _time_get_data_can:_time_get_data_can - _time_get_data_rfid;}
        if (_time_timeout_data < time_2_lan_nhan_data){
          strncpy( Data_TH.id_RFID,Data_RFID_TH.id_RFID, sizeof(Data_RFID_TH.id_RFID));
          Data_TH.data_weight=Data_CAN_TH.data_can;
          strncpy( Data_TH.id_RFID_NV,"x", sizeof("x"));
          xQueueSend( Queue_display, &Data_TH, xTicksToWait );
          if (status_IN_or_OUT){          
          printf("CHECK OUT: Time: %ld - Kg: %f - RFID: %s \n",_time_timeout_data,Data_TH.data_weight,Data_TH.id_RFID);
          xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
          }
          else {
            //send reset ID NV
            xSemaphoreGive(xreset_id_nv);
                
          }
          xSemaphoreGive(xSignal_Display_check);     
          _time_timeout_data = 10000;
          _time_get_data_can = 0;
          _time_get_data_rfid = 0;
          _time_get_tam=xTaskGetTickCount();
        }
    }
    if (!status_IN_or_OUT){   
        if ((_time_get_data_rfid_nv > _time_get_tam)&& (_time_get_tam >0)){ _time_timeout_data_VAO = _time_get_data_rfid_nv - _time_get_tam;
            if (_time_timeout_data_VAO < time_cho_nhan_RFID_NV){
              _time_get_tam=_time_get_data_rfid_nv;
              strncpy( Data_TH.id_RFID_NV,Data_RFID_NV.id_RFID, sizeof(Data_RFID_NV.id_RFID));
              printf("CHECK IN: Time: %ld - Kg: %f - RFID: %s - RFID NV: %s\n",_time_timeout_data_VAO,Data_TH.data_weight,Data_TH.id_RFID,Data_TH.id_RFID_NV);
              xQueueSend( Queue_display, &Data_TH, xTicksToWait );
              xQueueSend( Queue_mqtt, &Data_TH, xTicksToWait );
              _time_timeout_data_VAO=10000;
              _time_get_data_rfid_nv=0;
            //  _time_get_tam=0;
            }
            else  {
              xSemaphoreGive(xSignal_Display_checkdone);
              _time_get_data_rfid_nv=0;
              printf("TIme out check rfid nv \n");
            } 
        } 
        else if (_time_get_tam >0){     
          if (xTaskGetTickCount()- _time_get_tam > time_cho_nhan_RFID_NV){
          _time_get_tam=0;
          printf("Over time \n");      
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

