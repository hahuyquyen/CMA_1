
// uint16_t dataTruyen[20];

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) {
	//Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, transactionId);
	return true;
}
/*
 * #define pageInfor 10
#define pageNhaCC 12
#define pageThanhPham 13
#define pageComfirm 14
#define pageRunning 15
#define pagePopup 16
 */
void send485HMI(uint16_t address, uint16_t* kytu, uint8_t numByte) {
	if (!mb.slave()) {
		if (xSemaphoreTake(xMutexRS485, 1)) {
			mb.writeHreg(1, address, kytu, numByte, cb); //ID man hinh
			while (mb.slave()) { mb.task(); }
			xSemaphoreGive(xMutexRS485);
		}
	}
}
void sendSSID485() {
	modbusData.dataTruyen[0] = WiFi.localIP()[0];
	modbusData.dataTruyen[1] = WiFi.localIP()[1];
	modbusData.dataTruyen[2] = WiFi.localIP()[2];
	modbusData.dataTruyen[3] = WiFi.localIP()[3];
	send485HMI(7920, modbusData.dataTruyen, 4);
	send485Utf16(7924, WiFiConf.sta_ssid, 16);
}
void send485Utf16(uint16_t address, char* kytu, uint8_t numByte) {
	wmemset(modbusData.nameNvUtf16, 0x0000, numByte);
	size_t valuelen = utf8towchar(kytu, SIZE_MAX, modbusData.nameNvUtf16, sizeof(modbusData.nameNvUtf16));
	send485HMI(address, (uint16_t* )modbusData.nameNvUtf16, numByte);
}
void send485char(uint16_t address, char* kytu, uint8_t numByte) {
	for (int j = 0; j < numByte * 2; j = j + 2) {
			modbusData.dataTruyen[j / 2] = (((uint16_t)kytu[j + 1] << 8) | ((uint16_t)kytu[j]));
	}
	send485HMI(address, modbusData.dataTruyen, numByte);
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
	send485HMI(7900, modbusData.dataTruyen, numByte);
}
void send485PageKg(uint16_t page, uint32_t kg) {
	modbusData.dataTruyen[0] = (uint16_t)kg;
	modbusData.dataTruyen[1] = (uint16_t)kg >> 16;
	modbusData.dataTruyen[2] = page;
	send485HMI(7898, modbusData.dataTruyen,3);
}
void send485Kg(uint32_t kg) {
	modbusData.dataTruyen[0] = (uint16_t)kg;
	modbusData.dataTruyen[1] = (uint16_t)kg >> 16;
	send485HMI(7910, modbusData.dataTruyen, 2);
}
void LcdSeclectMode(uint8_t modeDisplay, Data_TH* dataLCDTH) {
	if (mb.slave()) { return; }
	switch (modeDisplay) {
	case 0: //popup
		if (statusPeripheral.mqtt.updateName) { send485Utf16(8162, statusPeripheral.mqtt.nameNhanVien, 28); }
		char textTam[12];
		memset(textTam, 0x00, sizeof(textTam));
		if (dataLCDTH->id_RFID_NV[0] != 'x') { memcpy(textTam, &dataLCDTH->id_RFID_NV[16], 8); textTam[9] = '\0'; }
		else memcpy(textTam, "x", 1);
		send485char(8150, textTam, 6);
		memset(textTam, 0x00, sizeof(textTam));
		if (dataLCDTH->id_RFID[0] != 'x') { memcpy(textTam, &dataLCDTH->id_RFID[16], 8); textTam[9] = '\0'; }
		else memcpy(textTam, "x", 1);
		send485char(8156, textTam, 6);
		send485PageKg(pagePopup, (uint32_t)(dataLCDTH->data_weight * 1000));
		break;
	case 1: //running
		if (statusPeripheral.mqtt.updateName) {statusPeripheral.mqtt.updateName = false;
			send485Utf16(8162, "\0", 28); // reset ten nv
		}
		send485Utf16(8000, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], 32);
		send485Utf16(8032, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], 64);
		send485PageAndData(pageRunning, false);
		break;
	case 2: //infor bat dau
		send485PageAndData(pageInfor,true);
		break;
	case 3: //infor bat dau
		send485PageAndData(pageInfor,true);
		break;
	case 4: //nha cung cap
		send485Utf16(8000, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], 32);
		send485PageAndData(pageNhaCC, false);
		break;
	case 5: //thanh pham inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect]
		send485Utf16(8032, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], 64);
		send485PageAndData(pageThanhPham, false);
		break;
	case 6: // xac nhan
		send485Utf16(8000, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], 32);
		send485Utf16(8032, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], 64);
		send485PageAndData(pageComfirm, false);
		break;
	case 10: //infor setting
		break;
	default:
		break;
	}
}


// uint16_t dataTruyen[20];
extern double can_data;
bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) {
	//Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, transactionId);
	return true;
}
 //////////////////////////////////////////////////////////
 // Truyen Modbus RTU tới slaver /////////////
 //////////////////////////////////////////////////////////
