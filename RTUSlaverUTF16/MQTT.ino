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

}

void onMqttPublish(uint16_t packetId) {
 printf("Publish acknowledged: %d \n",packetId);
}
