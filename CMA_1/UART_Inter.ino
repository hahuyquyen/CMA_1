

void memset_volatile(volatile void* s, uint8_t c, size_t n)
{
	/*volatile uint8_t *p = s;
	while (n-- > 0) {
		*p++ = c;
	}*/
	/*for (int i=0;i<n;i++){
	  s[i]=' ';
	}*/
}
/*
static void IRAM_ATTR uart1_intr_handle(void *arg)
{ volatile uart_dev_t *uart = &UART0;
  while(uart->status.rxfifo_cnt){
   canbuff=canbuff+1;
   if (canbuff>40){canbuff=0;}
   can_rxbuf[canbuff] = uart->fifo.rw_byte; // read all bytes
   if ((can_rxbuf[canbuff]== can_LF) && (can_rxbuf[canbuff-1] == can_CR)&& (can_rxbuf[canbuff-21] == 0x02)&& (can_rxbuf[canbuff-20] == 0x01)){
	for (int ii=4;ii<21;ii++){
	  rfid_data[ii-4]=can_rxbuf[ii];
	}
	canbuff=0;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueSendFromISR ( Queue_can_interrup, &rfid_data, &xHigherPriorityTaskWoken);
  //  xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
   }
 }
  uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);//clear UART interrupt status
}
void setting_uart(){
  uart_config_t uart_config = {
	.baud_rate = uart_can_baud_rate,
	.data_bits = UART_DATA_8_BITS,
	.parity = UART_PARITY_DISABLE,
	.stop_bits = UART_STOP_BITS_1,
	.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(uart_can_num, &uart_config);
  uart_set_pin(uart_can_num, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(uart_can_num, BUF_SIZE_UART * 2, 0, 0, NULL, 0);//Install UART driver, and get the queue.
  uart_isr_free(uart_can_num);// release the pre registered UART handler/subroutine
  uart_isr_register(uart_can_num,uart1_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console_uart1); // register new UART subroutine
  uart_enable_rx_intr(uart_can_num); // enable RX interrupt
  //gpio_set_pull_mode(BUTTON1, GPIO_PULLUP_ONLY);
  xCountingSemaphore = xSemaphoreCreateCounting( 10, 0 );
  xSignal_FromRFID = xSemaphoreCreateCounting( 10, 0 );
}
*/
