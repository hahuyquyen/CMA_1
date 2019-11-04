/*
 * CPU chạy 240mhz
 * vTaskDelay (x)  x là số stick muốn trì hoãn
 * Trong freertos nên dùng hàm delay của RTOS thay vì của arduino
 * Vì khi dùng delay rtos thì task sẽ được chuyển đổi trong delay
 * 
 * 
 */



void Display( void * pvParameters ){
    display_NV Display_NV_TASK;
    boolean status_led= true;
    pinMode(2, OUTPUT);
    unsigned long _time_counting_task_display=0;
    uint16_t Time_blink= 1000;
    while(true){
      xQueueReceive( Queue_Time_blink, &Time_blink,  ( TickType_t ) 2 );
      if (xTaskGetTickCount()- _time_counting_task_display > Time_blink){
        _time_counting_task_display = xTaskGetTickCount();
        status_led=!status_led;
        digitalWrite(2,status_led);
        printf("Free Heap %d\n",ESP.getFreeHeap());
      }
      if(xQueueReceive( Queue_display, &Display_NV_TASK,  ( TickType_t ) 2 )== pdPASS ){
        printf("Hien thi nhan vien \n");
      }
      vTaskDelay(30);
    }
    vTaskDelete(NULL) ;
}

/*
 * 
 * 
short int 2 -32,768 to 32,767 %hd
unsigned short int  2 0 to 65,535 %hu
unsigned int  4 0 to 4,294,967,295  %u
int 4 -2,147,483,648 to 2,147,483,647 %d
long int  4 -2,147,483,648 to 2,147,483,647 %ld
unsigned long int 4 0 to 4,294,967,295  %lu
long long int 8 -(2^63) to (2^63)-1 %lld
unsigned long long int  8 0 to 18,446,744,073,709,551,615 %llu
signed char 1 -128 to 127 %c
unsigned char 1 0 to 255  %c
float 4   %f
double  8   %lf
long double 12    %Lf
We can use the sizeof() operator to check the size of a variable. See the following C program for
 */
