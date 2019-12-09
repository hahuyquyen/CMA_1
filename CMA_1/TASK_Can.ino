static uint8_t uart_bien[11];
static double can_data=0;
//////////////////////////////////////////////////////////////////
////// Task doc du lieu tu can DI28SS ////////////////////////////
//////////////////////////////////////////////////////////////////
void TaskCAN( void * pvParameters ){    
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
    Serial1.begin(9600, SERIAL_8N1, 26, 12); //12 tx 13 lÃ  rx(bau,se,rx,tx)
    static Data_CAN Data_CAN;
    int tam=0;
    unsigned long lastTimeSendCan=0;
    for (;;){
      if (Serial1.available()){ 
       uint8_t incomingData = Serial1.read();
       
       if ( incomingData == canStartByte){tam=0;}
       else if ( incomingData == canStopByte) {
        if(tach(&can_data)){
                if(can_data!=Data_CAN.data_can){
                  Data_CAN.data_can=can_data;
                  if(can_data > 0.5){xQueueSend( Queue_can, &Data_CAN, xTicksToWait );}
                }
                else if (xTaskGetTickCount() - lastTimeSendCan > 1000){
                  lastTimeSendCan=xTaskGetTickCount();
                  if(can_data > 0.5){xQueueSend( Queue_can, &Data_CAN, xTicksToWait );}
                }
         }
       }
       else {uart_bien[tam++]=incomingData;if(tam>10)tam=0;}  
     }
    // vTaskDelay(15); 
     vTaskDelayUntil(xTaskGetTickCount(),50);
    //  vTaskDelay(5000); 
    //  printf("Task CAN StackHigh %d, Free Heap = %d\n",uxTaskGetStackHighWaterMark(NULL),ESP.getFreeHeap());     
      /*
       Hien thi thong tin cua stack de chinh bo nho
      task nay can  2048 bo nho stack -> chỉnh 3072 dư 50%
       */
    }
    vTaskDelete(NULL) ;
}
//////////////////////////////////////////////////////////////////
////// Task so KG tu Data ////////////////////////////
//////////////////////////////////////////////////////////////////
boolean tach(double* soky){
  int tam1=0;
  int hangtram=0;
  double soam= 1;
  *soky=0;
  /*for (int j=0;j<sizeof(uart_bien);j++){
    Serial.print(uart_bien[j], HEX);Serial.print("-");
  }
  Serial.println("");*/
  for (int j=0;j<sizeof(uart_bien);j++){ if (uart_bien[j] != canNULLByte){tam1=j;break;} }
  for(int j=tam1;j<sizeof(uart_bien);j++){if(uart_bien[j] == can_dau_phay){hangtram=(j-tam1)-1;break;}}
  for(int j=tam1;j<sizeof(uart_bien);j++){
                  if ((uart_bien[j] == 0x41)||(uart_bien[j] == 0x40)||(uart_bien[j] == 0x48)||(uart_bien[j] == 0x44)||(uart_bien[j] == 0x45)){*soky=0; return false;break;}//Serial.println(" ");
                  if ((uart_bien[j] == 0x43)||(uart_bien[j] == 0x47)||(uart_bien[j] == 0x42)||(uart_bien[j] == 0x46)){*soky = *soky * soam;return true;break;} // so on dinh
                  else if (uart_bien[j] == can_Minus){soam=-1;}
                  else if (uart_bien[j] == can_dau_phay){hangtram=hangtram+1;}
                  else *soky+=(double)((uart_bien[j] - 48)*pow(10,hangtram-(j-tam1)));          
   }
   return false;
}


/*
 r 00
 u 10
 b 9600
 rs 00
 H 1
 */
