/*
 http://192.168.100.86:4999/SetTimeRtc?timestamp=1576032486
 http://192.168.100.77:4999/setKhuVuc?i=2&kv=1


 Mna Lan Dung

 Using DM9051
DM9051 Ethernet module consumes one SPI interface plus an interrupt and reset GPIO. By default they're connected as follows:
GPIO DM9051
GPIO19  SPI_CLK
GPIO23  SPI_MOSI
GPIO25  SPI_MISO
GPIO22  SPI_CS
GPIO4 Interrupt
GPIO5 Reset
 */



 //////////////////////////////////////////////////////////////////
 ////// Truyen tham so Html ////////////////////////////
 //////////////////////////////////////////////////////////////////
String processor(const String& var) {
	char __dataFileName[sizeof(var)];
	var.toCharArray(__dataFileName, sizeof(__dataFileName));
	if (var == F("SSID")) {
		return String(WiFiConf.sta_ssid);
	}
	else if (var == F("PASS")) {
		return String(WiFiConf.sta_pwd);
	}
	else if (var == F("IP")) {
		return String(WiFiConf.sta_ip);
	}
	else if (var == F("GATEWAY")) {
		return String(WiFiConf.sta_gateway);
	}
	else if (var == F("SUB")) {
		return String(WiFiConf.sta_subnet);
	}
	else if (var == F("ServerMQTT")) {
		return String(WiFiConf.mqtt_server);
	}
	else if (var == F("PortMQTT")) {
		return String(WiFiConf.mqtt_port);
	}
	else if (var == F("USERMQTT")) {
		return String(WiFiConf.mqtt_user);
	}
	else if (var == F("PASSMQTT")) {
		return String(WiFiConf.mqtt_pass);
	}
	else if (var == F("SUBTopic1")) {
		return String(WiFiConf.mqtt_subto1);
	}
	else if (var == F("CHOOSEDHCP")) {
		return atoi(WiFiConf.choose_dhcp) == 1 ? String(1) : String(0);
	}

	return String();
}
/*
  void handleUpdate(AsyncWebServerRequest *request) {
  char* html = (char *)"<form method='POST' action='/doUpdate' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
  request->send(200, F("text/html"), html);
  }
*/
void handleDoUpdate(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final) {

	if (!index) {
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
		AsyncWebServerResponse* response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
		response->addHeader(F("Refresh"), "20");
		response->addHeader(F("Location"), "/");
		request->send(response);
		if (!Update.end(true)) {
			Update.printError(Serial);
		}
		else {
			ESP.restart();
		}
	}
}

