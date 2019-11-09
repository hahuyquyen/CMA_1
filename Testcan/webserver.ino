

String processor( const String& var){
  char __dataFileName[sizeof(var)];
  var.toCharArray(__dataFileName, sizeof(__dataFileName));
  if(var == F("SSID")){return String(WiFiConf.sta_ssid);}
  else if(var == F("PASS")){return String(WiFiConf.sta_pwd);}
  else if(var == F("IP")){ return String(WiFiConf.sta_ip);}
  else if(var == F("GATEWAY")){return String(WiFiConf.sta_gateway);}
  else if(var == F("SUB")){return String(WiFiConf.sta_subnet);}
  else if(var == F("ServerMQTT")){return String(WiFiConf.mqtt_server);}
  else if(var == F("PortMQTT")){return String(WiFiConf.mqtt_port);}
  else if(var == F("USERMQTT")){return String(WiFiConf.mqtt_user);}
  else if(var == F("PASSMQTT")){return String(WiFiConf.mqtt_pass);}
  else if(var == F("SUBTopic1")){return String(WiFiConf.mqtt_subto1);}
  else if(var == F("SUBTopic2")){return String(WiFiConf.mqtt_subto2);}
  else if(var == F("SUBTopic3")){return String(WiFiConf.mqtt_subto3);}
  else if(var == F("CHOOINOUT")){return String(WiFiConf.mqtt_choose_inout);}
  else if(var == F("CHOOSEDHCP")){return atoi(WiFiConf.choose_dhcp) == 1?String(1):String(0);}

  return String();
}
/*
void handleUpdate(AsyncWebServerRequest *request) {
  char* html = (char *)"<form method='POST' action='/doUpdate' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
  request->send(200, F("text/html"), html);
}
*/
void handleDoUpdate(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
 
  if (!index){
  //  Serial.println("Update");
    content_len = request->contentLength();
    // if filename includes spiffs, update the spiffs partition
    int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len) {
    Update.printError(Serial);
  }

  if (final) {
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
    response->addHeader("Refresh", "20");  
    response->addHeader("Location", "/");
    request->send(response);
    if (!Update.end(true)){
      Update.printError(Serial);
    } else {
      ESP.restart();
    }
  }
}


void setupWiFiConf(void) {
    server.on("/2.css", HTTP_GET, [](AsyncWebServerRequest *request){request->send(SPIFFS, "/2.css", F("text/css"));});
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send(SPIFFS, F("/index.html"));});
    server.on("/wifi_conf", HTTP_GET, [](AsyncWebServerRequest *request){request->send(SPIFFS, F("/wifi_conf.html"), String(), false, processor);});
    server.on("/mqtt_conf", HTTP_GET, [](AsyncWebServerRequest *request){request->send(SPIFFS, F("/mqtt_conf.html"), String(), false, processor);});
    server.on("/set_wifi_conf", HTTP_POST, [](AsyncWebServerRequest *request){
        if (request->hasParam("html_ssid", true)) { request->getParam("html_ssid", true)->value().toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));} 
        if (request->hasParam("html_wpa_password", true)) {request->getParam("html_wpa_password", true)->value().toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));} 
        if (request->hasParam("html_ip_wifi", true)) { request->getParam("html_ip_wifi", true)->value().toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));} 
        if (request->hasParam("html_gateway_wifi", true)) {request->getParam("html_gateway_wifi", true)->value().toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));} 
        if (request->hasParam("html_sub_wifi", true)) {request->getParam("html_sub_wifi", true)->value().toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));} 
        if (request->hasParam("button", true)) {request->getParam("button", true)->value().toCharArray(WiFiConf.choose_dhcp, sizeof(WiFiConf.choose_dhcp));} 
        if (request->hasParam("chooseinout", true)) {request->getParam("chooseinout", true)->value().toCharArray(WiFiConf.mqtt_choose_inout, sizeof(WiFiConf.mqtt_choose_inout));} 
        saveWiFiConf();
        request->send(200, F("text/plain"), F("OK BABY"));
    });
    server.on("/set_mqtt_conf", HTTP_POST, [](AsyncWebServerRequest *request){
        if (request->hasParam(F("ServerMQTT"), true)) {request->getParam("ServerMQTT", true)->value().toCharArray(WiFiConf.mqtt_server, sizeof(WiFiConf.mqtt_server));} 
        if (request->hasParam("PortMQTT", true)) {request->getParam("PortMQTT", true)->value().toCharArray(WiFiConf.mqtt_port, sizeof(WiFiConf.mqtt_port));} 
        if (request->hasParam("USERMQTT", true)) {request->getParam("USERMQTT", true)->value().toCharArray(WiFiConf.mqtt_user, sizeof(WiFiConf.mqtt_user)); } 
        if (request->hasParam("PASSMQTT", true)) {request->getParam("PASSMQTT", true)->value().toCharArray(WiFiConf.mqtt_pass, sizeof(WiFiConf.mqtt_pass));} 
        if (request->hasParam("SUBTopic1", true)) {request->getParam("SUBTopic1", true)->value().toCharArray(WiFiConf.mqtt_subto1, sizeof(WiFiConf.mqtt_subto1));} 
        if (request->hasParam("SUBTopic2", true)) {request->getParam("SUBTopic2", true)->value().toCharArray(WiFiConf.mqtt_subto2, sizeof(WiFiConf.mqtt_subto2));} 
        if (request->hasParam("SUBTopic3", true)) {request->getParam("SUBTopic3", true)->value().toCharArray(WiFiConf.mqtt_subto3, sizeof(WiFiConf.mqtt_subto3));} 
        saveWiFiConf();
        request->send(200, F("text/plain"), F("OK"));
    });
    
    server.on("/firmware", HTTP_GET, [](AsyncWebServerRequest *request){request->send(SPIFFS, F("/update.html"));});
   // server.on("/firmware", HTTP_GET, [](AsyncWebServerRequest *request){request->send(SPIFFS, F("/index.html"));});
    server.on("/doUpdate", HTTP_POST,[](AsyncWebServerRequest *request) {},
          [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                  size_t len, bool final) {handleDoUpdate(request, filename, index, data, len, final);});
    server.on("/Reboot", HTTP_GET, [](AsyncWebServerRequest *request){request->send(200, F("text/html"), F("OK.. Rebooting"));vTaskDelay(1000);ESP.restart();});
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, F("/favicon.ico"), F("image/png"));
});
}
