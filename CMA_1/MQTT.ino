/***************************************************************************************************************************************************************************************
  Set thanh pham
  {"t":"3","l":"3","d":[{"i":"10","n":"Cá Semitrimmed ( Còn dè, Còn Mỡ, Còn thịt đỏ, bỏ đường chỉ hồng trên lưng)"},{"i":"11","n":"Thanh Pham 2"},{"i":"12","n":"Thanh Pham 3"}]}
  Set nha CC
  {"t":"1","l":"2","d":[{"i":"10","n":"Cá Semitrimmed"},{"i":"12","n":"Thanh Pham 3"}]}
  Set Khu Vuc can
  {"t":"2","l":"2","d":[{"i":2,"n":"Sửa Cá"},{"i":1,"n":"Fillet"}]}
*****************************************************************************************************************************************************************************************/

/*
   {
   i: Số thứ tự gói tin,
   k: Giai đoạn cân
   b: Mã rổ,
   p: Mã thành phẩm,
   e: Mã nhân viên,
   s: Mã cân,
   m: Mã loại cá,
   w: Khối lượng cá,
   t: Thời gian gửi,
  }
*/
/*

Gui data to topic MQTT
*/


void SendDataMqtt(char* topic, char* data) {
	if (xSemaphoreTake(xMutexMQTT, 1)) {
		if (statusPeripheral.mqtt.statusMqttConnect) {
			mqttClient.publish(topic, 0, true, data);
		}
		xSemaphoreGive(xMutexMQTT);
	}
}


/*
Connect to MQTT
*/
void ConnectToMqtt() {
	mqttClient.connect();
}
//////////////////////////////////////////////////////////////////
////// Config data MQTT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void EncoderJsonMqtt() {
	DynamicJsonDocument doc(300);
	doc["k"] = stateMachine.giaidoanINOUT;
	doc["x"] = stateMachine.giaidoanKV;
	doc["b"] = dataEncoderJsonMqtt.id_RFID;
	doc["e"] = dataEncoderJsonMqtt.id_RFID_NV;
	doc["s"] = (unsigned long)stateMachine.hardwareId;
	doc["w"] = dataEncoderJsonMqtt.data_weight;
	doc["t"] = timeStamp.unixtime();
	doc["c"] = inforServer.nhaCC.arrayType[inforServer.nhaCC.userSelect];
	doc["p"] = inforServer.thanhPham.arrayType[inforServer.thanhPham.userSelect];
	doc["r"] = 0;
	char* msg1 = (char*)calloc(300, 1);
	serializeJson(doc, msg1, 300);
	char textToWrite[16];
	sprintf(textToWrite, "/CMA/%lu", (unsigned long)timeStamp.unixtime());
	SdWriteFile(SD, textToWrite, msg1);
#ifdef debug_UART
	Serial.printf("Send MQTT: %s", msg1);
#endif
#ifdef debug_Web
	DebugData("Send MQTT: %s", msg1);
	Serial.printf("Send MQTT: %s\n", msg1);
#endif
	//char tam[] = "/data";
	SendDataMqtt(inforServer.mqttConfig.topicSenData, msg1);
	free(msg1);  // giai phong bo nho msg
	doc.clear(); // giai phong bo nho json
}
//////////////////////////////////////////////////////////////////
////// Setting MQTT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onMqttConnect(bool sessionPresent) {
	statusPeripheral.mqtt.statusMqttConnect = true;
	mqttClient.subscribe(inforServer.mqttConfig.topicGetStatusACK, 0);
	mqttClient.subscribe(inforServer.mqttConfig.topicGetConfig, 0);
	if (WiFiConf.mqtt_subto1[0] != 'x') {
		mqttClient.subscribe(WiFiConf.mqtt_subto1, 0); //0,1,2 laf qos
	}
}
//////////////////////////////////////////////////////////////////
////// MQTT Disconnect //////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
	statusPeripheral.mqtt.statusMqttConnect = false;
	if (WiFi.isConnected()) {
		xTimerStart(mqttReconnectTimer, 0);
	}
