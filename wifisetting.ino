void wifiOnDisconnect()
{
  printf("STA Disconnected");
  delay(1000);
  wifi_connect(WIFI_AP_STA,WiFiConf.sta_ssid,WiFiConf.sta_pwd,WiFiConf.ap_ssid);
}
void WiFiEvent(WiFiEvent_t event)
{
   // Serial.printf("[WiFi-event] event: %d\n", event);

       switch (event) {
        case SYSTEM_EVENT_WIFI_READY: 
            printf("WiFi interface ready\n");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            printf("Completed scan for access points\n");
            break;
        case SYSTEM_EVENT_STA_START:
            printf("WiFi client started\n");
            break;
        case SYSTEM_EVENT_STA_STOP:
            printf("WiFi clients stopped\n");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            printf("Connected to access point\n");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            printf("Disconnected from WiFi access point\n");
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            printf("Authentication mode of access point has changed\n");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            printf("Obtained IP address:\n ");
          //  printf(WiFi.localIP());
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            printf("Lost IP address and IP address is reset to 0\n");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            printf("WiFi Protected Setup (WPS): succeeded in enrollee mode\n");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            printf("WiFi Protected Setup (WPS): failed in enrollee mode\n");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            printf("WiFi Protected Setup (WPS): timeout in enrollee mode\n");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            printf("WiFi Protected Setup (WPS): pin code in enrollee mode\n");
            break;
        case SYSTEM_EVENT_AP_START:
            printf("WiFi access point started\n");
            break;
        case SYSTEM_EVENT_AP_STOP:
            printf("WiFi access point  stopped\n");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            printf("Client connected\n");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            printf("Client disconnected\n");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            printf("Assigned IP address to client\n");
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            printf("Received probe request\n");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            printf("IPv6 is preferred\n");
            break;
        case SYSTEM_EVENT_ETH_START:
            printf("Ethernet started\n");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            printf("Ethernet stopped\n");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
           printf("Ethernet connected\n");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            printf("Ethernet disconnected\n");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            printf("Obtained IP address\n");
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
    //WiFi.softAP(ap_ssid,"88888888");
   // printf(ssid);
   // printf(password);
    WiFi.begin(ssid,password);
   // WiFi.begin("Bach Huu Phat", "0908777832");
}

void wifi_staticip(char *ip_in, char* gateway_in, char* subnet_in){
  //if (atoi(WiFiConf.choose_dhcp) == 0){
    parseBytes1(ip_in, '.', 1, 4, 10);
    parseBytes1(gateway_in, '.', 2, 4, 10);
    parseBytes1(subnet_in, '.', 3, 4, 10);
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        printf("STA Failed to configure");
    }
  //}
}
