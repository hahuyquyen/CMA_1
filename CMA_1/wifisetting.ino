

void wifiOnDisconnect()
{   uint16_t* time_blink = (uint16_t*)malloc(sizeof(uint16_t));
    *time_blink = 200;
    xQueueSend(Queue_Time_blink, time_blink, (TickType_t) 1);
    free(time_blink);            
    xTimerStop(mqttReconnectTimer, 0); // tat tu dong ket noi mqtt
    status_wifi_connect_AP = false ; 
}
void wifigotip()
{            
            status_wifi_connect_AP = true;
            counter_wifi_disconnect=0;
            WiFi.softAPdisconnect(true);
            printf("Wifi %s\n",WiFi.localIP().toString().c_str());
            uint16_t* time_blink = (uint16_t*)malloc(sizeof(uint16_t));
            *time_blink = 2000;
            xQueueSend(Queue_Time_blink, time_blink, (TickType_t) 1);    
            free(time_blink);
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
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) { printf("STA Failed to configure");}
  //}
}
