uint8_t uart_bien[11];
double can_data=0;
double can_data_old=0;
void TaskCAN( void * pvParameters ){    
    const TickType_t xTicksToWait = pdMS_TO_TICKS(5);
    static data_user Data_task_CAN;
    Data_task_CAN.id = 1;
    uint8_t _rfid_data[20];
    int tam=0;
    while(true){
      if (Serial1.available()){ 
       uint8_t incomingData = Serial1.read();
       if ( incomingData == 0x3D){tam=0;}
       else if ( incomingData == 0x0D) {
        if(tach(&can_data)){if (can_data!=can_data_old){can_data_old=can_data;printf("Can weight: %f \n",can_data_old);}}
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
                //  Serial.print(uart_bien[j],HEX);
                 // Serial.print(" ");
                  if ((uart_bien[j] == 0x41)||(uart_bien[j] == 0x40)||(uart_bien[j] == 0x47)||(uart_bien[j] == 0x44)||(uart_bien[j] == 0x43)||(uart_bien[j] == 0x47)){*soky=0;return false;break;}
                  if ((uart_bien[j] == 0x42)||(uart_bien[j] == 0x46)){*soky = *soky * soam;return true;break;} // so on dinh
                  else if (uart_bien[j] == 0x2D){soam=-1;}
                  else if (uart_bien[j] == 0x2E){hangtram=hangtram+1;}
                  else *soky+=(double)((uart_bien[j] - 48)*pow(10,hangtram-(j-tam1)));          
   }
   return false;
}

/*void tachdata_can(uint8_t* datain,data_user* dataout){
          double giatri=0;
          int hangtram=0;
          int tam= 1;
          for (int j=0;j<2;j++){
                giatri=0;
                tam=1;
                for (int tang=(j*9)+1;tang<(j*9)+8; tang++){
                   if((datain[tang] == can_dau_cham) || (datain[tang] == can_dau_phay)) {hangtram = tang-1; break;}
                }
                hangtram=hangtram-1;
                for (int tang1=(j*9)+1;tang1<(j*9)+8; tang1++){
                  if (datain[tang1] == can_Minus ){tam = -1;}
                  else if ((datain[tang1] == can_dau_cham) || (datain[tang1] == can_dau_phay)){hangtram=hangtram+1;}
                  else{
                   giatri+=(double)((datain[tang1]-48)*pow(10,hangtram-(tang1-1)));
                    }
                }
                if (datain[j*9] == can_header_weight ){dataout->data_weight=giatri*tam;}
                else{dataout->data_tare=giatri*tam;} 
          }
}

*/



/*
 * 
 * 
 * 
 * if(xQueueReceive( Queue_can_interrup, &_rfid_data,  ( TickType_t ) 2 )== pdPASS ){
        tachdata_can(&_rfid_data[0],&Data_task_CAN);
        for (int j=0;j<2;j++){
        giatri=0;
        tam=1;
        for (int tang=(j*9)+1;tang<(j*9)+8; tang++){
           if((_rfid_data[tang] == can_dau_cham) || (_rfid_data[tang] == can_dau_phay)) {hangtram = tang-1; break;}
        }
        hangtram=hangtram-1;
        for (int tang1=(j*9)+1;tang1<(j*9)+8; tang1++){
          if (_rfid_data[tang1] == can_Minus ){tam = -1;}
          else if ((_rfid_data[tang1] == can_dau_cham) || (_rfid_data[tang1] == can_dau_phay)){hangtram=hangtram+1;}
          else{
           giatri+=(double)((_rfid_data[tang1]-48)*pow(10,hangtram-(tang1-1)));
            }
        }
        if (_rfid_data[j*9] == can_header_weight ){Data_task_CAN.data_weight=giatri*tam;}
        else{Data_task_CAN.data_tare=giatri*tam;} 
      }
      xQueueSend( Queue_can, &Data_task_CAN, xTicksToWait );
     
      }
 */
