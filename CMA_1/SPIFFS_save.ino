/*
  SD.end()
  SD.begin()
  de khoi dong lai the SD Cars.

  Thu vien khi khoi dong lại bo nho cap phát khong bị xoa boi ham free() nen mât bộ nhớ


  Sữa
  if (card->base_path) {
        err = esp_vfs_fat_unregister_path(card->base_path);
        free(card->base_path);
    }
    them dong free de xoa bo nho

  Sữaz thu viện thay delay(100) bằng

*/

//////////////////////////////////////////////////////////////////
////// Delete file SD ///////////////////////////////////////
//////////////////////////////////////////////////////////////////
void deleteFile(fs::FS &fs, const char * path) {
  fs.remove(path);
}
//////////////////////////////////////////////////////////////////
////// Rename File ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/*void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
  }*/
//////////////////////////////////////////////////////////////////
////// rename //////////////////////////////////////
//////////////////////////////////////////////////////////////////

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
#ifdef debug_UART
  Serial.printf("Renaming file %s to %s\n", path1, path2);
#endif
  if (fs.rename(path1, path2)) {
#ifdef debug_UART
    Serial.println("File renamed");
#endif
  } else {
#ifdef debug_UART
    Serial.println("Rename failed");
#endif
  }
}
//////////////////////////////////////////////////////////////////
////// Read SD and send MQTT //////////////////////////////////////
//////////////////////////////////////////////////////////////////
void readFile(fs::FS &fs, const char * path, uint32_t len) {
  //Serial.printf("Reading : %s , L %lu \n", path,( unsigned long )len);
  File file = fs.open(path);
  if (!file) {
    statusPeripheral.sdCard.statusConnect = false;
    return;
  }
  char* msg1 = (char*)malloc(len + 1);
  uint32_t num_ = 0;
  while (file.available())msg1[num_ ++] = file.read();
  msg1[num_] = '\0';
  file.close();
  if (statusPeripheral.mqtt.statusMqttConnect) {
    mqttClient.publish("/data", 0, true, msg1);
  }
  free(msg1);

}
//////////////////////////////////////////////////////////////////
////// Write SD and send MQTT ////////////////////////////////////
//////////////////////////////////////////////////////////////////
void writeFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    statusPeripheral.sdCard.statusConnect = false;
    return;
  }
  file.print(message);
  file.close();
}
