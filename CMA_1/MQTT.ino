 /*
  * 
  * 
  * 
QoS
á»ž Ä‘Ã¢y cÃ³ 3 tuá»³ chá»�n *QoS (Qualities of service) * khi "publish" vÃ  "subscribe":

QoS0 Broker/client sáº½ gá»Ÿi dá»¯ liá»‡u Ä‘Ãºng 1 láº§n, quÃ¡ trÃ¬nh gá»Ÿi Ä‘Æ°á»£c xÃ¡c nháº­n bá»Ÿi chá»‰ giao thá»©c TCP/IP, giá»‘ng kiá»ƒu Ä‘em con bá»� chá»£.
QoS1 Broker/client sáº½ gá»Ÿi dá»¯ liá»‡u vá»›i Ã­t nháº¥t 1 láº§n xÃ¡c nháº­n tá»« Ä‘áº§u kia, nghÄ©a lÃ  cÃ³ thá»ƒ cÃ³ nhiá»�u hÆ¡n 1 láº§n xÃ¡c nháº­n Ä‘Ã£ nháº­n Ä‘Æ°á»£c dá»¯ liá»‡u.
QoS2 Broker/client Ä‘áº£m báº£m khi gá»Ÿi dá»¯ liá»‡u thÃ¬ phÃ­a nháº­n chá»‰ nháº­n Ä‘Æ°á»£c Ä‘Ãºng 1 láº§n, quÃ¡ trÃ¬nh nÃ y pháº£i tráº£i qua 4 bÆ°á»›c báº¯t tay.
  */
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
void truyen_mqtt(){
      DateTime now = rtc.now();
      StaticJsonDocument<500> doc;
      doc["k"] = static_cast<uint8_t>(inforServer.PhanLoaiKV);
      doc["b"] = datatruyen_mqtt.id_RFID;
      doc["e"] = datatruyen_mqtt.id_RFID_NV;
      doc["s"] = (unsigned long) idDevice;      
      doc["w"] = datatruyen_mqtt.data_weight;
      doc["t"] = now.unixtime();
      doc["m"] = inforServer.maLoaica[inforServer.userSelectLoaiCa];
      doc["c"] = inforServer.maNhaCC[inforServer.userSelectNhaCC];
      doc["p"] = inforServer.maThanhPham[inforServer.userSelectThanhPham];
      char buffer[500];
      serializeJson(doc, buffer);
      if (status_mqtt_connect){ mqttClient.publish("/data", 0, true, buffer);}
      char textToWrite[ 16 ];
      sprintf(textToWrite,"/CMA/%lu", ( unsigned long )now.unixtime());
      writeFile(SD, textToWrite, buffer);
}
void onMqttConnect(bool sessionPresent) {
        status_mqtt_connect = true;
        mqttClient.subscribe( MQTT_TOPIC.dataAck,0 ); 
        mqttClient.subscribe( MQTT_TOPIC.configGetId,0 ); 
        if (WiFiConf.mqtt_subto1[0] != 'x'){mqttClient.subscribe( WiFiConf.mqtt_subto1,0 );}  //0,1,2 laf qos
        if (WiFiConf.mqtt_subto2[0] != 'x'){mqttClient.subscribe(WiFiConf.mqtt_subto2,0);}
        if (WiFiConf.mqtt_subto3[0] != 'x'){mqttClient.subscribe(WiFiConf.mqtt_subto3,0);}  
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
 status_mqtt_connect = false;
  if (WiFi.isConnected()) {xTimerStart(mqttReconnectTimer, 0);}
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {//printf("Subscribe acknowledged: %f\n",packetId);
}

void onMqttUnsubscribe(uint16_t packetId) {// printf("Unsubscribe acknowledged: %f\n",packetId);
}


/*
 * {"t":"1","l":"3","data":[{"i":5455,"n":"pham an nhàn há há 7"},{"i":68,"n":"nhàn 8"},{"i":98,"n":"nhàn 9"}]}
 */

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  
  printf("MQTT GET: %s \n",topic);
  //printf("QOS: %c \n", properties.qos);
 // printf("noi dung: %s \n", payload);

  StaticJsonDocument<1500> jsonBuffer;
  DeserializationError error = deserializeJson(jsonBuffer,payload);
  if (error) Serial.println("error json");
  if ((strcmp(WiFiConf.mqtt_subto1,topic) == 0)||(strcmp(MQTT_TOPIC.configGetId,topic) == 0)){
        if (jsonBuffer["t"].as<uint8_t>() == 1){
          inforServer.tongLoaiCa=jsonBuffer["l"].as<uint8_t>();
          strlcpy(inforServer.nameLoaiCa[0], ramChuaChon, sizeof(inforServer.nameLoaiCa[0]));
          for (int i=0;i<inforServer.tongLoaiCa;i++){
            inforServer.maLoaica[i+1]= jsonBuffer["data"][i]["i"].as<uint16_t>();
            strlcpy(inforServer.nameLoaiCa[i+1], jsonBuffer["data"][i]["n"], sizeof(inforServer.nameLoaiCa[i]));
          }      
        }
        else if (jsonBuffer["t"].as<uint8_t>() == 2){
           strlcpy(inforServer.nameSoLo[0], ramChuaChon, sizeof(inforServer.nameSoLo[0]));
          inforServer.tongNhaCC=jsonBuffer["l"].as<uint8_t>();
          for (int i=0;i<inforServer.tongNhaCC;i++){
            inforServer.maNhaCC[i+1]=jsonBuffer["data"][i]["i"].as<uint16_t>();
            strlcpy(inforServer.nameSoLo[i+1], jsonBuffer["data"][i]["n"], sizeof(inforServer.nameSoLo[i]));
          }  
        }
        else if (jsonBuffer["t"].as<uint8_t>() == 3){
          strlcpy(inforServer.nameThanhPham[0], ramChuaChon, sizeof(inforServer.nameThanhPham[0]));
          inforServer.tongThanhPham=jsonBuffer["l"].as<uint8_t>();
          for (int i=0;i<inforServer.tongThanhPham;i++){
            inforServer.maThanhPham[i+1]=jsonBuffer["data"][i]["i"].as<uint16_t>();
            strlcpy(inforServer.nameThanhPham[i+1], jsonBuffer["data"][i]["n"], sizeof(inforServer.nameThanhPham[i+1]));
          }
        }  

  }
  if (strcmp(MQTT_TOPIC.dataAck,topic) == 0){
      uint32_t sttData = jsonBuffer["i"].as<uint32_t>();
      uint8_t statusSaveData = jsonBuffer["s"].as<uint8_t>();
      char textToWrite[ 16 ];
      sprintf(textToWrite,"/CMA/%lu", ( unsigned long )sttData);
      if (statusSaveData == 1)deleteFile(SD,textToWrite);
  }
  if (strcmp(WiFiConf.mqtt_subto3,topic) == 0){printf("vung 3 \n");}
}

void onMqttPublish(uint16_t packetId) {
 printf("Publish acknowledged: %d \n",packetId);
}
