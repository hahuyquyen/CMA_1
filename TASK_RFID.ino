/*
 * RFID mình dùng module JT-2850, giao tiếp uart
 */
//Calculate the checksum of the message and added as the last byte before sending it over
//to RFID reader
unsigned char CheckSum (unsigned char * uBuff, unsigned char uBuffLen)  
{  
  unsigned char i, uSum = 0;  
  for (i = 0; i <uBuffLen; i++)  
  {  
    uSum = uSum + uBuff [i];  
  }  
  uSum = (~ uSum) + 1;  
  return uSum;  
} 
void txrx(unsigned char * message, unsigned char messageLen)
{
 // mySerial.write(message, messageLen);
  uart_tx_chars(UART_NUM_2, (const char*)message, messageLen);
  vTaskDelay(10);
  uint16_t tam = UART2.status.txfifo_cnt;
  printf("bien tam %d \n",tam);

}
void TaskRFID( void * pvParameters ){
  // Reset reader command frame CODE
  // Sẽ trả A0 03 65 00 F8
    byte reset_message[] = {0xA0,0x03,0x65,0x00,0xF8};
    reset_message[4] = CheckSum(reset_message, 4);
    //Set reader to RS485 mode
    //1=>RS485
    //2=>Wiegand
    //3=>RS232
    /*
     * 0x72 Link selection for actively
        sending data after the reader
        reads the data
        1， 2， 3 1： RS485 link
        2： wiegand link
        3： RS232 link
     */
    byte com_mode_message[] = {0xA0,0x06,0x60,0x00,0x00,0x72,0x01,0x88};
    com_mode_message[7] = CheckSum(com_mode_message, 7);
    txrx(com_mode_message,8);
    
    //0x02 => Timer mode
    //other mode is cai eprom mode
    //0x01 => master slave mode
    byte mode_message[] = {0xA0,0x06,0x60,0x00,0x00,0x70,0x02,0x88};
    mode_message[7] = CheckSum(mode_message, 7);
    txrx(mode_message,8);
    //Tai lieu trang 15 set thong so luu trong eprom a0-06-60-device-eprom MS-epromLS - Gia tri - checjksum
    //0x71 Khoảng thời gian đọc giá trị 10-100-> số ms là x*10 Vd set 10 thì ms = 10*10 = 100ms (số HEX phải chuyển qua dex)
    //Set the timing to 1-sec (0x64 * 10=100*10) in ms, Range is 1-100 (10ms - 1sec) 
    byte timing_message[] = {0xA0,0x06,0x60,0x00,0x00,0x71,0x64,0x25};
    //This will set it to measure every 10 ms
    //byte timing_message[] = {0xA0,0x06,0x60,0x00,0x00,0x71,0x0A,0x7F};
    
    timing_message[7] = CheckSum(timing_message, 7);
    txrx(timing_message,8);
    
    //Send reset to apply the changes
    txrx(reset_message,5);
    
    Serial.println("Waiting for card..");
    while(true){
      unsigned long currentMillis=xTaskGetTickCount();
         vTaskDelay(10);
    }
}
