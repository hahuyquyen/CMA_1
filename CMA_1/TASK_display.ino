void sendSSID485() {
	modbusData.dataTruyen[0] = WiFi.localIP()[0];
	modbusData.dataTruyen[1] = WiFi.localIP()[1];
	modbusData.dataTruyen[2] = WiFi.localIP()[2];
	modbusData.dataTruyen[3] = WiFi.localIP()[3];
	mb.senDataToDevice(rs485HMIAddresIP, modbusData.dataTruyen, 4);
	mb.send485Utf16(rs485HMIAddresSSID, WiFiConf.sta_ssid, 16);
}

void send485PageAndData(uint16_t page, boolean dataSend) {
	uint8_t numByte = 1;
	modbusData.dataTruyen[0] = page;
	modbusData.dataTruyen[1] = 0;
	if (dataSend) {
		modbusData.dataTruyen[2] = (uint16_t)stateMachine.hardwareId;
		modbusData.dataTruyen[3] = (uint16_t)stateMachine.hardwareId >> 16;
		modbusData.dataTruyen[4] = (uint16_t)stateMachine.giaidoanINOUT;
		modbusData.dataTruyen[5] = (uint16_t)stateMachine.giaidoanKV;
		numByte = 6;
	}
	mb.senDataToDevice(rs485HMIAddresPage, modbusData.dataTruyen, numByte);
}
void send485PageKg(uint16_t page, uint32_t kg) {
	modbusData.dataTruyen[0] = (uint16_t)kg;
	modbusData.dataTruyen[1] = (uint16_t)kg >> 16;
	modbusData.dataTruyen[2] = page;
	mb.senDataToDevice(rs485HMIAddresKgPopup, modbusData.dataTruyen, 3);
}
void send485Kg(uint32_t kg) {
	modbusData.dataTruyen[0] = (uint16_t)kg;
	modbusData.dataTruyen[1] = (uint16_t)kg >> 16;
	mb.senDataToDevice(rs485HMIAddressKg, modbusData.dataTruyen, 2);
}
uint16_t getstatuswifi() {
	return mb.readDataHMI(rs485HMIAddresStatusApMode,1);
}
void setstatuswifi() {
	uint16_t data = 0;
	mb.senDataToDevice(rs485HMIAddresStatusApMode, &data, 1);
}
void send485NameTP() {
	mb.send485Utf16(rs485HMIAddresNameThanhPham, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], 96);

};
void send485NameNhaCC() {
	mb.send485Utf16(rs485HMIAddresNameNhaCC, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], 32);

}
void LcdSeclectMode(uint8_t modeDisplay, Data_TH* dataLCDTH) {
	if (mb.slave()) { return; }
	switch (modeDisplay) {
	case statusDisplayPopup: //popup
		if (statusPeripheral.mqtt.updateName) { mb.send485Utf16(rs485HMIAddresNameNhanVien, statusPeripheral.mqtt.nameNhanVien, 28); }
		char textTam[12];
		memset(textTam, 0x00, sizeof(textTam));
		if (dataLCDTH->id_RFID_NV[0] != 'x') { memcpy(textTam, &dataLCDTH->id_RFID_NV[16], 8); textTam[9] = '\0'; }
		else memcpy(textTam, "x", 1);
		mb.send485char(rs485HMIAddresRFIDNV, textTam, 6);
		memset(textTam, 0x00, sizeof(textTam));
		if (dataLCDTH->id_RFID[0] != 'x') { memcpy(textTam, &dataLCDTH->id_RFID[16], 8); textTam[9] = '\0'; }
		else memcpy(textTam, "x", 1);
		mb.send485char(rs485HMIAddresRFIDRo, textTam, 6);
		send485PageKg(pagePopup, (uint32_t)(dataLCDTH->data_weight * 1000));
		break;
	case statusDisplayRunning: //running
		if (statusPeripheral.mqtt.updateName) {
			statusPeripheral.mqtt.updateName = false;
			mb.send485Utf16(rs485HMIAddresNameNhanVien, "\0", 28); // reset ten nv 
		}
		send485NameTP();
		send485NameNhaCC();
		send485PageAndData(pageRunning, false);
		break;
	case statusDisplayInfor: //infor bat dau
		send485PageAndData(pageInfor, true);
		break;
	case 3: //infor bat dau
		send485PageAndData(pageInfor, true);
		break;
	case statusDisplayNhaCC: //nha cung cap
		send485NameNhaCC();
		send485PageAndData(pageNhaCC, false);
		break;
	case statusDisplayThanhPham: //thanh pham inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect] 
		send485NameTP();
		send485PageAndData(pageThanhPham, false);
		break;
	case statusDisplayXacNhan: // xac nhan
		send485NameNhaCC();
		send485NameTP();
		send485PageAndData(pageComfirm, false);
		break;
	default:
		break;
	}
}
/*
*/