void send485HMI(uint16_t address, uint16_t* kytu, uint8_t numByte) {
	if (!mb.slave()) {
		if (xSemaphoreTake(xMutexRS485, 1)) {
			mb.writeHreg(1, address, kytu, numByte, cb); //ID man hinh
			while (mb.slave()) { mb.task(); }
			xSemaphoreGive(xMutexRS485);
		}
	}
}
//////////////////////////////////////////////////////////
// Convert UTF8 to UTF16  and send to Modbus /////////////
// Data truyen tieng viet co dau thuoc utf16 /////////////
//////////////////////////////////////////////////////////
void send485Utf16(uint16_t address, char* kytu, uint8_t numByte) {
	wmemset(modbusData.nameNvUtf16, 0x0000, numByte);
	size_t valuelen = utf8towchar(kytu, SIZE_MAX, modbusData.nameNvUtf16, sizeof(modbusData.nameNvUtf16));
	send485HMI(address, (uint16_t*)modbusData.nameNvUtf16, numByte);
}
//////////////////////////////////////////////////////////
// Ghep 2 char 8 bit thanh 16 bit truyen modbus   ////////
// Data truyen la ky tu khong dau                ///////////
//////////////////////////////////////////////////////////
void send485char(uint16_t address, char* kytu, uint8_t numByte) {
	memset(modbusData.dataTruyen, '\0\0', numByte);
	uint8_t endChar = 0;
	for (int j = 0; j < numByte * 2; j = j + 2) {
		modbusData.dataTruyen[j / 2] = (((uint16_t)kytu[j + 1] << 8) | ((uint16_t)kytu[j]));
		if ((kytu[j + 1] == '\0') || (kytu[j] == '\0')) {
			endChar = j;
			break;

		}
	}
	send485HMI(address, modbusData.dataTruyen, endChar);
}
//////////////////////////////////////////////////////////
// Send IP và SSID toi Modbus                     ////////
//////////////////////////////////////////////////////////
void sendSSID485() {
	modbusData.dataTruyen[0] = WiFi.localIP()[0];
	modbusData.dataTruyen[1] = WiFi.localIP()[1];
	modbusData.dataTruyen[2] = WiFi.localIP()[2];
	modbusData.dataTruyen[3] = WiFi.localIP()[3];
	send485HMI(rs485AddressIPAddress, modbusData.dataTruyen, 4);
	send485char(rs485AddressNameSSID, WiFiConf.sta_ssid, 16);
	//send485Utf16(rs485AddressNameSSID, WiFiConf.sta_ssid, 16);
}
//////////////////////////////////////////////////////////
// Send STT page va Data kem theo (hardwareId, giai doan  ////////
//////////////////////////////////////////////////////////
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
	send485HMI(rs485AddressPage, modbusData.dataTruyen, numByte);
}
//////////////////////////////////////////////////////////
// Send STT page va so Kg static khi nhan ma NV  ////////
//////////////////////////////////////////////////////////
void send485PageKg(uint16_t page, uint32_t kg) {
	modbusData.dataTruyen[0] = (uint16_t)kg;
	modbusData.dataTruyen[1] = (uint16_t)kg >> 16;
	modbusData.dataTruyen[2] = page;
	send485HMI(rs485AddressSoKgStatic, modbusData.dataTruyen,3);
}
//////////////////////////////////////////////////////////
// Send so Kg dong cua can  ////////
//////////////////////////////////////////////////////////
void send485Kg(uint32_t kg) {
	modbusData.dataTruyen[0] = (uint16_t)kg;
	modbusData.dataTruyen[1] = (uint16_t)kg >> 16;
	send485HMI(rs485AddressSoKg, modbusData.dataTruyen, 2);
}
void LcdSeclectMode(uint8_t modeDisplay, Data_TH* dataLCDTH) {
	if (mb.slave()) { return; }
	switch (modeDisplay) {
	case 0: //popup
		if (statusPeripheral.mqtt.updateName) { send485Utf16(rs485AddressNameNhanVien, statusPeripheral.mqtt.nameNhanVien, 28); }
		char textTam[12];
		memset(textTam, 0x00, sizeof(textTam));
		if (dataLCDTH->id_RFID_NV[0] != 'x') { memcpy(textTam, &dataLCDTH->id_RFID_NV[16], 8); textTam[9] = '\0'; }
		else memcpy(textTam, "x", 2);
		send485char(rs485AddressMaNV, textTam, 6);
		memset(textTam, 0x00, sizeof(textTam));
		if (dataLCDTH->id_RFID[0] != 'x') { memcpy(textTam, &dataLCDTH->id_RFID[16], 8); textTam[9] = '\0'; }
		else memcpy(textTam, "x", 2);
		send485char(rs485AddressMaRo, textTam, 6);
		send485PageKg(pagePopup, (uint32_t)(dataLCDTH->data_weight * 1000));
		break;
	case 1: //running
		if (statusPeripheral.mqtt.updateName) {statusPeripheral.mqtt.updateName = false;
			send485Utf16(rs485AddressNameNhanVien, "\0", 28); // reset ten nv
		}
		send485Utf16(rs485AddressNameNhaCc, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], 32);
		send485Utf16(rs485AddressNameThanhPham, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], 64);
		send485PageAndData(pageRunning, false);
		break;
	case 2: //infor bat dau
		send485PageAndData(pageInfor,true);
		break;
	case 3: //infor bat dau
		send485PageAndData(pageInfor,true);
		break;
	case 4: //nha cung cap
		send485Utf16(rs485AddressNameNhaCc, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], 32);
		send485PageAndData(pageNhaCC, false);
		break;
	case 5: //thanh pham inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect]
		send485Utf16(rs485AddressNameThanhPham, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], 64);
		send485PageAndData(pageThanhPham, false);
		break;
	case 6: // xac nhan
		send485Utf16(rs485AddressNameNhaCc, inforServer.nhaCC.arrayName[inforServer.nhaCC.userSelect], 32);
		send485Utf16(rs485AddressNameThanhPham, inforServer.thanhPham.arrayName[inforServer.thanhPham.userSelect], 64);
		send485PageAndData(pageComfirm, false);
		break;
	case 10: //infor setting
		break;
	default:
		break;
	}
}



