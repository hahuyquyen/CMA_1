/*
  SD.end()
  SD.begin()
  de khoi dong lai the SD Cars.

  Thu vien khi khoi dong lại bo nho cap phát khong bị xoa boi ham free() nen mât bộ nhớ


  Sữa
  if (card->base_path) {
		err = esp_vfs_fat_unregister_path(card->base_path);SdReOpenFolder
		free(card->base_path);
	}
	them dong free de xoa bo nho

  Sữaz thu viện thay delay(100) bằng

*/
/////////////////////////////////////////////////
// Init SD card neu loi giua chung //////////////
////////////////////////////////////////////////
//extern statusPeripheral;
void SdReInit(File* masterFile) {
	if (statusPeripheral.mqtt.timeTruyenMQTT == 1000) {
		masterFile->close();
	}
	SD.end();
	delay(200);
	if (!SD.begin(15, SDSPI, 6000000)) {
		statusPeripheral.mqtt.timeTruyenMQTT = 3000;
#ifdef debug_Web
		DebugData("Reinit Card : Failed");
#endif
	}
	else {
		statusPeripheral.sdCard.statusConnect = true;
		statusPeripheral.mqtt.timeTruyenMQTT = 1000;
#ifdef debug_UART
		Serial.println("Reinit Card : OK");
#endif
#ifdef debug_Web
		DebugData("Reinit Card : OK");
#endif
	}

}
/////////////////////////////////////////////////
// Mo lại Folder                   //////////////
////////////////////////////////////////////////
void SdReOpenFolder() {
	statusPeripheral.sdCard.lastTimeReadEnd = xTaskGetTickCount();
	statusPeripheral.sdCard.statusGetAllFile = false;
	root_CMA = SD.open("/CMA");
	if (!root_CMA) {
		statusPeripheral.sdCard.statusConnect = false;
	}
}
/////////////////////////////////////////////////
// Open Next File                   //////////////
////////////////////////////////////////////////
void SdOpenNextFile(File* fileMaster) {
	File file = fileMaster->openNextFile();
	if (file) {
		if (!file.isDirectory()) {
			if (file.name()[5] == 'O') { //delete
			  //  Serial.print("delete file ");
			 //   Serial.println(file.name());
				SdDeleteFile(SD, file.name());
			}
			else {
				SdReadFile(SD, file.name(), file.size());
			}
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
void SdDeleteFile(fs::FS& fs, const char* path) {
	fs.remove(path);
}
//////////////////////////////////////////////////////////////////
////// Rename File ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/*void SdRenameFile(fs::FS &fs, const char * path1, const char * path2){
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

void SdRenameFile(fs::FS& fs, const char* path1, const char* path2) {
#ifdef debug_Web
	DebugData("Rename %s to %s", path1, path2);
#endif
	if (!fs.rename(path1, path2)) {
		statusPeripheral.sdCard.statusConnect = false;
#ifdef debug_Web
		DebugData("Rename failed");
#endif
	}
}
//////////////////////////////////////////////////////////////////
////// Read SD and send MQTT //////////////////////////////////////
//////////////////////////////////////////////////////////////////
void SdReadFile(fs::FS& fs, const char* path, uint32_t len) {
	File file = fs.open(path);
	if (!file) {
		statusPeripheral.sdCard.statusConnect = false;
		return;
	}
	char* msg1 = (char*)calloc(len + 1, sizeof(char));
	uint32_t num_ = 0;
	uint8_t numLineRepeat = 0;
	while (file.available()) {
		msg1[num_] = file.read();
		if (msg1[num_] == 'r') { numLineRepeat = num_; }
		num_ = num_ + 1;
	}
	msg1[num_] = '\0';
	file.close();
	uint8_t numCounterACK = (uint8_t)msg1[numLineRepeat + 3];
	if (numCounterACK < 52) {  //ascii to number : 48~0 -> 52~4
		numCounterACK = numCounterACK + 1;
		msg1[numLineRepeat + 3] = numCounterACK;
		SdWriteFile(SD, path, msg1);
		SendDataMqtt(inforServer.mqttConfig.topicSenData, msg1);
	}
	else {
		SdDeleteFile(SD, path);
	}
	free(msg1);
}
//////////////////////////////////////////////////////////////////
////// Write SD and send MQTT ////////////////////////////////////
//////////////////////////////////////////////////////////////////
void SdWriteFile(fs::FS& fs, const char* path, const char* message) {
	File file = fs.open(path, FILE_WRITE);
	if (!file) {
		statusPeripheral.sdCard.statusConnect = false;
		return;
	}
	file.print(message);
	file.close();
}