#ifdef debug_Web
	DebugError("MQTT: Disconnect");
#endif
}


/***************************************************************************************************************************************************************************************
  Set thanh pham
  {"t":"3","l":"3","d":[{"i":"10","n":"Cá Semitrimmed ( Còn dè, Còn Mỡ, Còn thịt đỏ, bỏ đường chỉ hồng trên lưng)"},{"i":"11","n":"Thanh Pham 2"},{"i":"12","n":"Thanh Pham 3"}]}
  Set nha CC
  {"t":"1","l":"2","d":[{"i":"10","n":"Cá Semitrimmed"},{"i":"12","n":"Thanh Pham 3"}]}
  Set Khu Vuc can
  {"t":"2","l":"2","d":[{"i":2,"n":"Sửa Cá"},{"i":1,"n":"Fillet"}]}
 *****************************************************************************************************************************************************************************************/
 //////////////////////////////////////////////////////////////////
 ////// GET MQTT ///////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
#ifdef debug_UART
	printf("MQTT: %s ,ND: %s \n", topic, payload);
#endif
#ifdef debug_Web
	DebugInfor("MQTT: %s ,ND: %s ", topic, payload);
#endif
	DynamicJsonDocument jsonBuffer(2048);
	DeserializationError error = deserializeJson(jsonBuffer, payload);
	if (error) {
#ifdef debug_UART
		Serial.println("error js");
#endif
#ifdef debug_Web
		DebugError("error js");
#endif

	}
	else if ((strcmp(WiFiConf.mqtt_subto1, topic) == 0)
		 || (strcmp(inforServer.mqttConfig.topicGetConfig, topic) == 0)) {
		/*
		  Nhan thong tin server cai dat ca lam viec
		*/
		if ((!jsonBuffer.containsKey("t")) || (!jsonBuffer.containsKey("d"))) {return;}
		switch (jsonBuffer["t"].as<uint8_t>()) {
		case mqttGetNhaCC:
			strlcpy(inforServer.nhaCC.arrayName[0], ramChuaChon, sizeof(inforServer.nhaCC.arrayName[0]));
			inforServer.nhaCC.total = jsonBuffer["l"].as<uint8_t>();
			for (int i = 0; i < inforServer.nhaCC.total; i++) {
				if (jsonBuffer["d"][i].isNull()) {
					inforServer.nhaCC.total = 0;
					return;
				} // Fix loi thieu data
				if (i == 15)break; //fix tran array cua data
				inforServer.nhaCC.arrayType[i + 1] = jsonBuffer["d"][i]["i"].as<uint16_t>();
				strlcpy(inforServer.nhaCC.arrayName[i + 1], jsonBuffer["d"][i]["n"], sizeof(inforServer.nhaCC.arrayName[i + 1]));
			}
			variLcdUpdate.updateLCD = true;
			break;
		case mqttGetKV:
			if (!jsonBuffer.containsKey("s")) { return; }
			stateMachine.giaidoanINOUT=jsonBuffer["l"].as<uint8_t>();
			stateMachine.giaidoanKV = jsonBuffer["u"].as<uint8_t>();
			stateMachine.setGiaiDoan();
			stateMachine.setKV();
			statusPeripheral.mqtt.lastTimeGetDataConfig = 0;
			break;
		case mqttGetThanhPham:
			strlcpy(inforServer.thanhPham.arrayName[0], ramChuaChon, sizeof(inforServer.thanhPham.arrayName[0]));
			inforServer.thanhPham.total = jsonBuffer["l"].as<uint8_t>();
			for (int i = 0; i < inforServer.thanhPham.total; i++) {
				if (jsonBuffer["d"][i].isNull()) {
					inforServer.thanhPham.total = 0;
					return;
				} // Fix loi thieu data
				if (i == 15)break; //qua array cua data
				inforServer.thanhPham.arrayType[i + 1] = jsonBuffer["d"][i]["i"].as<uint16_t>();
				strlcpy(inforServer.thanhPham.arrayName[i + 1], jsonBuffer["d"][i]["n"], sizeof(inforServer.thanhPham.arrayName[i + 1]));
			}
			variLcdUpdate.updateLCD = true;
			statusPeripheral.mqtt.lastTimeGetDataConfig = 0;
			break;
		case mqttGetTimeStamp:
			if (!jsonBuffer.containsKey("s")) { return; }
			statusPeripheral.timeStampServer = jsonBuffer["s"].as<uint32_t>();
			SetTimeRtc(statusPeripheral.timeStampServer);
			break;
		case mqttGetResetCa:
			if (jsonBuffer["s"].as<uint8_t>() == 1) {
				stateMachine.bottonSelect = 0;
				stateMachine.deviceStatus = deviceSetting;
				khoiTaoGiaTri(false);
				if (stateMachine.deviceStatus == deviceSetting) { variLcdUpdate.stateDisplayLCD = 1; }
			}
			break;
		default: break;
		}
	}
	else if (strcmp(inforServer.mqttConfig.topicGetStatusACK, topic) == 0) {
		/*
		  Khi Nhan duoc data server bao luu thanh cong goi tin thi sẽ xoa file luu trong sd card
		*/
		if ((!jsonBuffer.containsKey("i")) || (!jsonBuffer.containsKey("s"))) {
			return;
		}
		uint32_t sttData = jsonBuffer["i"].as<uint32_t>();
		uint8_t statusSaveData = jsonBuffer["s"].as<uint8_t>();
		char textEnd[18];
		sprintf(textEnd, "/CMA/O_%lu", (unsigned long)sttData);
		char textBegin[18];
		sprintf(textBegin, "/CMA/%lu", (unsigned long)sttData);
		// if (statusSaveData == 1)SdDeleteFile(SD,textToWrite); Thay doi tu delete toi rename.
		if (statusSaveData == 1) {
			SdRenameFile(SD, textBegin, textEnd);
			if (jsonBuffer.containsKey("e")) {
				statusPeripheral.mqtt.updateName = true;
				strlcpy(statusPeripheral.mqtt.nameNhanVien, jsonBuffer["e"], sizeof(statusPeripheral.mqtt.nameNhanVien));
			}
		}
	}
	jsonBuffer.clear(); //giai phong bo nho
}
//////////////////////////////////////////////////////////////////
////// Callback - Chua Dung //////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onMqttPublish(uint16_t packetId) {//printf("Publish acknowledged: %d \n",packetId);
}
void onMqttSubscribe(uint16_t packetId, uint8_t qos) {//printf("Subscribe acknowledged: %f\n",packetId);
}
void onMqttUnsubscribe(uint16_t packetId) {// printf("Unsubscribe acknowledged: %f\n",packetId);
}


