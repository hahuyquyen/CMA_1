/*#include <HardwareSerial.h>
3D-20-20-20-30-2E-30-30-43-D-
HardwareSerial MySerial(1);
*/
uint8_t uart_bien[11];
double can_data=0;
double can_data_old=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial1.begin(9600, SERIAL_8N1, 13, 12); //12 tx 13 là rx
}
double soky=0;
int tam=0;
void loop() {
    if (Serial1.available())
      { 
       uint8_t incomingData = Serial1.read();
       if ( incomingData == 0x3D){tam=0;}
       else if ( incomingData == 0x0D) {
        can_data = tach();
        if (can_data!=can_data_old){
          can_data_old=can_data;
             printf("Can weight: %f \n",can_data_old);
        }
       }
       else {
        uart_bien[tam]=incomingData;
        tam++;
        if(tam>10)tam=0;
       }
        
     }
     
}
double tach(){
  int tam1=0;
  int hangtram=0;
  int soam= 1;
  double soky=0;
  for (int j=0;j<sizeof(uart_bien);j++){ if (uart_bien[j] != 0x20){tam1=j;break;} }
  for(int j=tam1;j<sizeof(uart_bien);j++){if(uart_bien[j] == 0x2E){hangtram=(j-tam1)-1;break;}}
  for(int j=tam1;j<sizeof(uart_bien);j++){
                  if ((uart_bien[j] == 0x41)||(uart_bien[j] == 0x40)||(uart_bien[j] == 0x42)||(uart_bien[j] == 0x43)){break;}
                  else if (uart_bien[j] == 0x2D){soam=-1;}
                  else if (uart_bien[j] == 0x2E){hangtram=hangtram+1;}
                  else soky+=(double)((uart_bien[j] - 48)*pow(10,hangtram-(j-tam1)));          
   }
   soky=soky*soam;
 
   return soky;
}

