void wifiOnDisconnect()
{
  Serial.println("STA Disconnected");
  delay(1000);
  wifi_connect(WIFI_AP_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
}
void WiFiEvent(WiFiEvent_t event)
{
   // Serial.printf("[WiFi-event] event: %d\n", event);

    switch(event) {
              case SYSTEM_EVENT_STA_GOT_IP:
                  Serial.println("WiFi connected");
                  break;
              case SYSTEM_EVENT_STA_DISCONNECTED:
                  Serial.println("WiFi lost connection");
                  WiFi.reconnect();
                  wifiOnDisconnect();
                  break;
              case SYSTEM_EVENT_STA_START:
                Serial.println("ESP32 station start");
                break;
              case SYSTEM_EVENT_STA_CONNECTED:
                WiFi.softAPdisconnect(true);
                Serial.println("ESP32 station connected to AP");
                break;
              default: break;
    }
}

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
    WiFi.softAP(ap_ssid,"88888888");
    WiFi.begin(ssid, password);
}

void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in){
  
    parseBytes1(ip_in, '.', 1, 4, 10);
    parseBytes1(gateway_in, '.', 2, 4, 10);
    parseBytes1(subnet_in, '.', 3, 4, 10);
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }
    WiFi.onEvent(WiFiEvent);
}
