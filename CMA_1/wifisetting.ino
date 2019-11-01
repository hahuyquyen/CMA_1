

void wifiOnDisconnect()
{
  printf("STA Disconnected\n");
  vTaskDelay(1000);
  wifi_connect(WIFI_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
}
void WiFiEvent(WiFiEvent_t event)
{
   // Serial.printf("[WiFi-event] event: %d\n", event);

       switch (event) {
        case SYSTEM_EVENT_WIFI_READY: 
            printf("WiFi interface ready\n");
            break;
        case SYSTEM_EVENT_STA_START:
            printf("WiFi client started\n");
            break;
        case SYSTEM_EVENT_STA_STOP:
            printf("Stop Wifi\n");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            printf("Connected to AP\n");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            printf("Wifi %s\n",WiFi.localIP().toString().c_str());
            break;
        case SYSTEM_EVENT_AP_START:
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

void wifi_connect(wifi_mode_t wifi_mode,char *ssid,char *password,char *ap_ssid){
    WiFi.mode(wifi_mode);
    WiFi.begin(ssid,password);
}
void wifi_AP(wifi_mode_t wifi_mode,char *ssid,char *password){
    
    WiFi.mode(wifi_mode);
    WiFi.softAP(ssid,password);
}
void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in){
  //if (atoi(WiFiConf.choose_dhcp) == 0){
    parseBytes1(ip_in, '.', 1, 4, 10);
    parseBytes1(gateway_in, '.', 2, 4, 10);
    parseBytes1(subnet_in, '.', 3, 4, 10);
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) { printf("STA Failed to configure");}
  //}
}
