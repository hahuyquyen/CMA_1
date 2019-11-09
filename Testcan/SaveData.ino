/**
 * Tải Eprom thông tin struct wifi
 */
bool loadWiFiConf() {
  //printf(F("loading WiFiConf"));
  if (EEPROM.read(WIFI_CONF_START + 0) == wifi_conf_format[0] &&
      EEPROM.read(WIFI_CONF_START + 1) == wifi_conf_format[1] &&
      EEPROM.read(WIFI_CONF_START + 2) == wifi_conf_format[2] &&
      EEPROM.read(WIFI_CONF_START + 3) == wifi_conf_format[3])
  {
    for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
      *((char*)&WiFiConf + t) = EEPROM.read(WIFI_CONF_START + t); //& là địa chỉ  của biến Struc, *là data tức là gán data trong ô nhớ struc bằng eprom đọc dc (char*) là ép kiểu dữ liệu
    }
    return true;
  } 
  return false;
}
/*
 * Lưu Eprom thông tin wifi
 */
void saveWiFiConf(void) {
  for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
    EEPROM.write(WIFI_CONF_START + t, *((char*)&WiFiConf + t));
  }
  EEPROM.commit();
}