void Display(void* pvParameters) {
	boolean status_led = true;
	Data_TH dataDisplayTH;
	unsigned long lastTimeBlinkLed = 0;
	unsigned long timeoutDisplay = 0;
	//  unsigned long timeoutLcdLangDaIn = 0;
	unsigned long lastBlinkLCD = 0;
	uint16_t Time_blink = 1000;
	uint16_t Time_check = 2500;
	//Serial1.begin(38400, SERIAL_8N1, 26, 12); //12 tx 13 lÃ  rx(bau,se,rx,tx)
	mb.begin(&Serial);
	mb.master();
	LcdSeclectMode(2, &dataDisplayTH);
	variLcdUpdate.stateDisplayLCD = 1;
	uint8_t daucham_lcd = 0;
	boolean statusBuzzer = false;
	digitalWrite(pinLedGreen, HIGH);
	digitalWrite(pinLedRed, HIGH);
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	uint16_t mahinh = 10;

	for (;;) {

		if ((xTaskGetTickCount() - modbusData.timeSendSSID > 20000) || (modbusData.timeSendSSID == 0)) {
			modbusData.timeSendSSID = xTaskGetTickCount();
			sendSSID485();
		}
		else  if ((xTaskGetTickCount() - modbusData.timeSendKg > 800) || (modbusData.timeSendSSID == 0)) {
			modbusData.timeSendKg = xTaskGetTickCount();
			send485Kg((can_data > 0) ? ((uint32_t)(can_data * 1000)) : 0);
		}
		switch (stateMachine.deviceStatus) {
			//////////////////////////////////////////////////////////////////
		case deviceRunning: //////////////////////////////////////////////
		  //////////////////////////////////////////////////////////////////
			if (xQueueReceive(Queue_display, &dataDisplayTH, (TickType_t)1) == pdPASS) {
				variLcdUpdate.stateDisplayLCD = 0;  //popup man hinh
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
#ifdef debug_UART
					Serial.println(" Da Nhan du thong tin");
#endif
#ifdef debug_Web
					DebugData("Da Nhan du thong tin");
#endif
=======
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
			// Turn Off Buzzer, LED
			if (xSemaphoreTake(xSignal_Display_checkdone, 1)) { //Che do IN qua timeout se tat
				variLcdUpdate.stateDisplayLCD = 1;
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
				variLcdUpdate.stateDisplayLCD = 1;
				variLcdUpdate.updateLCD = true;  // popup tuc thi
				timeoutDisplay = 0;
			}
			xQueueReceive(Queue_Time_blink, &Time_blink, (TickType_t)1);
			if (xTaskGetTickCount() - lastTimeBlinkLed > Time_blink) {
				lastTimeBlinkLed = xTaskGetTickCount();
				status_led = !status_led;
			}
			printDebugHeap();
			switch (variLcdUpdate.stateDisplayLCD) {
			case 0: //Hien thi popUp
				if ((xTaskGetTickCount() - lastBlinkLCD > 250) || variLcdUpdate.updateLCD) {
					LcdSeclectMode(0, &dataDisplayTH);
					variLcdUpdate.updateLCD = false;
				}
				break;
			case 1: // hien thi running
				if ((xTaskGetTickCount() - lastBlinkLCD > 15000) || variLcdUpdate.updateLCD) {
					lastBlinkLCD = xTaskGetTickCount();
					LcdSeclectMode(1, &dataDisplayTH);
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

//unsigned long getTimeSendHeapDebug = 0;
void printDebugHeap() {
}
