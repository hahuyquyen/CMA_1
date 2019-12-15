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
/////////////////////////////////////////////////
// Init SD card neu loi giua chung //////////////
////////////////////////////////////////////////
//extern statusPeripheral;
void reInitSD(File* masterFile){
      if (statusPeripheral.mqtt.timeTruyenMQTT == 1000) {
        masterFile->close();
      }
      SD.end() ;
      delay(200);
      if (!SD.begin(15, SDSPI, 6000000)) {
        statusPeripheral.mqtt.timeTruyenMQTT = 3000;
#ifdef debug_UART
        Serial.println("Reinit Card : Failed");
#endif
      }
      else {
        statusPeripheral.sdCard.statusConnect = true;
        statusPeripheral.mqtt.timeTruyenMQTT = 1000;
#ifdef debug_UART
        Serial.println("Reinit Card : OK");
#endif
      }

}
/////////////////////////////////////////////////
// Mo lại Folder                   //////////////
////////////////////////////////////////////////
void reOpenFolder(){
      statusPeripheral.sdCard.lastTimeReadEnd = xTaskGetTickCount();
      statusPeripheral.sdCard.statusGetAllFile = false;
      root_CMA = SD.open("/CMA");
      if (!root_CMA) {
        statusPeripheral.sdCard.statusConnect = false;
      }
    #ifdef debug_UART
          Serial.println("Read Folder SD CATRĐ");
    #endif
}
/////////////////////////////////////////////////
// Open Next File                   //////////////
////////////////////////////////////////////////
void checkNextFile(File* fileMaster){
    File file = fileMaster->openNextFile();
    if (file) {
        if (!file.isDirectory()) {
            if (file.name()[5] == 'O') { //delete
              //  Serial.print("delete file ");
             //   Serial.println(file.name());
            //    deleteFile(SD, file.name());
            }
            else readFile(SD, file.name(), file.size());
        }
    }
    else {
        fileMaster->close();
        statusPeripheral.sdCard.statusGetAllFile = true;
        statusPeripheral.sdCard.lastTimeReadEnd = xTaskGetTickCount();
    }
}
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
  Serial.printf("Rename %s to %s\n", path1, path2);
#endif
  if (!fs.rename(path1, path2)) {
    statusPeripheral.sdCard.statusConnect = false;
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
  char* msg1 = (char*)calloc(len + 1,1);
  //inforServer.mqttConfig.dataSend
  uint32_t num_ = 0;
  while (file.available())msg1[num_ ++] = file.read();
  //while (file.available())inforServer.mqttConfig.dataSend[num_++] = file.read();
  msg1[num_] = '\0';
  file.close();
#ifdef debug_UART
    Serial.print("Send MQTT SDCARD : ");
    Serial.println(msg1);
#endif
  sendMQTT(msg1);
  //if (statusPeripheral.mqtt.statusMqttConnect) { mqttClient.publish("/data", 0, true, inforServer.mqttConfig.dataSend);}
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