//////////////////////////////////////////////////////////////////
////// Cai dat Html ////////////////////////////
//////////////////////////////////////////////////////////////////
void getVariHtml(AsyncWebServerRequest* request, const __FlashStringHelper* id, char* dataget, size_t chieudai) {
	if (request->hasParam(id, true)) {
		request->getParam(id, true)->value().toCharArray(dataget, chieudai);
	}
}
void setupWiFiConf(void) {
	server.on("/2.css", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(SPIFFS, "/2.css", F("text/css"));
		});
	server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(SPIFFS, F("/index.html"));
		});
	server.on("/wifi_conf", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(SPIFFS, F("/wifi_conf.html"), String(), false, processor);
		});
	server.on("/mqtt_conf", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(SPIFFS, F("/mqtt_conf.html"), String(), false, processor);
		});
	server.on("/setTime", HTTP_GET, [](AsyncWebServerRequest* request) {
		if (request->hasParam(F("t"))) {
			SetTimeRtc(strtoul(request->getParam(F("t"))->value().c_str(), NULL, 10));
		}
		request->send(200, PSTR(dataHtmlType), F("OK ...."));
		});
	server.on("/setKhuVuc", HTTP_GET, [](AsyncWebServerRequest* request) {
		if (request->hasParam(F("i"))) {
			stateMachine.giaidoanINOUT = request->getParam(F("i"))->value().toInt();
		}
		if (request->hasParam(F("kv"))) {
			stateMachine.giaidoanKV = request->getParam(F("kv"))->value().toInt();
		}
		stateMachine.setGiaiDoan();
		stateMachine.setKV();
		request->send(200, PSTR(dataHtmlType), F("OK ...."));
		});
	server.on("/setRf", HTTP_GET, [](AsyncWebServerRequest* request) {
		if (request->hasParam(F("p"))) {
			stateMachine.powerRFID = request->getParam(F("p"))->value().toInt();
			stateMachine.setPowerRFID();
			statusPeripheral.updateRFID = true;
			Serial.println(stateMachine.powerRFID, HEX);
		}
		request->send(200, PSTR(dataHtmlType), F("OK ...."));
		});
	server.on("/set_wifi_conf", HTTP_POST, [](AsyncWebServerRequest* request) {
		if (request->hasParam(F("html_ssid"), true)) {
			request->getParam(F("html_ssid"), true)->value().toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
		}
		// getVariHtml(AsyncWebServerRequest * request, char* id, char* dataget, size_t chieudai) {
	//	getVariHtml(request, F("html_wpa_password"), WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
		if (request->hasParam(F("html_wpa_password"), true)) {
			request->getParam(F("html_wpa_password"), true)->value().toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
		}
		getVariHtml(request, F("html_ip_wifi"), WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
	/*	if (request->hasParam(F("html_ip_wifi"), true)) {
			request->getParam(F("html_ip_wifi"), true)->value().toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
		}*/
		if (request->hasParam(F("html_gateway_wifi"), true)) {
			request->getParam(F("html_gateway_wifi"), true)->value().toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
		}
		if (request->hasParam(F("html_sub_wifi"), true)) {
			request->getParam(F("html_sub_wifi"), true)->value().toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));
		}
		if (request->hasParam(F("button"), true)) {
			request->getParam(F("button"), true)->value().toCharArray(WiFiConf.choose_dhcp, sizeof(WiFiConf.choose_dhcp));
		}
		//if (request->hasParam(F("chooseinout"), true)) {request->getParam(F("chooseinout"), true)->value().toCharArray(WiFiConf.mqtt_choose_inout, sizeof(WiFiConf.mqtt_choose_inout));}
		if (saveWiFiConf())request->send(200, F("text/plain"), F("OK BABY"));
		else request->send(200, PSTR(dataHtmlType), F("Fail Save EEPROM"));
		});
	server.on("/set_id", HTTP_POST, [](AsyncWebServerRequest* request) {
		if (request->hasParam("id", true)) {
#ifdef debug_UART
			Serial.println(request->getParam("id", true)->value());
#endif
			/*
			#ifdef debug_Web
				  DebugData("%s",request->getParam("id", true)->value());
			#endif*/
			stateMachine.hardwareId = strtoul(request->getParam("id", true)->value().c_str(), NULL, 10);
			stateMachine.setIdControl();
		}
		request->send(200, PSTR(dataHtmlType), F("OK ...."));
		});
	server.on("/set_mqtt_conf", HTTP_POST, [](AsyncWebServerRequest* request) {
		if (request->hasParam(PSTR(ServerMQTT), true)) {
			request->getParam(PSTR(ServerMQTT), true)->value().toCharArray(WiFiConf.mqtt_server, sizeof(WiFiConf.mqtt_server));
		}
		if (request->hasParam(PSTR(PortMQTT), true)) {
			request->getParam(PSTR(PortMQTT), true)->value().toCharArray(WiFiConf.mqtt_port, sizeof(WiFiConf.mqtt_port));
		}
		if (request->hasParam(PSTR(USERMQTT), true)) {
			request->getParam(PSTR(USERMQTT), true)->value().toCharArray(WiFiConf.mqtt_user, sizeof(WiFiConf.mqtt_user));
		}
		if (request->hasParam(PSTR(PASSMQTT), true)) {
			request->getParam(PSTR(PASSMQTT), true)->value().toCharArray(WiFiConf.mqtt_pass, sizeof(WiFiConf.mqtt_pass));
		}
		if (request->hasParam(PSTR(SUBTopic1), true)) {
			request->getParam(PSTR(SUBTopic1), true)->value().toCharArray(WiFiConf.mqtt_subto1, sizeof(WiFiConf.mqtt_subto1));
		}
		if (saveWiFiConf())request->send(200, PSTR(dataHtmlType), F("OK BABY"));
		else request->send(200, PSTR(dataHtmlType), F("Fail Save EEPROM"));
		});

	server.on("/firmware", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(SPIFFS, F("/update.html"));
		});
	// server.on("/firmware", HTTP_GET, [](AsyncWebServerRequest *request){request->send(SPIFFS, F("/index.html"));});
	server.on("/doUpdate", HTTP_POST, [](AsyncWebServerRequest* request) {},
		[](AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data,
			size_t len, bool final) {
				handleDoUpdate(request, filename, index, data, len, final);
		});
	server.on("/Reboot", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(200, PSTR(dataHtmlType), F("OK.. Rebooting"));
		vTaskDelay(1000);
		ESP.restart();
		});
	server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(SPIFFS, F("/favicon.ico"), F("image/png"));
		});
}
