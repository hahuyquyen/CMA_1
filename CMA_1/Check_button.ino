//#include <driver/adc.h>


//EasyButton button_left(button_left_pin, 50, true);
EasyButton button_right(button_right_pin, 50, true);
EasyButton button_ok(button_ok_pin, 50, true);
EasyButton buttonExit(buttonExitPin, 50, true);
EasyButton buttonError(buttonErrorPin, 50, true);
//EasyButton buttonPower(pinReadPower, 500, true);
//////////////////////////////////////////////////////////////////
////// Button POWER ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onPressedPower() {
	// if (xTaskGetTickCount() > 1000) {
	 //  Serial.println("Turn OFF");
	digitalWrite(pinPower, LOW);
	stateMachine.deviceStatus = deviceTurnOff;
	variLcdUpdate.updateLCD = true;
	// }
}
//////////////////////////////////////////////////////////////////
////// Button LEFT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/*
void onPressed_left() {
  if (stateMachine.deviceStatus == deviceSetting){
		if ((stateMachine.bottonSelect == 0) && (inforServer.giaiDoan.cheDoInOut > 0)) {
		  inforServer.changeData(false, &inforServer.giaiDoan.cheDoInOut);
		}
		else if ((stateMachine.bottonSelect == 1) && (inforServer.giaiDoan.userSelect > 0)) {
		  inforServer.changeData(false, &inforServer.giaiDoan.userSelect);
		}
		else if ((stateMachine.bottonSelect == 2) && (inforServer.nhaCC.userSelect > 0)) {
		  inforServer.changeData(false, &inforServer.nhaCC.userSelect);
		}
		else if ((stateMachine.bottonSelect == 3) && (inforServer.thanhPham.userSelect > 0)) {
		  inforServer.changeData(false, &inforServer.thanhPham.userSelect);
		}
		variLcdUpdate.stateDisplayLCD = 1;
		variLcdUpdate.numScroll = 0;
		variLcdUpdate.updateLCD = true;
  }
  else if (stateMachine.deviceStatus == deviceRunning){
	if (variLcdUpdate.stateDisplayLCD == 5){
	  variLcdUpdate.stateDisplayLCD = 1;
	}
	else    variLcdUpdate.stateDisplayLCD = 2;

  }
#ifdef debug_Web
  DebugData("Button - onPressed Left");
#endif
}
*/
//////////////////////////////////////////////////////////////////
////// Button RIGHT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onPressed_right() {
	/* if (stateMachine.bottonSelect == 0){ inforServer.giaiDoan.cheDoInOut = (inforServer.giaiDoan.cheDoInOut > 1) ? 0 : (inforServer.giaiDoan.cheDoInOut + 1);}
	  else if ((stateMachine.bottonSelect == 1)&&(inforServer.giaiDoan.total > 0)){ inforServer.giaiDoan.userSelect = (inforServer.giaiDoan.userSelect > (inforServer.giaiDoan.total-1)) ? 0 : (inforServer.giaiDoan.userSelect + 1);}
	  else if ((stateMachine.bottonSelect == 2)&&(inforServer.nhaCC.total > 0)){inforServer.nhaCC.userSelect = (inforServer.nhaCC.userSelect > (inforServer.nhaCC.total - 1)) ? 0 : (inforServer.nhaCC.userSelect + 1);}
	  else if ((stateMachine.bottonSelect == 3)&&(inforServer.thanhPham.total > 0)){inforServer.thanhPham.userSelect = (inforServer.thanhPham.userSelect > (inforServer.thanhPham.total-1)) ? 0 : (inforServer.thanhPham.userSelect + 1);}*/
	  /* if (stateMachine.bottonSelect == 0) {
		  inforServer.changeData(true, &inforServer.giaiDoan.cheDoInOut, (uint8_t) 2);
		}
		else if ((stateMachine.bottonSelect == 1) && (inforServer.giaiDoan.total > 0)) {
		  inforServer.changeData(true, &inforServer.giaiDoan.userSelect, inforServer.giaiDoan.total);
		}
		else */
	if ((stateMachine.bottonSelect == 2) && (inforServer.nhaCC.total > 0)) {
		inforServer.changeData(true, &inforServer.nhaCC.userSelect, inforServer.nhaCC.total);
	}
	else if ((stateMachine.bottonSelect == 3) && (inforServer.thanhPham.total > 0)) {
		inforServer.changeData(true, &inforServer.thanhPham.userSelect, inforServer.thanhPham.total);
	}
	if (stateMachine.deviceStatus == deviceSetting) { variLcdUpdate.stateDisplayLCD = 1; }
	variLcdUpdate.numScroll = 0;
	variLcdUpdate.updateLCD = true;
#ifdef debug_Web
	DebugData("Button - onPressed Right");
