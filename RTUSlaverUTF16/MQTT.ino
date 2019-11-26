 void connectToMqtt() {
  mqttClient.connect();
}
void truyen_mqtt(){
}
void onMqttConnect(bool sessionPresent) {
    //    status_mqtt_connect = true;
        mqttClient.subscribe( "hb",0 ); 
        mqttClient.subscribe( "fcg",0 ); 
        if (WiFiConf.mqtt_subto1[0] != 'x'){mqttClient.subscribe( WiFiConf.mqtt_subto1,0 );}  //0,1,2 laf qos
        if (WiFiConf.mqtt_subto2[0] != 'x'){mqttClient.subscribe(WiFiConf.mqtt_subto2,0);}
        if (WiFiConf.mqtt_subto3[0] != 'x'){mqttClient.subscribe(WiFiConf.mqtt_subto3,0);}  
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {

}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {//printf("Subscribe acknowledged: %f\n",packetId);
}

void onMqttUnsubscribe(uint16_t packetId) {// printf("Unsubscribe acknowledged: %f\n",packetId);
}



void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  printf("MQTT GET: %s \n",topic);
  StaticJsonDocument<2500> jsonBuffer;
  DeserializationError error = deserializeJson(jsonBuffer,payload);
  if (error) Serial.println("error json");
  if (strcmp(WiFiConf.mqtt_subto1,topic) == 0){
      strlcpy(mqttData.nameNvUtf8, jsonBuffer["name"], sizeof(mqttData.nameNvUtf8));
      wmemset(modbusData.nameNvUtf16, 0x0000, sizeof(modbusData.nameNvUtf16) / 2);
      size_t valuelen = utf8towchar(mqttData.nameNvUtf8, SIZE_MAX, modbusData.nameNvUtf16, sizeof(modbusData.nameNvUtf16));
      for (int j = 0 ; j < sizeof(modbusData.nameNvUtf16) / 2 ; j++) {mb.Hreg(REGN + j, modbusData.nameNvUtf16[j]);}
      time_cho = xTaskGetTickCount();
      scenes = 11;mb.Hreg(300, scenes);
      memset(mqttData.nameNvUtf8, '\0', sizeof(mqttData.nameNvUtf8));
  }
}

void onMqttPublish(uint16_t packetId) {
 printf("Publish acknowledged: %d \n",packetId);
}
