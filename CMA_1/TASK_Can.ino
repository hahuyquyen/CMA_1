uint8_t uart_bien[11];
double can_data=0;
double can_data_old=0;
void TaskCAN( void * pvParameters ){    
    const TickType_t xTicksToWait = pdMS_TO_TICKS(5);
    static Data_CAN Data_CAN;
    uint8_t _rfid_data[20];
    int tam=0;
    unsigned long time_send_para_can=0;
    for (;;){
      if (Serial1.available()){ 
       uint8_t incomingData = Serial1.read();
       if ( incomingData == 0x3D){tam=0;}
       else if ( incomingData == 0x0D) {
        if(tach(&can_data)){
                if(can_data!=Data_CAN.data_can){
                  Data_CAN.data_can=can_data;
                  Data_CAN.time_get=xTaskGetTickCount();
                  if(can_data > 0.1){xQueueSend( Queue_can, &Data_CAN, xTicksToWait );}
                  }//printf("CAN Nang : %f \n",can_data);
                else if (xTaskGetTickCount() - time_send_para_can > 2000){
                  time_send_para_can=xTaskGetTickCount();
                  xQueueSend( Queue_can, &Data_CAN, xTicksToWait );
                  }
         }
       }
       else {uart_bien[tam++]=incomingData;if(tam>10)tam=0;}  
     }
      vTaskDelay(25);   
    }
    vTaskDelete(NULL) ;
}

boolean tach(double* soky){
  int tam1=0;
  int hangtram=0;
  double soam= 1;
  *soky=0;
  for (int j=0;j<sizeof(uart_bien);j++){ if (uart_bien[j] != 0x20){tam1=j;break;} }
  for(int j=tam1;j<sizeof(uart_bien);j++){if(uart_bien[j] == 0x2E){hangtram=(j-tam1)-1;break;}}
  for(int j=tam1;j<sizeof(uart_bien);j++){
               //   Serial.print(uart_bien[j],HEX);
                 // Serial.print(" ");
                  if ((uart_bien[j] == 0x41)||(uart_bien[j] == 0x40)||(uart_bien[j] == 0x48)||(uart_bien[j] == 0x44)||(uart_bien[j] == 0x45)){*soky=0; return false;break;}//Serial.println(" ");
                  if ((uart_bien[j] == 0x43)||(uart_bien[j] == 0x47)||(uart_bien[j] == 0x42)||(uart_bien[j] == 0x46)){*soky = *soky * soam;return true;break;} // so on dinh
                  else if (uart_bien[j] == 0x2D){soam=-1;}
                  else if (uart_bien[j] == 0x2E){hangtram=hangtram+1;}
                  else *soky+=(double)((uart_bien[j] - 48)*pow(10,hangtram-(j-tam1)));          
   }
   return false;
}