#endif
}
//////////////////////////////////////////////////////////////////
////// Button OK ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onPressed_ok() {
	/* if ((stateMachine.bottonSelect == 0) && (inforServer.giaiDoan.cheDoInOut == 0)) return;
	 else if ((stateMachine.bottonSelect == 1) && (inforServer.giaiDoan.userSelect == 0)) return;
	 else */

	if ((stateMachine.bottonSelect == 2) && (inforServer.nhaCC.userSelect == 0)) return;
	else if ((stateMachine.bottonSelect == 3) && (inforServer.thanhPham.userSelect == 0)) return;
	if (stateMachine.bottonSelect > 6) return;
	if (stateMachine.bottonSelect == 0) {
		if (stateMachine.giaidoanINOUT == cheDoOut) {
			stateMachine.bottonSelect = 5;
		}
		else stateMachine.bottonSelect = 2;
	}
	else stateMachine.bottonSelect = stateMachine.bottonSelect + 1;
	/*- if  ((stateMachine.bottonSelect == 1) && (inforServer.giaiDoan.cheDoInOut == cheDoOut )) {
	   stateMachine.bottonSelect = 3;
	 }
	 else {*/

	 // }

	if (stateMachine.bottonSelect > 4) {
		stateMachine.deviceStatus = deviceRunning;
	}
	if (stateMachine.deviceStatus == deviceSetting) { variLcdUpdate.stateDisplayLCD = 1; }
	variLcdUpdate.numScroll = 0;
	variLcdUpdate.updateLCD = true;
}
//////////////////////////////////////////////////////////////////
////// Button EXit ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onPressedExit() {
	stateMachine.bottonSelect = 0;
	stateMachine.deviceStatus = deviceSetting;
#ifdef debug_UART
	// Serial.println("onPressed_vitri");
#endif
#ifdef debug_Web
	DebugData("Button - onPressed Exit");
#endif
	////////////////////////////////////////////////
	//b Reset de nhan cac thong so lai /////////////
	///////////////////////////////////////////////
	khoiTaoGiaTri(false);
	if (stateMachine.deviceStatus == deviceSetting) { variLcdUpdate.stateDisplayLCD = 1; }
	variLcdUpdate.updateLCD = true;
}
//////////////////////////////////////////////////////////////////
////// Button Error ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onPressedError() {
#ifdef debug_UART
	//Serial.println("onPressed");
#endif
#ifdef debug_Web
	DebugData("Button - onPressed_Error");
#endif
	if (stateMachine.deviceStatus == deviceSetting) {
		if (variLcdUpdate.stateDisplayLCD == 5) {
			variLcdUpdate.stateDisplayLCD = 1;
		}
		else    variLcdUpdate.stateDisplayLCD = 2;
	}
}
//////////////////////////////////////////////////////////////////
////// Task Check Button /////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void Check_button(void* pvParameters) {
	pinMode(pinReadPower, INPUT_PULLUP);
	// button_left.begin();
	button_right.begin();
	buttonExit.begin();
	button_ok.begin();
	buttonError.begin();
	// buttonPower.begin();
	 //button_left.onPressedFor(80, onPressed_left);
	button_right.onPressedFor(30, onPressed_right);
	buttonExit.onPressedFor(30, onPressedExit);
	button_ok.onPressedFor(30, onPressed_ok);
	buttonError.onPressedFor(30, onPressedError);
	//uttonPower.onPressedFor(700, onPressedPower);
	pinMode(pinReadPower, INPUT_PULLUP);
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	// pinMode(pinAnalogPower, INPUT);
	unsigned long lastTimeReadADC = 0;
	unsigned long lastTimeGetRSSI = 0;
	//int powervalue;
	/*while (digitalRead(pinReadPower) == LOW){
  #ifdef debug_UART
	  //Serial.println("Wait ");
  #endif
  #ifdef debug_Web
		DebugData("Wait");
  #endif
	  delay(50);
	}*/
	// statusPeripheral.powerValue= 200;
	// adc1_config_width(ADC_WIDTH_BIT_12);
	// adc1_config_channel_atten( ADC1_CHANNEL_5, ADC_ATTEN_11db );
	for (;;) {

		if (stateMachine.deviceStatus == deviceSetting) {

			button_right.read();
			button_ok.read();
		}
		// button_left.read();
		buttonExit.read();
		buttonError.read();
		// buttonPower.read();


	 ///////////////////////////////////////
	 // 5s get Power 1 lan ////////////////
	  //////////////////////////////////////
		if (xTaskGetTickCount() - lastTimeReadADC > 5000) {
			lastTimeReadADC = xTaskGetTickCount();
			//  statusPeripheral.powerValue = adc1_get_raw( ADC1_CHANNEL_5);
		   //   statusPeripheral.powerValue = map(statusPeripheral.powerValue, 2000, 4096, 0, 100); // Can Map dien ap tu 2.6->3.3 la 0-<>100
#ifdef debug_UART          
#endif
#ifdef debug_Web
	   //   DebugData("Power : %d", statusPeripheral.powerValue);
#endif

		}
		///////////////////////////////////
		//////// 30s get RSSI ////////////
		/////////////////////////////////
		if ((xTaskGetTickCount() - lastTimeGetRSSI > 30000) || (lastTimeGetRSSI == 0)) {
			lastTimeGetRSSI = xTaskGetTickCount();
			if (statusPeripheral.wifi.statusConnectAP == false) statusPeripheral.rssiWifi = getRSSI(WiFiConf.sta_ssid); //ham nay thuc hien lau nen chi khi mat ket noi moi do
			else statusPeripheral.rssiWifi = WiFi.RSSI();
		}
		///////////////////////////////////
		//////// delay 50 stick ////////////
		/////////////////////////////////
		vTaskDelayUntil(&xLastWakeTime, 50);
	}
	vTaskDelete(NULL);
}


/*  for (int tangTam = 0 ; tangTam < inforServer.thanhPham.total ; tangTam ++){
				 inforServer.userSelectThanhPham = inforServer.userSelectThanhPham - 1;
				if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){
					if (inforServer.userSelectThanhPham == 0) break;
					if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 1) {break;}
				}
				else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){
					if (inforServer.userSelectThanhPham == 0) break;
					if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 2) {break;}
				}
  }*/
