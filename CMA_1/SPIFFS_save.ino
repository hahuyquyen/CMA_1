/*void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}*/
void deleteFile(fs::FS &fs, const char * path){
    //Serial.printf("Deleting file: %s\n", path);
    fs.remove(path);
}
void readFile(fs::FS &fs, const char * path,uint32_t len){
    Serial.printf("Reading : %s , L %lu \n", path,len);
    File file = fs.open(path);
    if(!file){return;}
    char* msg1 = (char*)malloc(len + 1);
    uint32_t num_=0;
    while(file.available()){
        msg1[num_ ++]=file.read();
    }
     msg1[num_]='\0';
    file.close();
    if (status_mqtt_connect){ uint16_t packetIdPub1 = mqttClient.publish("/data", 0, true, msg1);}
    free(msg1);
}

void writeFile(fs::FS &fs, const char * path, const char * message){
   // Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
      //  Serial.println("Failed to open file for writing");
        return;
    }
    file.print(message);
  /*  if(){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }*/
    file.close();
}

