/*
 * ngõ ra cân là rs232, tốc độ baud 9600, tần số 10hz
 * 
z Text Command
CR (0DH) The end of data
LF (0AH) The end of text
0 - 9 (30H-39H) Numeric data
- (2DH) Minus
. (2EH) Decimal Point
STX (02H) Start of Text
SOH (01H) Weight stable flag
NUL (00H) Weight un-stable flag 
0~9 là 30->39H
. 2EH
, 2CH
- 2DH 
stable flag là 01H-00H 01~Stable 00~unstable
Header :(0) 30H ->net weight
         (4) 34H->tare  
Net Weight = - 1.20 lb. Tare Weight = 0.45lb. 

STX* |Stable flag| CR |*Header |Net weight| CR| *Header| Tare weight|CR |LF
1          1        1      1           7     1     1          7       1   1 

Vd gửi dữ liệu
02 01 0D 30 34 35 36 2E 30 30 30 0D 34  32 31 30 2E 36 35 32 0D 0A
 */
 /*
  * Nhiều Chế độ truyền SEPC 52 bit 3
  * Enable header: SPEC 53 bit 0 bằng 1
  * SPEC 52 Bit 2 Send STX before text bằng 1 là send
  * Set 59 bit3 xuống 1 và 52 bit 3 bang 0 là khi master yêu cầu sẽ gửi thông tin
  * Chế độ này master sẽ gửi kí tự ACK (0x06) xuống để Cân gửi trả dữ liệu.
  * 3 chê do truyen
  * - 59 bit 3 =  1 và 52 bit 3 =0 là  bắt tay: master truyền ACK và cân sẽ trả datta
  * 59 bit 2 =1 và 52 bit 3 = 0 khi nhấn enter trên cân mới truyền data
  * 59 bit 2 = 0 thì cân sẽ tự động truyền
  */
/*extern volatile uint8_t can_rxbuf[40];
extern volatile uint8_t canbuff;
extern volatile uint8_t rfidbuff;
extern volatile uint8_t rfid_rxbuf[40];
extern uint8_t rfid_data[20];*/

void TaskCAN( void * pvParameters ){    
    const TickType_t xTicksToWait = pdMS_TO_TICKS(5);
    static data_user Data_task_CAN;
    Data_task_CAN.id = 1;
    uint8_t _rfid_data[20];
    void tachdata_can(uint8_t* datain,data_user* dataout);
    while(true){
      /*
      * Khi nhận được tín hiệu UART truyền qua queue
      * và task CAN  sẽ phân tích mà tách giá trị cân lưu vào struct đưa vào queue
      * uint8_t rfid_data[20];
      */
      if(xQueueReceive( Queue_can_interrup, &_rfid_data,  ( TickType_t ) 2 )== pdPASS ){
        tachdata_can(&_rfid_data[0],&Data_task_CAN);
        xQueueSend( Queue_can, &Data_task_CAN, xTicksToWait );
        Data_task_CAN.data_weight = 0;
        Data_task_CAN.data_tare = 0 ;
      }
      
     /*
      * Cho kiểu truyền dữ liệu của cân theo cơ chế bắt tay.
      * Khi nhận được tín hiệu thẻ RFID thì task RFID sẽ mở khóa xSignal_FromRFID
      * và task CAN nhận được chìa khóa sẽ gửi tín hiệu để cân gửi data về
      */
     if( xSemaphoreTake( xSignal_FromRFID, ( TickType_t ) 5 ) == pdTRUE ){
        printf("Gui tin hieu cho can send data\n");
     }
      vTaskDelay(20);
        
    }
    vTaskDelete(NULL) ;
}
void tachdata_can(uint8_t* datain,data_user* dataout){
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
