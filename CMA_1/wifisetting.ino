/*
 Trang ma loi wifi

 https://demo-dijiudu.readthedocs.io/en/latest/api-guides/wifi.html
 [AsyncTCP.cpp:953] _poll(): 0x3ffe16e0 != 0x3ffe2678
 */
//////////////////////////////////////////////////////////////////
////// even wifi mat ket noi ////////////////////////////
//////////////////////////////////////////////////////////////////
void wifiOnDisconnect(WiFiEventInfo_t info)
{ uint16_t* time_blink = (uint16_t*)malloc(sizeof(uint16_t));
  *time_blink = 200;
  xQueueSend(Queue_Time_blink, time_blink, (TickType_t) 1);
  free(time_blink);
  xTimerStop(mqttReconnectTimer, 0); // tat tu dong ket noi mqtt
  statusPeripheral.wifi.statusConnectAP = false ;
   if (info.disconnected.reason == 6) {
      Serial.println("NOT_AUTHED reconnect");
      WiFi.reconnect();
   }
   else if (info.disconnected.reason == 8) {
      Serial.println("assoc leave");
      wifi_connect(0, WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
   }
   /*
    * reason 201 la no AP found
    */
}
//////////////////////////////////////////////////////////////////
////// Event wifi nhan IP ////////////////////////////
//////////////////////////////////////////////////////////////////
void wifigotip()
{
  if (statusPeripheral.wifi.ApConnect){
    //WiFi.disconnect(true);
    WiFi.softAPdisconnect();
    wifi_connect(0, WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
    return;
  }
  statusPeripheral.wifi.statusConnectAP = true;
  statusPeripheral.wifi.counterWifiDisconnect = 0;
  //
  printf("Wifi %s\n", WiFi.localIP().toString().c_str());
  uint16_t* time_blink = (uint16_t*)malloc(sizeof(uint16_t));
  *time_blink = 2000;
  xQueueSend(Queue_Time_blink, time_blink, (TickType_t) 1);
  free(time_blink);
  connectToMqtt();
}
//////////////////////////////////////////////////////////////////
////// event trang thai wifi ////////////////////////////
//////////////////////////////////////////////////////////////////
void WiFiEvent(WiFiEvent_t event)
{
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
  }
}
//////////////////////////////////////////////////////////////////
////// stach data ip ////////////////////////////
//////////////////////////////////////////////////////////////////
void parseBytes1(const char* str, char sep, int address, int maxBytes, int base) {
  for (int i = 0; i < maxBytes; i++) {
    if (address == 1) local_IP[i] = strtoul(str, NULL, base);  
    else if (address == 2) gateway[i] = strtoul(str, NULL, base);  
    else if (address == 3) subnet[i] = strtoul(str, NULL, base); 
    str = strchr(str, sep);           
    if (str == NULL || *str == '\0') {
      break;                            
    }
    str++;                               
  }
}
//////////////////////////////////////////////////////////////////
////// Cai dat che do wifi ////////////////////////////
//////////////////////////////////////////////////////////////////
void wifi_connect(byte _mode, wifi_mode_t wifi_mode, char *ssid, char *password, char *ap_ssid) {
  WiFi.disconnect(true);///
  WiFi.mode(wifi_mode);
  if (_mode == 0) {
    statusPeripheral.wifi.ApConnect = false;
    WiFi.begin(ssid, password); //mode STA
  }
  else   if (_mode == 1) {
    statusPeripheral.wifi.ApConnect = true;
    WiFi.softAP(ap_ssid, "12345678"); //mode AP
  }
  else {
    WiFi.begin(ssid, password);
    statusPeripheral.wifi.ApConnect = true;
    WiFi.softAP(ap_ssid, "12345678");
  }
}
//////////////////////////////////////////////////////////////////
////// Cai dat static ip cho wifi  ////////////////////////////
//////////////////////////////////////////////////////////////////
void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in) {
  //if (atoi(WiFiConf.choose_dhcp) == 0){
  parseBytes1(ip_in, '.', 1, 4, 10);
  parseBytes1(gateway_in, '.', 2, 4, 10);
  parseBytes1(subnet_in, '.', 3, 4, 10);
  // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) { printf("STA Failed to configure");}
  //}
}