/////////////////////////////////////////////////
// Send MQTT Con fig                   //////////////
////////////////////////////////////////////////
void SendDataConfigMqtt(uint8_t loaiconfig = 0, uint8_t maboxung = 0) {
	StaticJsonDocument<55> doc;
	char buffer[55];
	doc["i"] = stateMachine.hardwareId;
	doc["t"] = loaiconfig;
	doc["d"] = maboxung;
	serializeJson(doc, buffer);
	SendDataMqtt(inforServer.mqttConfig.topicSenConfig, buffer);
}
/////////////////////////////////////////////////////
//Check Get Config /////////////////////////////////
///////////////////////////////////////////////////
void checkSendMQTTConfig() {
	if (statusPeripheral.timeStampServer == 0) 
	{
		SendDataConfigMqtt(mqttGetTimeStamp, 0);
	}
	else if ((stateMachine.giaidoanINOUT == 1)
		&& (inforServer.nhaCC.total == 0)) 
	{
		SendDataConfigMqtt(mqttGetNhaCC, 0);
	}
	else if ((stateMachine.giaidoanINOUT == 1)
		&& (inforServer.thanhPham.total == 0)) 
	{ // laf dau vao
		SendDataConfigMqtt(mqttGetThanhPham, stateMachine.giaidoanKV);
	}
}