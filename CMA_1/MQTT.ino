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
 
 
 void connectToMqtt() {
  mqttClient.connect();
}
void truyen_mqtt(){
      size_t size_needed = snprintf(NULL, 0, "{\"id\":\"%s\",\"device\":\"%s\",\"data\":[%g,0]}", datatruyen_mqtt.id_RFID, datatruyen_mqtt.id_RFID_NV, datatruyen_mqtt.data_weight) + 1;
      char* msg1 = (char*)malloc(size_needed);
      if (msg1 != NULL) {  // malloc ok
       sprintf(msg1, "{\"id\":\"%s\",\"device\":\"%s\",\"data\":[%g,0]}", datatruyen_mqtt.id_RFID, datatruyen_mqtt.id_RFID_NV, datatruyen_mqtt.data_weight) ;
       printf("mqtt %s\n",msg1);
       uint16_t packetIdPub1 = mqttClient.publish(datatruyen_mqtt.id_RFID, 2, true, msg1);
      //  printf("Publishing at QoS 1, packetId: %f\n",packetIdPub1);
      }
      else {printf("Ko du heap, reset\n");ESP.restart(); }
      free(msg1);
}
void onMqttConnect(bool sessionPresent) {
  status_mqtt_connect = true;
        if (WiFiConf.mqtt_subto1[0] != 'x'){mqttClient.subscribe( WiFiConf.mqtt_subto1,2 );}  //0,1,2 laf qos
        if (WiFiConf.mqtt_subto2[0] != 'x'){mqttClient.subscribe(WiFiConf.mqtt_subto2,2);}
        if (WiFiConf.mqtt_subto3[0] != 'x'){mqttClient.subscribe(WiFiConf.mqtt_subto3,2);}  
  
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
 // printf("Disconnected to MQTT.\n");
 status_mqtt_connect = false;
  if (WiFi.isConnected()) {xTimerStart(mqttReconnectTimer, 0);}
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {//printf("Subscribe acknowledged: %f\n",packetId);
}

void onMqttUnsubscribe(uint16_t packetId) {// printf("Unsubscribe acknowledged: %f\n",packetId);
}

<<<<<<< HEAD
=======

/*{"type":"0","data":[5,1,3,6,8,9]}  
 * {"type":"3","l":"3","data":[{"i":"5455","N":"pham an nhàn há há 7"},{"i":"68","N":"nhàn 8"},{"i":"98","N":"nhàn 9"}]}
 * 
 * {"type":"3","l":"3","data":[{"i":"5455","N":"nhàn 1"},{"i":"68","N":"nhàn 2"},{"i":"98","N":"nhàn 3"}]}
 */
>>>>>>> 1da45cf975ec47940cf18224b4068f2bba99fb0a
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  
  printf("Publish received: %s \n",topic);
  printf("QOS: %c \n", properties.qos);
  printf("noi dung: %s \n", payload);
<<<<<<< HEAD
  if (strcmp(WiFiConf.mqtt_subto1,topic) == 0){printf("vung 1 \n");
  xQueueSend( Queue_display, &Display_NV, ( TickType_t ) 2  );
=======
  if (strcmp(WiFiConf.mqtt_subto1,topic) == 0){
        StaticJsonDocument<1500> jsonBuffer;
        DeserializationError error = deserializeJson(jsonBuffer,payload);
        if (error) Serial.println("error json");
        if (jsonBuffer["type"].as<uint8_t>() == 1){
          Serial.print("Nhan loai Loai Ca : ");
          chonloaica.SL_LoaiCa=jsonBuffer["l"].as<uint8_t>();
          strlcpy(Nha_SX.Loai_ca[0], "Chưa Chọn", sizeof(Nha_SX.Loai_ca[0]));
          for (int i=0;i<jsonBuffer["l"].as<uint8_t>();i++){
            chonloaica.STT_LoaiCa[i+1]=jsonBuffer["data"][i]["i"].as<uint32_t>();
            strlcpy(Nha_SX.Loai_ca[i+1], jsonBuffer["data"][i]["N"], sizeof(Nha_SX.Loai_ca[i]));
          }          
        }
        else if (jsonBuffer["type"].as<uint8_t>() == 2){
           strlcpy(Nha_SX.So_Lo[0], "Chưa Chọn", sizeof(Nha_SX.So_Lo[0]));
          Serial.print("Nhan loai HD : ");
          Serial.println(jsonBuffer["data"][1].as<uint8_t>());
          chonloaica.SL_NhaCC=jsonBuffer["l"].as<uint8_t>();
          for (int i=0;i<chonloaica.SL_NhaCC;i++){
            chonloaica.STT_NhaCC[i+1]=jsonBuffer["data"][i].as<uint32_t>();
            strlcpy(Nha_SX.So_Lo[i+1], jsonBuffer["data"][i]["N"], sizeof(Nha_SX.So_Lo[i]));
          }   
        }
        else if (jsonBuffer["type"].as<uint8_t>() == 3){
           strlcpy(Nha_SX.Thanh_Pham[0], "Chưa Chọn", sizeof(Nha_SX.Thanh_Pham[0]));
          Serial.print("Nhan loai Loai Ca : ");
          Serial.println(jsonBuffer["l"].as<uint8_t>());
          chonloaica.SL_ThanhPham=jsonBuffer["l"].as<uint8_t>();
          for (int i=0;i<jsonBuffer["l"].as<uint8_t>();i++){
            chonloaica.STT_ThanhPham[i+1]=jsonBuffer["data"][i]["i"].as<uint32_t>();
            strlcpy(Nha_SX.Thanh_Pham[i+1], jsonBuffer["data"][i]["N"], sizeof(Nha_SX.Thanh_Pham[i+1]));
          }
        }  
>>>>>>> 1da45cf975ec47940cf18224b4068f2bba99fb0a
  }
  if (strcmp(WiFiConf.mqtt_subto2,topic) == 0){printf("vung 2 \n");}
  if (strcmp(WiFiConf.mqtt_subto3,topic) == 0){printf("vung 3 \n");}
}

void onMqttPublish(uint16_t packetId) {
 printf("Publish acknowledged: %d \n",packetId);
}

