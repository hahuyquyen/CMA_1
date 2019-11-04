 /*
  * 
  * 
  * 
QoS
Ở đây có 3 tuỳ chọn *QoS (Qualities of service) * khi "publish" và "subscribe":

QoS0 Broker/client sẽ gởi dữ liệu đúng 1 lần, quá trình gởi được xác nhận bởi chỉ giao thức TCP/IP, giống kiểu đem con bỏ chợ.
QoS1 Broker/client sẽ gởi dữ liệu với ít nhất 1 lần xác nhận từ đầu kia, nghĩa là có thể có nhiều hơn 1 lần xác nhận đã nhận được dữ liệu.
QoS2 Broker/client đảm bảm khi gởi dữ liệu thì phía nhận chỉ nhận được đúng 1 lần, quá trình này phải trải qua 4 bước bắt tay.
  */
 
 
 void connectToMqtt() {
  mqttClient.connect();
}
void truyen_mqtt(){
      size_t size_needed = snprintf(NULL, 0, "{\"id\":\"%s\",\"device\":\"%s\",\"data\":[%g,%g]}", datatruyen_mqtt.id_RFID, WiFiConf.mqtt_choose_inout, datatruyen_mqtt.data_weight,datatruyen_mqtt.data_tare) + 1;
      char* msg1 = (char*)malloc(size_needed);
      if (msg1 != NULL) {  // malloc ok
       sprintf(msg1, "{\"id\":\"%s\",\"device\":\"%s\",\"data\":[%g,%g]}", datatruyen_mqtt.id_RFID,WiFiConf.mqtt_choose_inout, datatruyen_mqtt.data_weight,datatruyen_mqtt.data_tare) ;
       printf("mqtt %s\n",msg1);
       uint16_t packetIdPub1 = mqttClient.publish(datatruyen_mqtt.id_RFID, 2, true, msg1);
      //  printf("Publishing at QoS 1, packetId: %f\n",packetIdPub1);
      }
      else {printf("Ko du heap, reset\n");ESP.restart(); }
      free(msg1);
}
void onMqttConnect(bool sessionPresent) {
        if (WiFiConf.mqtt_subto1[0] != 'x'){mqttClient.subscribe( WiFiConf.mqtt_subto1,2 );}  //0,1,2 laf qos
        if (WiFiConf.mqtt_subto2[0] != 'x'){mqttClient.subscribe(WiFiConf.mqtt_subto2,2);}
        if (WiFiConf.mqtt_subto3[0] != 'x'){mqttClient.subscribe(WiFiConf.mqtt_subto3,2);}  
  
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
 // printf("Disconnected to MQTT.\n");
  if (WiFi.isConnected()) {xTimerStart(mqttReconnectTimer, 0);
  }
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
