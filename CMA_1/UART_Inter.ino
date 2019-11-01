
volatile uint8_t can_rxbuf[40];
volatile uint8_t canbuff=0;
volatile uint8_t rfidbuff=0;
volatile uint8_t rfid_rxbuf[40];
volatile uint8_t rfid_data[20];
void memset_volatile(volatile void *s, uint8_t c, size_t n)
{
    /*volatile uint8_t *p = s;
    while (n-- > 0) {
        *p++ = c;
    }*/
    /*for (int i=0;i<n;i++){
      s[i]=' ';
    }*/
}

static void IRAM_ATTR uart1_intr_handle(void *arg)
{
  uint16_t rx_fifo_len;
  //status = UART0.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART0.status.rxfifo_cnt; // read number of bytes in UART buffer
  while(rx_fifo_len){
   canbuff=canbuff+1;
   if (canbuff>40){canbuff=0;}
   can_rxbuf[canbuff] = UART0.fifo.rw_byte; // read all bytes
   rx_fifo_len--;
   if ((can_rxbuf[canbuff]== 0x0A) && (can_rxbuf[canbuff-1] == 0x0D)&& (can_rxbuf[canbuff-21] == 0x02)&& (can_rxbuf[canbuff-20] == 0x01)){
    
    for (int ii=4;ii<21;ii++){
      rfid_data[ii-4]=can_rxbuf[ii];     
    }
    canbuff=0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
    
   }
   
 }
  uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);//clear UART interrupt status
 //uart_write_bytes(UART_NUM_0, (const char*)can_rxbuf, canbuff);
 
}
/*
static void IRAM_ATTR uart_intr_handle2(void *arg)
{
  uint16_t rx_fifo_len;
  rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer
  while(rx_fifo_len){
    
   rfid_rxbuf[rfidbuff++] = UART2.fifo.rw_byte; // read all bytes
   rx_fifo_len--;
   rfidbuff=rfidbuff+1;
   if (rfidbuff>40){rfidbuff=0;}
 }
  uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);//clear UART interrupt status
 //uart_write_bytes(UART_NUM_2, (const char*)rfid_rxbuf, 40);
 //memset_volatile(rfid_rxbuf, '\0',40); 
}

*/
void setting_uart(){
      /*
     * cai UART 1
     */
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_NUM_0, &uart_config);
  uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);//Install UART driver, and get the queue.
  uart_isr_free(UART_NUM_0);// release the pre registered UART handler/subroutine
  uart_isr_register(UART_NUM_0,uart1_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console_uart1); // register new UART subroutine
  uart_enable_rx_intr(UART_NUM_0); // enable RX interrupt

  /*
  uart_config_t uart2_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_NUM_2, &uart2_config);
  uart_set_pin(UART_NUM_2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);//Install UART driver, and get the queue.
  uart_isr_free(UART_NUM_2);// release the pre registered UART handler/subroutine
  uart_isr_register(UART_NUM_2,uart_intr_handle2, NULL, ESP_INTR_FLAG_IRAM, &handle_console_uart2); // register new UART subroutine
  uart_enable_rx_intr(UART_NUM_2); // enable RX interrupt
*/
  xCountingSemaphore = xSemaphoreCreateCounting( 10, 0 );
}
