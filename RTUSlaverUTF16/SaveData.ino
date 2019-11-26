bool loadWiFiConf() {
 /* if (EEPROM.read(WIFI_CONF_START + 0) == wifi_conf_format[0] &&
      EEPROM.read(WIFI_CONF_START + 1) == wifi_conf_format[1] &&
      EEPROM.read(WIFI_CONF_START + 2) == wifi_conf_format[2] &&
      EEPROM.read(WIFI_CONF_START + 3) == wifi_conf_format[3])
  {*/
    for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
      *((char*)&WiFiConf + t) = EEPROM.read(WIFI_CONF_START + t); //& lÃƒÂ  Ã„â€˜Ã¡Â»â€¹a chÃ¡Â»â€°  cÃ¡Â»Â§a biÃ¡ÂºÂ¿n Struc, *lÃƒÂ  data tÃ¡Â»Â©c lÃƒÂ  gÃƒÂ¡n data trong ÃƒÂ´ nhÃ¡Â»â€º struc bÃ¡ÂºÂ±ng eprom Ã„â€˜Ã¡Â»ï¿½c dc (char*) lÃƒÂ  ÃƒÂ©p kiÃ¡Â»Æ’u dÃ¡Â»Â¯ liÃ¡Â»â€¡u
    }
    return true;
 /* } 
  return false;*/
}

void saveWiFiConf(void) {
  Serial.println("Save data");
  for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
    EEPROM.write(WIFI_CONF_START + t, *((char*)&WiFiConf + t));
  }
  EEPROM.commit();
}
