//#include <driver/adc.h>


EasyButton button_right(button_right_pin, 50, true);
EasyButton button_ok(button_ok_pin, 50, true);
EasyButton buttonExit(buttonExitPin, 50, true);

//////////////////////////////////////////////////////////////////
////// Button POWER ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onPressedPower() {
	digitalWrite(pinPower, LOW);
	stateMachine.deviceStatus = deviceTurnOff;
	variLcdUpdate.updateLCD = true;
}
//////////////////////////////////////////////////////////////////
////// Button RIGHT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onPressed_right() {
	if ((stateMachine.bottonSelect == stateChooseNhaCC) 
		&& (inforServer.nhaCC.total > 0)) 
	{
		inforServer.changeData(true, &inforServer.nhaCC.userSelect, inforServer.nhaCC.total);
	}
	else if ((stateMachine.bottonSelect == stateChooseThanhPham)
		&& (inforServer.thanhPham.total > 0)) 
	{
		inforServer.changeData(true, &inforServer.thanhPham.userSelect, inforServer.thanhPham.total);
	}
	if (stateMachine.deviceStatus == deviceSetting) { variLcdUpdate.stateDisplayLCD = 1; }
	//variLcdUpdate.numScroll = 0;
	variLcdUpdate.updateLCD = true;
#ifdef debug_Web
	DebugData("Button - onPressed Right");
#endif
}
//////////////////////////////////////////////////////////////////
////// Button OK ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onPressed_ok() {
	if ((stateMachine.bottonSelect == stateChooseNhaCC) && (inforServer.nhaCC.userSelect == 0)) return;
	else if ((stateMachine.bottonSelect == stateChooseThanhPham) && (inforServer.thanhPham.userSelect == 0)) return;
	if (stateMachine.bottonSelect > 6) return;
	if (stateMachine.bottonSelect == 0) {
		if (stateMachine.giaidoanINOUT == cheDoOut) {
			stateMachine.bottonSelect = stateChooseRunning;
		}
		else stateMachine.bottonSelect = stateChooseNhaCC;
	}
	else stateMachine.bottonSelect = stateMachine.bottonSelect + 1;
	// Bo qua buoc chon confirm 
	if (stateMachine.bottonSelect == stateChooseConfirm) { stateMachine.bottonSelect = stateChooseRunning; }
	
	if (stateMachine.bottonSelect > 4) { 
		stateMachine.deviceStatus = deviceRunning;
	}
	if (stateMachine.deviceStatus == deviceSetting) { variLcdUpdate.stateDisplayLCD = 1; }
	//variLcdUpdate.numScroll = 0;
	variLcdUpdate.updateLCD = true;
}
//////////////////////////////////////////////////////////////////
////// Button EXit //////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void onPressedExit() {
	stateMachine.bottonSelect = 0;
	stateMachine.deviceStatus = deviceSetting;
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
////// Button Error /////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/*void onPressedError() {
#ifdef debug_Web
	DebugData("Button - onPressed_Error");
#endif
	if (stateMachine.deviceStatus == deviceSetting) {
		if (variLcdUpdate.stateDisplayLCD == 5) {
			variLcdUpdate.stateDisplayLCD = 1;
		}
		else    variLcdUpdate.stateDisplayLCD = 2;
	}
}*/
//////////////////////////////////////////////////////////////////
////// Task Check Button /////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void Check_button(void* pvParameters) {
	pinMode(pinReadPower, INPUT_PULLUP);
	button_right.begin();
	buttonExit.begin();
	button_ok.begin();
	button_right.onPressedFor(30, onPressed_right);
	buttonExit.onPressedFor(30, onPressedExit);
	button_ok.onPressedFor(30, onPressed_ok);
	pinMode(pinReadPower, INPUT_PULLUP);
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	unsigned long lastTimeReadADC = 0;
	unsigned long lastTimeGetRSSI = 0;
	for (;;) {

		if (stateMachine.deviceStatus == deviceSetting) {
			button_right.read();
			button_ok.read();
		}
		buttonExit.read();
	 ///////////////////////////////////////
	 // 5s get Power 1 lan ////////////////
	  //////////////////////////////////////
		if (xTaskGetTickCount() - lastTimeReadADC > 5000) {
			lastTimeReadADC = xTaskGetTickCount();
			//  statusPeripheral.powerValue = adc1_get_raw( ADC1_CHANNEL_5);
		   //   statusPeripheral.powerValue = map(statusPeripheral.powerValue, 2000, 4096, 0, 100); // Can Map dien ap tu 2.6->3.3 la 0-<>100
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