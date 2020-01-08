


//////////////////////////////////////////////////////////////////
////// Save Data EPPROOM //////////////////////////////////////
//////////////////////////////////////////////////////////////////
/*
bool loadWiFiConf() {
	if (EEPROM.read(WIFI_CONF_START + 0) == wifi_conf_format[0] &&
		EEPROM.read(WIFI_CONF_START + 1) == wifi_conf_format[1] &&
		EEPROM.read(WIFI_CONF_START + 2) == wifi_conf_format[2] &&
		EEPROM.read(WIFI_CONF_START + 3) == wifi_conf_format[3])
	{
		for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
			*((char*)&WiFiConf + t) = EEPROM.read(WIFI_CONF_START + t); //& lÃ  Ä‘á»‹a chá»‰  cá»§a biáº¿n Struc, *lÃ  data tá»©c lÃ  gÃ¡n data trong Ã´ nhá»› struc báº±ng eprom Ä‘á»�c dc (char*) lÃ  Ã©p kiá»ƒu dá»¯ liá»‡u
		}
		return true;
	}
	return false;
}*/
//////////////////////////////////////////////////////////////////
////// Load Data eprom //////////////////////////////////////
//////////////////////////////////////////////////////////////////
/*bool saveWiFiConf(void) {
	for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
		EEPROM.write(WIFI_CONF_START + t, *((char*)&WiFiConf + t));
	}
	return EEPROM.commit();
}*/
/*
bool saveWiFiConf(void) {
	return setChar((char*)&WiFiConf, sizeof(WiFiConf));
}
*/
/*
  bool loaddata() {
	uint8_t bien=0;
	for (unsigned int t = 0; t < sizeof(inforServer); t++) {
	  bien ++ ;
	   ((uint8_t*)&inforServer + t) = EEPROM.read(700 + t); //& lÃ  Ä‘á»‹a chá»‰  cá»§a biáº¿n Struc, *lÃ  data tá»©c lÃ  gÃ¡n data trong Ã´ nhá»› struc báº±ng eprom Ä‘á»�c dc (char*) lÃ  Ã©p kiá»ƒu dá»¯ liá»‡u
	}
	return true;
  }
  void savedata(void) {
  for (unsigned int t = 0; t < sizeof(inforServer); t++) {
	EEPROM.write(700 + t, *((uint8_t*)&inforServer + t));
  }
  EEPROM.commit();
  }
  bool loaddata_SX() {
	for (unsigned int t = 0; t < sizeof(Nha_SX); t++) {
	   ((char*)&Nha_SX + t) = EEPROM.read(1000 + t); //& lÃ  Ä‘á»‹a chá»‰  cá»§a biáº¿n Struc, *lÃ  data tá»©c lÃ  gÃ¡n data trong Ã´ nhá»› struc báº±ng eprom Ä‘á»�c dc (char*) lÃ  Ã©p kiá»ƒu dá»¯ liá»‡u
	}
   return true;
  }
  void savedata_SX(void) {
  for (unsigned int t = 0; t < sizeof(Nha_SX); t++) {
	EEPROM.write(1000 + t, *((char*)&Nha_SX + t));
  }
  EEPROM.commit();
  }*/
