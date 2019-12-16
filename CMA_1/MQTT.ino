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
void SendDataMqtt(char* data) {
    if (statusPeripheral.mqtt.statusMqttConnect) {
        mqttClient.publish("/data", 0, true, data);
    }
}
void ConnectToMqtt() {
  mqttClient.connect();
}
//////////////////////////////////////////////////////////////////
////// Config data MQTT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void EncoderJsonMqtt() {
  //DateTime now = rtc.now();
 // StaticJsonDocument<300> doc;
  DynamicJsonDocument doc(300);
  doc["k"] = inforServer.giaiDoan.cheDoInOut;
  doc["x"] = inforServer.giaiDoan.arrayType[inforServer.giaiDoan.userSelect];
  doc["b"] = dataEncoderJsonMqtt.id_RFID;
  doc["e"] = dataEncoderJsonMqtt.id_RFID_NV;
  doc["s"] = (unsigned long) stateMachine.idDevice;
  doc["w"] = dataEncoderJsonMqtt.data_weight;
  doc["t"] = timeStamp.unixtime();
  doc["c"] = inforServer.nhaCC.arrayType[inforServer.nhaCC.userSelect];
  doc["p"] = inforServer.thanhPham.arrayType[inforServer.thanhPham.userSelect];
  //char buffer[500];
  char* msg1 = (char*)calloc(300, 1);
  serializeJson(doc, msg1 , 300);
  char textToWrite[ 16 ];
  sprintf(textToWrite, "/CMA/%lu", ( unsigned long )timeStamp.unixtime());
  SdWriteFile(SD, textToWrite, msg1 , 0);
#ifdef debug_UART
  Serial.print("Send MQTT: ");
  Serial.println(msg1);
