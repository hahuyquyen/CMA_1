 /***************************************************************************************************************************************************************************************
Set thanh pham 
{"t":"3","l":"3","d":[{"i":"10","n":"Cá Semitrimmed ( Còn dè, Còn Mỡ, Còn thịt đỏ, bỏ đường chỉ hồng trên lưng)"},{"i":"11","n":"Thanh Pham 2"},{"i":"12","n":"Thanh Pham 3"}]}
Set nha CC
{"t":"1","l":"2","d":[{"i":"10","n":"Cá Semitrimmed"},{"i":"12","n":"Thanh Pham 3"}]}
Set Khu Vuc can
{"t":"2","l":"2","d":[{"i":2,"n":"Sửa Cá"},{"i":1,"n":"Fillet"}]}
 *****************************************************************************************************************************************************************************************/
 
 /*
  * {
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
 
void connectToMqtt() {
  mqttClient.connect();
}
//////////////////////////////////////////////////////////////////
////// Send MQTT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void truyen_mqtt(){
      DateTime now = rtc.now();
      StaticJsonDocument<500> doc;
      doc["k"] = inforServer.giaiDoan.cheDoInOut;
      doc["x"] = inforServer.giaiDoan.arrayType[inforServer.giaiDoan.userSelect];
      doc["b"] = datatruyen_mqtt.id_RFID;
      doc["e"] = datatruyen_mqtt.id_RFID_NV;
      doc["s"] = (unsigned long) stateMachine.idDevice;      
      doc["w"] = datatruyen_mqtt.data_weight;
      doc["t"] = now.unixtime();
      doc["c"] = inforServer.nhaCC.arrayType[inforServer.nhaCC.userSelect];
      doc["p"] = inforServer.thanhPham.arrayType[inforServer.thanhPham.userSelect];
      char buffer[500];
      serializeJson(doc, buffer);
      char textToWrite[ 16 ];
      sprintf(textToWrite,"/CMA/%lu", ( unsigned long )now.unixtime());
      writeFile(SD, textToWrite, buffer);
      if (statusPeripheral.mqtt.statusMqttConnect){ mqttClient.publish("/data", 0, true, buffer);}
     
}
//////////////////////////////////////////////////////////////////
////// Setting MQTT ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onMqttConnect(bool sessionPresent) {
        statusPeripheral.mqtt.statusMqttConnect = true;
        mqttClient.subscribe( inforServer.mqttConfig.topicGetStatusACK,0 ); 
        mqttClient.subscribe( inforServer.mqttConfig.topicGetConfig,0 ); 
        if (WiFiConf.mqtt_subto1[0] != 'x'){mqttClient.subscribe( WiFiConf.mqtt_subto1,0 );}  //0,1,2 laf qos
}
//////////////////////////////////////////////////////////////////
////// MQTT Disconnect //////////////////////////////////////
//////////////////////////////////////////////////////////////////
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
 statusPeripheral.mqtt.statusMqttConnect = false;
  if (WiFi.isConnected()) {xTimerStart(mqttReconnectTimer, 0);}
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
  printf("MQTT GET: %s \n",topic);
  //printf("QOS: %c \n", properties.qos);
  printf("noi dung: %s \n", payload);
#endif
  StaticJsonDocument<1500> jsonBuffer;
  DeserializationError error = deserializeJson(jsonBuffer,payload);
  if (error) {
#ifdef debug_UART    
  Serial.println("error js");
#endif
  }
  else if ((strcmp(WiFiConf.mqtt_subto1,topic) == 0)||(strcmp(inforServer.mqttConfig.topicGetConfig,topic) == 0)){
    /*
     Nhan thong tin server cai dat ca lam viec
     */
        if (!jsonBuffer.containsKey("t")) {return;}
        if (!jsonBuffer.containsKey("d")) {return;}
        if (jsonBuffer["t"].as<uint8_t>() == 1){   
          strlcpy(inforServer.nhaCC.arrayName[0], ramChuaChon, sizeof(inforServer.nhaCC.arrayName[0]));
          inforServer.nhaCC.total=jsonBuffer["l"].as<uint8_t>();
          for (int i=0;i<inforServer.nhaCC.total;i++){
            if (i == 15)break; //qua array cua data
            inforServer.nhaCC.arrayType[i+1]=jsonBuffer["d"][i]["i"].as<uint16_t>();
            strlcpy(inforServer.nhaCC.arrayName[i+1], jsonBuffer["d"][i]["n"], sizeof(inforServer.nhaCC.arrayName[i+1]));
          }  
        }
        else if (jsonBuffer["t"].as<uint8_t>() == 2){
          strlcpy(inforServer.giaiDoan.arrayName[0], ramChuaChon, sizeof(inforServer.giaiDoan.arrayName[0]));
          inforServer.giaiDoan.total=jsonBuffer["l"].as<uint8_t>();
          for (int i=0;i<inforServer.giaiDoan.total;i++){
            if (i == 10)break; //qua array cua data
            inforServer.giaiDoan.arrayType[i+1]=jsonBuffer["d"][i]["i"].as<uint16_t>();
            strlcpy(inforServer.giaiDoan.arrayName[i+1], jsonBuffer["d"][i]["n"], sizeof(inforServer.giaiDoan.arrayName[i+1]));
          }
          statusPeripheral.mqtt.lastTimeGetDataConfig = 0;
        }
       else if (jsonBuffer["t"].as<uint8_t>() == 3){
          strlcpy(inforServer.thanhPham.arrayName[0], ramChuaChon, sizeof(inforServer.thanhPham.arrayName[0]));
          inforServer.thanhPham.total=jsonBuffer["l"].as<uint8_t>();
          for (int i=0;i<inforServer.thanhPham.total;i++){
            if (i == 15)break; //qua array cua data
            inforServer.thanhPham.arrayType[i+1]=jsonBuffer["d"][i]["i"].as<uint16_t>();
            strlcpy(inforServer.thanhPham.arrayName[i+1], jsonBuffer["d"][i]["n"], sizeof(inforServer.thanhPham.arrayName[i+1]));
          }
          statusPeripheral.mqtt.lastTimeGetDataConfig = 0;
        }  
  }
  else if (strcmp(inforServer.mqttConfig.topicGetStatusACK,topic) == 0){
    /*
     Khi Nhan duoc data server bao luu thanh cong goi tin thi sẽ xoa file luu trong sd card
     */
      if (!jsonBuffer.containsKey("i")) {return;}
      else if (!jsonBuffer.containsKey("s")) {return;}
      uint32_t sttData = jsonBuffer["i"].as<uint32_t>();
      uint8_t statusSaveData = jsonBuffer["s"].as<uint8_t>();
      char textToWrite[ 16 ];
      sprintf(textToWrite,"/CMA/%lu", ( unsigned long )sttData);
     // if (statusSaveData == 1)deleteFile(SD,textToWrite); Thay doi tu delete toi rename.
      if (statusSaveData == 1)renameFiles(SD,textToWrite);
  }
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
