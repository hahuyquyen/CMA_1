void deleteFile(fs::FS &fs, const char * path){fs.remove(path);}
void readFile(fs::FS &fs, const char * path,uint32_t len){
    Serial.printf("Reading : %s , L %lu \n", path,( unsigned long )len);
    File file = fs.open(path);
    if(!file){return;}
    char* msg1 = (char*)malloc(len + 1);
    uint32_t num_=0;
    while(file.available())msg1[num_ ++]=file.read();
    msg1[num_]='\0';
    file.close();
    if (status_mqtt_connect){mqttClient.publish("/data", 0, true, msg1);}
    free(msg1);
    
}
void writeFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_WRITE);
    if(!file){return;}
    file.print(message);
    file.close();
}