#endif
  SendDataMqtt(msg1);
  free(msg1);
  doc.clear(); // giai phong bo nho
 // if (statusPeripheral.mqtt.statusMqttConnect) {SendDataMqtt(inforServer.mqttConfig.dataSend);}
  
}
//////////////////////////////////////////////////////////////////
////// Setting MQTT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onMqttConnect(bool sessionPresent) {
  statusPeripheral.mqtt.statusMqttConnect = true;
  mqttClient.subscribe( inforServer.mqttConfig.topicGetStatusACK, 0 );
  mqttClient.subscribe( inforServer.mqttConfig.topicGetConfig, 0 );
  if (WiFiConf.mqtt_subto1[0] != 'x') {
    mqttClient.subscribe( WiFiConf.mqtt_subto1, 0 ); //0,1,2 laf qos
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
  // Bo nho lon hon 1kb thay vi dung Static chuyen qua dung DynamicJsonDocument no cap phat malloc va free
 // StaticJsonDocument<1500> jsonBuffer;
  DynamicJsonDocument jsonBuffer(2048);
  DeserializationError error = deserializeJson(jsonBuffer, payload);
  if (error) {
#ifdef debug_UART
    Serial.println("error js");
    //Serial.
#endif
  }
  else if ((strcmp(WiFiConf.mqtt_subto1, topic) == 0) || (strcmp(inforServer.mqttConfig.topicGetConfig, topic) == 0)) {
    /*
      Nhan thong tin server cai dat ca lam viec
    */
    if (!jsonBuffer.containsKey("t")) {
      return;
    }
    if (!jsonBuffer.containsKey("d")) {
      return;
    }
    if (jsonBuffer["t"].as<uint8_t>() == 1) {
      strlcpy(inforServer.nhaCC.arrayName[0], ramChuaChon, sizeof(inforServer.nhaCC.arrayName[0]));
      inforServer.nhaCC.total = jsonBuffer["l"].as<uint8_t>();
      for (int i = 0; i < inforServer.nhaCC.total; i++) {
        if (jsonBuffer["d"][i].isNull()) {Serial.println("error key");inforServer.nhaCC.total = 0;return;} // Fix loi thieu data
        if (i == 15)break; //fix tran array cua data
        inforServer.nhaCC.arrayType[i + 1] = jsonBuffer["d"][i]["i"].as<uint16_t>();
        strlcpy(inforServer.nhaCC.arrayName[i + 1], jsonBuffer["d"][i]["n"], sizeof(inforServer.nhaCC.arrayName[i + 1]));
      }
    }
    else if (jsonBuffer["t"].as<uint8_t>() == 2) {
        if (!jsonBuffer.containsKey("s")) {return;}
      strlcpy(inforServer.giaiDoan.arrayName[0], ramChuaChon, sizeof(inforServer.giaiDoan.arrayName[0]));
      inforServer.giaiDoan.total = jsonBuffer["l"].as<uint8_t>();
      statusPeripheral.timeStampServer = jsonBuffer["s"].as<uint32_t>();
      SetTimeRtc(statusPeripheral.timeStampServer);
      for (int i = 0; i < inforServer.giaiDoan.total; i++) {
        if (jsonBuffer["d"][i].isNull()) {Serial.println("error key");inforServer.giaiDoan.total = 0;return;} // Fix loi thieu data
        if (i == 10)break; //qua array cua data
        inforServer.giaiDoan.arrayType[i + 1] = jsonBuffer["d"][i]["i"].as<uint16_t>();
        strlcpy(inforServer.giaiDoan.arrayName[i + 1], jsonBuffer["d"][i]["n"], sizeof(inforServer.giaiDoan.arrayName[i + 1]));
      }
      
      statusPeripheral.mqtt.lastTimeGetDataConfig = 0;
    }
    else if (jsonBuffer["t"].as<uint8_t>() == 3) {
      strlcpy(inforServer.thanhPham.arrayName[0], ramChuaChon, sizeof(inforServer.thanhPham.arrayName[0]));
      inforServer.thanhPham.total = jsonBuffer["l"].as<uint8_t>();
      for (int i = 0; i < inforServer.thanhPham.total; i++) {
        if (jsonBuffer["d"][i].isNull()) {Serial.println("error key");inforServer.thanhPham.total=0;return;} // Fix loi thieu data
        if (i == 15)break; //qua array cua data
        inforServer.thanhPham.arrayType[i + 1] = jsonBuffer["d"][i]["i"].as<uint16_t>();
        strlcpy(inforServer.thanhPham.arrayName[i + 1], jsonBuffer["d"][i]["n"], sizeof(inforServer.thanhPham.arrayName[i + 1]));
      }
      statusPeripheral.mqtt.lastTimeGetDataConfig = 0;
    }
    /////////////////////////////////////////
    /// Nhan du lieu het ca reset data /////
    ///////////////////////////////////////
    else if (jsonBuffer["t"].as<uint8_t>() == 5) {
        uint8_t statusSaveData = jsonBuffer["s"].as<uint8_t>();
        if (statusSaveData == 1) {
            stateMachine.bottonSelect = 0;
            stateMachine.deviceStatus = deviceSetting;
            khoiTaoGiaTri(false);
            if (stateMachine.deviceStatus == deviceSetting) { variLcdUpdate.stateDisplayLCD = 1; }
        }
    }
  }
  else if (strcmp(inforServer.mqttConfig.topicGetStatusACK, topic) == 0) {
    /*
      Khi Nhan duoc data server bao luu thanh cong goi tin thi sẽ xoa file luu trong sd card
    */
    if (!jsonBuffer.containsKey("i")) {
      return;
    }
    else if (!jsonBuffer.containsKey("s")) {
      return;
    }
    uint32_t sttData = jsonBuffer["i"].as<uint32_t>();
    uint8_t statusSaveData = jsonBuffer["s"].as<uint8_t>();
    char textEnd[ 18 ];
    sprintf(textEnd, "/CMA/O_%lu", ( unsigned long )sttData);
    char textBegin[ 18 ];
    sprintf(textBegin, "/CMA/%lu", ( unsigned long )sttData);
    // if (statusSaveData == 1)SdDeleteFile(SD,textToWrite); Thay doi tu delete toi rename.
    //if (statusSaveData == 1)SdRenameFile(SD, textBegin, textEnd);
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
