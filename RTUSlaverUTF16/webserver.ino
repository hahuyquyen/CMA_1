
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
    size_t content_len = request->contentLength();
    // if filename includes spiffs, update the spiffs partition
    int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
      Update.printError(Serial);
    }
  }
  /*  if (!index) {
      writeEvent("INFO", "updt", "Firmware update started", filename.c_str());
      Update.runAsync(true);
      if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) {
        writeEvent("ERRO", "updt", "Not enough space to update","");
        #ifdef DEBUG
        Update.printError(Serial);
        #endif
      }
    }*/
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
        if (request->hasParam(F("html_ssid"), true)) { request->getParam(F("html_ssid"), true)->value().toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));} 
        if (request->hasParam(F("html_wpa_password"), true)) {request->getParam(F("html_wpa_password"), true)->value().toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));} 
        if (request->hasParam(F("html_ip_wifi"), true)) { request->getParam(F("html_ip_wifi"), true)->value().toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));} 
        if (request->hasParam(F("html_gateway_wifi"), true)) {request->getParam(F("html_gateway_wifi"), true)->value().toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));} 
        if (request->hasParam(F("html_sub_wifi"), true)) {request->getParam(F("html_sub_wifi"), true)->value().toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));} 
        if (request->hasParam(F("button"), true)) {request->getParam(F("button"), true)->value().toCharArray(WiFiConf.choose_dhcp, sizeof(WiFiConf.choose_dhcp));} 
        if (request->hasParam(F("chooseinout"), true)) {request->getParam(F("chooseinout"), true)->value().toCharArray(WiFiConf.mqtt_choose_inout, sizeof(WiFiConf.mqtt_choose_inout));} 
        saveWiFiConf();
        request->send(200, F("text/plain"), F("OK BABY"));
    });
    server.on("/set_id", HTTP_POST, [](AsyncWebServerRequest *request){
        if (request->hasParam("id", true)) { 
         /* Serial.println(request->getParam("id", true)->value());
          idDevice = strtoul(request->getParam("id", true)->value().c_str(), NULL, 10);
          EEPROM.writeUInt(800, idDevice);
          EEPROM.commit();*/
          } 
        request->send(200, F("text/plain"), F("OK ...."));
    });
    server.on("/set_mqtt_conf", HTTP_POST, [](AsyncWebServerRequest *request){
        if (request->hasParam(F("ServerMQTT"), true)) {request->getParam("ServerMQTT", true)->value().toCharArray(WiFiConf.mqtt_server, sizeof(WiFiConf.mqtt_server));} 
        if (request->hasParam(F("PortMQTT"), true)) {request->getParam(F("PortMQTT"), true)->value().toCharArray(WiFiConf.mqtt_port, sizeof(WiFiConf.mqtt_port));} 
        if (request->hasParam(F("USERMQTT"), true)) {request->getParam(F("USERMQTT"), true)->value().toCharArray(WiFiConf.mqtt_user, sizeof(WiFiConf.mqtt_user)); } 
        if (request->hasParam(F("PASSMQTT"), true)) {request->getParam(F("PASSMQTT"), true)->value().toCharArray(WiFiConf.mqtt_pass, sizeof(WiFiConf.mqtt_pass));} 
        if (request->hasParam(F("SUBTopic1"), true)) {request->getParam(F("SUBTopic1"), true)->value().toCharArray(WiFiConf.mqtt_subto1, sizeof(WiFiConf.mqtt_subto1));} 
        if (request->hasParam(F("SUBTopic2"), true)) {request->getParam(F("SUBTopic2"), true)->value().toCharArray(WiFiConf.mqtt_subto2, sizeof(WiFiConf.mqtt_subto2));} 
        if (request->hasParam(F("SUBTopic3"), true)) {request->getParam(F("SUBTopic3"), true)->value().toCharArray(WiFiConf.mqtt_subto3, sizeof(WiFiConf.mqtt_subto3));} 
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


void wifiOnDisconnect()
{   //uint16_t* time_blink = (uint16_t*)malloc(sizeof(uint16_t));
 //  *time_blink = 200;
//    xQueueSend(Queue_Time_blink, time_blink, (TickType_t) 1);
 //   free(time_blink);            
    xTimerStop(mqttReconnectTimer, 0); // tat tu dong ket noi mqtt
    scenes = idHmiWifiDisConnect;
     mb.Hreg(399, scenes);
    statusWifiConnect = 0 ; 
}
void wifigotip()
{            
            statusWifiConnect = 1;
            intCounterWifi=0;
            WiFi.softAPdisconnect(true);
            printf("Wifi %s\n",WiFi.localIP().toString().c_str());
            mb.Hreg(416 , WiFi.localIP()[0]);
            mb.Hreg(417 , WiFi.localIP()[1]);
            mb.Hreg(418 , WiFi.localIP()[2]);
            mb.Hreg(419 , WiFi.localIP()[3]);
         scenes = idHmiMainPage;
         mb.Hreg(399, 0);
         connectToMqtt();
}
void WiFiEvent(WiFiEvent_t event)
{
   // Serial.printf("[WiFi-event] event: %d\n", event);

       switch (event) {
        case SYSTEM_EVENT_WIFI_READY: //printf("WiFi interface ready\n");
            break;
        case SYSTEM_EVENT_STA_START: //printf("WiFi client started\n");
            break;
        case SYSTEM_EVENT_STA_STOP: // printf("Stop Wifi\n");
            break;
        case SYSTEM_EVENT_STA_CONNECTED: //printf("Connected to AP\n"); 
            break;
        default: break;
    }}

void parseBytes1(const char* str, char sep, int address, int maxBytes, int base) {
  for (int i = 0; i < maxBytes; i++) {
    if (address == 1) local_IP[i] = strtoul(str, NULL, base);  // Convert byte
    else if (address == 2) gateway[i] = strtoul(str, NULL, base);  // Convert byte
    else if (address == 3) subnet[i] = strtoul(str, NULL, base);  // Convert byt
    str = strchr(str, sep);               // Find next separator
    if (str == NULL || *str == '\0') {
      break;                            // No more separators, exit
    }
    str++;                                // Point to next character after separator
  }
}

void wifi_connect(byte _mode,wifi_mode_t wifi_mode,char *ssid,char *password,char *ap_ssid){
  WiFi.mode(wifi_mode);
  if (_mode ==0){WiFi.begin(ssid,password);} //mode STA
  else   if (_mode ==1){WiFi.softAP(ap_ssid,"12345678");} //mode AP
  else{
    WiFi.begin(ssid,password);
    WiFi.softAP(ap_ssid,"12345678");
  }   
}
void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in){
  //if (atoi(WiFiConf.choose_dhcp) == 0){
    parseBytes1(ip_in, '.', 1, 4, 10);
    parseBytes1(gateway_in, '.', 2, 4, 10);
    parseBytes1(subnet_in, '.', 3, 4, 10);
   // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) { printf("STA Failed to configure");}
  //}
}