void Display(void* pvParameters) {
	boolean status_led = true;
	struct Data_TH dataDisplayTH;
	unsigned long lastTimeBlinkLed = 0;
	unsigned long timeoutDisplay = 0;
	unsigned long lastBlinkLCD = 0;
	uint16_t Time_blink = 1000;
	uint16_t Time_check = 2500;
	//Serial1.begin(38400, SERIAL_8N1, 26, 12); //12 tx 13 lÃ  rx(bau,se,rx,tx)
	mb.begin(&Serial);
	mb.master();
	LcdSeclectMode(statusDisplayInfor, &dataDisplayTH);
	variLcdUpdate.stateDisplayLCD = statusDisplayRunning;
	//uint8_t daucham_lcd = 0;
	boolean statusBuzzer = false;
	digitalWrite(pinLedGreen, HIGH);
	digitalWrite(pinLedRed, HIGH);
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	//uint16_t mahinh = 10;

	for (;;) {

		if ((xTaskGetTickCount() - modbusData.timeSendSSID > 20000) || (modbusData.timeSendSSID == 0)) {
			modbusData.timeSendSSID = xTaskGetTickCount();
			sendSSID485();
		}
		else  if ((xTaskGetTickCount() - modbusData.timeSendKg > 800) || (modbusData.timeSendSSID == 0)) {
			modbusData.timeSendKg = xTaskGetTickCount();
			send485Kg((can_data > 0) ? ((uint32_t)(can_data * 1000)) : 0);
			uint16_t statuswwifi = getstatuswifi();
			if (statuswwifi == 1) {
				//if (statusPeripheral.wifi.statusConnectAP == false) { // khong ket noi wifi
				wifi_connect(1, WIFI_AP, WiFiConf.sta_ssid, WiFiConf.sta_pwd,(char *)"ESP32", true);
#ifdef debug_Web
					DebugData("Set AP  %d ", statuswwifi);
#endif
				//}
				setstatuswifi();
			}
		}
		switch (stateMachine.deviceStatus) {
			//////////////////////////////////////////////////////////////////
		case deviceRunning: //////////////////////////////////////////////
		  //////////////////////////////////////////////////////////////////
			if (xQueueReceive(Queue_display, &dataDisplayTH, (TickType_t)1) == pdPASS) {
				variLcdUpdate.stateDisplayLCD = statusDisplayPopup;  //popup man hinh
				variLcdUpdate.updateLCD = true;  // popup tuc thi
				if ((dataDisplayTH.id_RFID_NV[0] == 'x') && (GetSttKhuVuc() == sttKvSuaCaIN)) {
#ifdef debug_UART
					Serial.println("Data : Ro & Can KV Sua Ca - IN");
#endif
#ifdef debug_Web
					DebugData("Data : Ro & Can KV Sua Ca - IN");
#endif
					digitalWrite(pinLedGreen, LOW);
				}
				else if (GetSttKhuVuc() == sttKvSuaCaIN) {
#ifdef debug_Web
					DebugData("Da Nhan du thong tin");
#endif
					timeoutDisplay = xTaskGetTickCount();
					statusBuzzer = true;
					digitalWrite(pinBuzzer, statusBuzzer);
				}
				else {
					timeoutDisplay = xTaskGetTickCount();
					statusBuzzer = true;
					digitalWrite(pinBuzzer, statusBuzzer);
				}
			}
 
			if (xSemaphoreTake(xSignal_Display_checkdone, 1)) { //Che do IN qua timeout se tat
				variLcdUpdate.stateDisplayLCD = statusDisplayRunning;
				variLcdUpdate.updateLCD = true;  // 
				statusBuzzer = false;
				digitalWrite(pinBuzzer, statusBuzzer);
				digitalWrite(pinLedGreen, HIGH);
				timeoutDisplay = 0;
			}
			if ((xTaskGetTickCount() - timeoutDisplay > 500) && (timeoutDisplay > 0) && (statusBuzzer)) {
				statusBuzzer = false;
				digitalWrite(pinBuzzer, statusBuzzer);
				digitalWrite(pinLedGreen, HIGH);
			}
			if ((xTaskGetTickCount() - timeoutDisplay > Time_check) && (timeoutDisplay > 0)) {
				variLcdUpdate.stateDisplayLCD = statusDisplayRunning;
				variLcdUpdate.updateLCD = true;  // popup tuc thi
				timeoutDisplay = 0;
			}
			///////////////////////////////////////////////////////////////////////////
			/// Nhan Time Blink LED , phiên bản cũ khong dung  co the bo /////////////
			/////////////////////////////////////////////////////////////////////////
			xQueueReceive(Queue_Time_blink, &Time_blink, (TickType_t)1);
			if (xTaskGetTickCount() - lastTimeBlinkLed > Time_blink) {
				lastTimeBlinkLed = xTaskGetTickCount();
				status_led = !status_led;
			}
			////////////////////////////////////////////
			printDebugHeap();
			switch (variLcdUpdate.stateDisplayLCD) {
			case statusDisplayPopup: //Hien thi popUp 
				if ((xTaskGetTickCount() - lastBlinkLCD > 250) || variLcdUpdate.updateLCD) {
					LcdSeclectMode(statusDisplayPopup, &dataDisplayTH);
					variLcdUpdate.updateLCD = false;
				}
				break;
			case statusDisplayRunning: // hien thi running
				if ((xTaskGetTickCount() - lastBlinkLCD > 15000) || variLcdUpdate.updateLCD) {
					lastBlinkLCD = xTaskGetTickCount();
					LcdSeclectMode(statusDisplayRunning, &dataDisplayTH);
					variLcdUpdate.updateLCD = false;
				}
				break;
			default: break;
			}
			break;
			//////////////////////////////////////////////////////////////////
		case deviceSetting: ///////////////////////////////////////////////
		  //////////////////////////////////////////////////////////////////
			if ((xTaskGetTickCount() - lastBlinkLCD > 5000) || variLcdUpdate.updateLCD || lastBlinkLCD == 0) {
				variLcdUpdate.updateLCD = false;
				lastBlinkLCD = xTaskGetTickCount();
				LcdSeclectMode(stateMachine.bottonSelect + 2, &dataDisplayTH);
			}
			//////////////////////////////////////////////////////////////////
		case deviceTurnOff: /////////////////////////////////////////////
		  //////////////////////////////////////////////////////////////////
			if ((xTaskGetTickCount() - lastBlinkLCD > 5000) || variLcdUpdate.updateLCD) {
				//  LcdDisplayTurnOffPower();
				lastBlinkLCD = xTaskGetTickCount();
			}
			break;
		default:
			break;
		}
		printDebugHeap();
		vTaskDelayUntil(&xLastWakeTime, 50);
	}
	vTaskDelete(NULL);
}

unsigned long getTimeSendHeapDebug = 0;
void printDebugHeap() {
}
