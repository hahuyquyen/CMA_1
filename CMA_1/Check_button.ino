volatile unsigned long DebounceTimer;
volatile int ButtonPressed;
volatile unsigned int delayTime = 100;
#define button_mode_ca 32
#define button_khuvuc 34
#define button_error 35
#define button_vitri 36
#define button_du_phong 39

void Check_button( void * pvParameters ){    
    pinMode(button_mode_ca, INPUT_PULLUP);
    pinMode(button_khuvuc, INPUT_PULLUP);
    pinMode(button_error, INPUT_PULLUP);
    pinMode(button_vitri, INPUT_PULLUP);
    pinMode(button_du_phong, INPUT_PULLUP);
    Serial.println(F("Start: Light Weight depounce blink without delay code"));
    attachInterrupt(digitalPinToInterrupt(pin), [] {if (ButtonPressed+= (millis() - DebounceTimer) >= (delayTime )) DebounceTimer = millis();}, RISING);
   
    for (;;){

      if (ButtonPressed> 0) {
        ButtonPressed = 0;
        Serial.println(Pressed );
      }
  
      vTaskDelay(25);   
    }
    vTaskDelete(NULL) ;
}

