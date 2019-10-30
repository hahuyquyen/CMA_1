String processor( const String& var){
  char __dataFileName[sizeof(var)];
  var.toCharArray(__dataFileName, sizeof(__dataFileName));
 // printf(__dataFileName);
 // printf("\n");
  if(var == "SSID"){return String(WiFiConf.sta_ssid);}
  else if(var == "PASS"){return String(WiFiConf.sta_pwd);}
  else if(var == "IP"){ return String(WiFiConf.sta_ip);}
  else if(var == "GATEWAY"){return String(WiFiConf.sta_gateway);}
  else if(var == "SUB"){return String(WiFiConf.sta_subnet);}
  else if(var == "ServerMQTT"){return String(WiFiConf.mqtt_server);}
  else if(var == "PortMQTT"){return String(WiFiConf.mqtt_port);}
  else if(var == "USERMQTT"){return String(WiFiConf.mqtt_user);}
  else if(var == "PASSMQTT"){return String(WiFiConf.mqtt_pass);}
  else if(var == "SUBTopic1"){return String(WiFiConf.mqtt_subto1);}
  else if(var == "SUBTopic2"){return String(WiFiConf.mqtt_subto2);}
  else if(var == "SUBTopic3"){return String(WiFiConf.mqtt_subto3);}
  else if(var == "CHOOSEDHCP"){
  if (atoi(WiFiConf.choose_dhcp) == 1) return String(1);else return String(0);
  }
  return String();
}
void handleUpdate(AsyncWebServerRequest *request) {
  char* html = "<form method='POST' action='/doUpdate' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
  request->send(200, "text/html", html);
}

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
      Serial.println("Update complete");
      Serial.flush();
      ESP.restart();
    }
  }
}


void setupWiFiConf(void) {
    server.on("/2.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/2.css", "text/css");
    });
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
    });
    server.on("/wifi_conf", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/wifi_conf.html", String(), false, processor);
    });
    server.on("/mqtt_conf", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/mqtt_conf.html", String(), false, processor);
    });
    server.on("/set_wifi_conf", HTTP_POST, [](AsyncWebServerRequest *request){
        if (request->hasParam("html_ssid", true)) {
          String ss1 = request->getParam("html_ssid", true)->value();
           ss1.trim();
          ss1.toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
        } 
        if (request->hasParam("html_wpa_password", true)) {
           String ss2 = request->getParam("html_wpa_password", true)->value();
            ss2.trim();
          ss2.toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
        } 
        if (request->hasParam("html_ip_wifi", true)) {
           String ss3 = request->getParam("html_ip_wifi", true)->value();
            ss3.trim();
          ss3.toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
        } 
        if (request->hasParam("html_gateway_wifi", true)) {
           String ss4 = request->getParam("html_gateway_wifi", true)->value();
            ss4.trim();
          ss4.toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
        } 
        if (request->hasParam("html_sub_wifi", true)) {
           String ss5 = request->getParam("html_sub_wifi", true)->value();
           ss5.trim();
          ss5.toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));
        } 
        if (request->hasParam("button", true)) {
          String data1 = request->getParam("button", true)->value();
          data1.trim();
          data1.toCharArray(WiFiConf.choose_dhcp, sizeof(WiFiConf.choose_dhcp));
          printf(WiFiConf.choose_dhcp);
        } 
        saveWiFiConf();
        request->send(200, "text/plain", "OK BABY");
    });
    server.on("/set_mqtt_conf", HTTP_POST, [](AsyncWebServerRequest *request){
        if (request->hasParam("ServerMQTT", true)) {
          String ss1 = request->getParam("ServerMQTT", true)->value();
           ss1.trim();
          ss1.toCharArray(WiFiConf.mqtt_server, sizeof(WiFiConf.mqtt_server));
        } 
        if (request->hasParam("PortMQTT", true)) {
           String ss2 = request->getParam("PortMQTT", true)->value();
            
            ss2.trim();
            ss2.toCharArray(WiFiConf.mqtt_port, sizeof(WiFiConf.mqtt_port));
          //  uint16_t port_tam = atol(WiFiConf.mqtt_port);
         //   printf("Port MQTT %ul \n",port_tam);
            // printf("
        } 
        if (request->hasParam("USERMQTT", true)) {
           String ss3 = request->getParam("USERMQTT", true)->value();
            ss3.trim();
          ss3.toCharArray(WiFiConf.mqtt_user, sizeof(WiFiConf.mqtt_user));
        } 
        if (request->hasParam("PASSMQTT", true)) {
           String ss4 = request->getParam("PASSMQTT", true)->value();
            ss4.trim();
          ss4.toCharArray(WiFiConf.mqtt_pass, sizeof(WiFiConf.mqtt_pass));
        } 
        if (request->hasParam("SUBTopic1", true)) {
           String ss5 = request->getParam("SUBTopic1", true)->value();
           ss5.trim();
          ss5.toCharArray(WiFiConf.mqtt_subto1, sizeof(WiFiConf.mqtt_subto1));
        } 
        if (request->hasParam("SUBTopic2", true)) {
          String data1 = request->getParam("SUBTopic2", true)->value();
          data1.trim();
          data1.toCharArray(WiFiConf.mqtt_subto2, sizeof(WiFiConf.mqtt_subto2));
        } 
        if (request->hasParam("SUBTopic3", true)) {
          String data2 = request->getParam("SUBTopic3", true)->value();
          data2.trim();
          data2.toCharArray(WiFiConf.mqtt_subto3, sizeof(WiFiConf.mqtt_subto3));
        } 
        saveWiFiConf();
        request->send(200, "text/plain", "Hello, POST: ");
    });
    
  server.on("/firmware", HTTP_GET, [](AsyncWebServerRequest *request){handleUpdate(request);});
  server.on("/doUpdate", HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                  size_t len, bool final) {handleDoUpdate(request, filename, index, data, len, final);}
  );
}
