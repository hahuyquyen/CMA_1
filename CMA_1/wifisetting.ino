/*
  Trang ma loi wifi

  https://demo-dijiudu.readthedocs.io/en/latest/api-guides/wifi.html
  [AsyncTCP.cpp:953] _poll(): 0x3ffe16e0 != 0x3ffe2678
*/
///////////////////////////////////////////
// GEt RSSI Theo Name Wifi  //////////////
/////////////////////////////////////////
int32_t getRSSI(const char* target_ssid) {
	byte available_networks = WiFi.scanNetworks();
	for (int network = 0; network < available_networks; network++) {
		if (strcmp(WiFi.SSID(network).c_str(), target_ssid) == 0) {
			return WiFi.RSSI(network);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////
////// even wifi mat ket noi           ////////////////////////////
//////////////////////////////////////////////////////////////////
void wifiOnDisconnect(WiFiEventInfo_t info)
{
	uint16_t* time_blink = (uint16_t*)malloc(sizeof(uint16_t));
	*time_blink = 200;
	xQueueSend(Queue_Time_blink, time_blink, (TickType_t)1);
	free(time_blink);
	xTimerStop(mqttReconnectTimer, 0); // tat tu dong ket noi mqtt
	statusPeripheral.wifi.statusConnectAP = false;
	if (info.disconnected.reason == 6) {
#ifdef debug_UART
		Serial.println("NOT_AUTHED reconnect");
#endif
#ifdef debug_Web
		DebugError("Wifi: NOT_AUTHED reconnect");
#endif
		WiFi.reconnect();
	}
	else if (info.disconnected.reason == 8) {
#ifdef debug_UART
		Serial.println("assoc leave");
#endif
#ifdef debug_Web
		DebugError("Wifi: assoc leave");
#endif
		wifi_connect(0, WIFI_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd, WiFiConf.ap_ssid, false);
	}
	/*
	   reason 201:  no AP found
	*/
}
////////////////////////////////////////////////////////////////////
////// Event wifi nhan IP /////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void wifigotip()
{
	if (statusPeripheral.wifi.ApConnect) {
		WiFi.softAPdisconnect();
		wifi_connect(0, WIFI_STA, WiFiConf.sta_ssid, WiFiConf.sta_pwd, WiFiConf.ap_ssid, false);
		return;
	}
	statusPeripheral.wifi.statusConnectAP = true;
	statusPeripheral.wifi.counterWifiDisconnect = 0;
#ifdef debug_Web
	DebugData("Wifi %s", WiFi.localIP().toString().c_str());
#endif
	uint16_t* time_blink = (uint16_t*)malloc(sizeof(uint16_t));
	*time_blink = 2000;
	xQueueSend(Queue_Time_blink, time_blink, (TickType_t)1);
	free(time_blink);
	ConnectToMqtt();
}
////////////////////////////////////////////////////////////////////
////// event trang thai wifi //////////////////////////////////////
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
///////////////////////////////////////////////////////////////////
////// tach data ip //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////
////// Cai dat che do wifi ////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void wifi_connect(byte _mode, wifi_mode_t wifi_mode, char* ssid, char* password, char* ap_ssid , bool statusModbus) {
	if (modbusData.connectAP) { //Neu Nhan cai dat tu HMI thi khong duoc reset
		return;
	}
	if (statusModbus) {
		modbusData.connectAP = true;
	}
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
////////////////////////////////////////////////////////////////////
////// Cai dat static ip cho wifi  ////////////////////////////////
//////////////////////////////////////////////////////////////////
void wifi_staticip(char* ip_in, char* gateway_in, char* subnet_in) {
	//if (atoi(WiFiConf.choose_dhcp) == 0){
	parseBytes1(ip_in, '.', 1, 4, 10);
	parseBytes1(gateway_in, '.', 2, 4, 10);
	parseBytes1(subnet_in, '.', 3, 4, 10);
	// if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) { printf("STA Failed to configure");}
	//}
}
