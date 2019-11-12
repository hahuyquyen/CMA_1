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

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  
  printf("Publish received: %s \n",topic);
  printf("QOS: %c \n", properties.qos);
  printf("noi dung: %s \n", payload);
  if (strcmp(WiFiConf.mqtt_subto1,topic) == 0){printf("vung 1 \n");
  xQueueSend( Queue_display, &Display_NV, ( TickType_t ) 2  );
  }
  if (strcmp(WiFiConf.mqtt_subto2,topic) == 0){printf("vung 2 \n");}
  if (strcmp(WiFiConf.mqtt_subto3,topic) == 0){printf("vung 3 \n");}
}

void onMqttPublish(uint16_t packetId) {
 printf("Publish acknowledged: %d \n",packetId);
}

