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


02 01 0D 30 34 35 36 2E 30 30 30 0D 34  32 31 30 2E 36 35 32 0D 0A
 */
extern volatile uint8_t can_rxbuf[40];
extern volatile uint8_t canbuff;
extern volatile uint8_t rfidbuff;
extern volatile uint8_t rfid_rxbuf[40];
extern volatile uint8_t rfid_data[20];
int tam= 1;
int hangtram=0;
void TaskCAN( void * pvParameters ){
  double giatri=0;
    while(true){
      xSemaphoreTake( xCountingSemaphore, portMAX_DELAY );
      for (int j=0;j<2;j++){
        giatri=0;
        tam=1;
        for (int tang=(j*9)+1;tang<(j*9)+8; tang++){
           if((rfid_data[tang] == 0x2C) || (rfid_data[tang] == 0x2E)) {hangtram = tang-1; break;}
        }
        hangtram=hangtram-1;
        for (int tang1=(j*9)+1;tang1<(j*9)+8; tang1++){
          if (rfid_data[tang1] == 0x2D){tam = -1;}
          else if ((rfid_data[tang1] == 0x2C) || (rfid_data[tang1] == 0x2E)){hangtram=hangtram+1;}
          else{
           giatri+=(float)((rfid_data[tang1]-48)*pow(10,hangtram-(tang1-1)));
            }
        }
        if (rfid_data[j*9] == 0x30){congnhan.data_weight=giatri*tam;}
        else{congnhan.data_tare=giatri*tam;} 
      }
      printf("Can weight: %f \n",congnhan.data_weight);
      printf("Can tare: %f \n", congnhan.data_tare);
        
      vTaskDelay(1000);
      /*giatri=0;
        tam=1;
        
        for (int tang=1;tang<8; tang++){
           if((rfid_data[tang] == 0x2C) || (rfid_data[tang] == 0x2E)) {hangtram = tang-1; break;}
        }
        printf("hangtram= %d \n",hangtram);
        hangtram=hangtram-1;
        for (int tang1=1;tang1<8; tang1++){
          if (rfid_data[tang1] == 0x2D){tam = -1;}
          else if ((rfid_data[tang1] == 0x2C) || (rfid_data[tang1] == 0x2E)){printf("GT %f \n",giatri);hangtram=hangtram+1;}
          else{
           giatri+=(rfid_data[tang1]-48)*pow(10,hangtram-(tang1-1));
           printf("ff %d  - %d\n",(rfid_data[tang1]-48),hangtram-(tang1-1));
            }
            printf("WW %f \n",giatri);
        }
        if (rfid_data[0] == 0x30){congnhan.data_weight=giatri*tam;}
        else{congnhan.data_tare=giatri*tam;} */
/*
           
        tam= 1;
        hangtram=0;
        giatri=0;
        for (int tang=10;tang<17; tang++){
           if((rfid_data[tang] == 0x2C) || (rfid_data[tang] == 0x2E)) {hangtram = tang-1; break;}
        }
        hangtram=hangtram-1;
        for (int tang1=10;tang1<17; tang1++){
          if (rfid_data[tang1] == 0x2D){tam = -1;}
          else if ((rfid_data[tang1] == 0x2C) || (rfid_data[tang1] == 0x2E)){printf("GT %f \n",giatri);hangtram=hangtram+1;}
          else{
           giatri+=(rfid_data[tang1]-48)*pow(10,hangtram-(tang1-1));
            } 
        }    
         if (rfid_data[9] == 0x30){congnhan.data_weight=giatri*tam;}
         else{congnhan.data_tare=giatri*tam;}    */
        
    }
}
