void check_file_exit(){
  if (SPIFFS.exists("/save_mqtt.txt")){
    printf("file mqtt trong spiffs da co\n");
  }
}

void readFile(fs::FS &fs, const char * path, char* dataout){
    printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        printf("Failed to open file for reading\n");
        return;
    }

    printf("Read from file: ");
    String giatritam = "";
    giatritam = file.readStringUntil('\n');
    dataout=giatritam.c_str();
    printf("%s \n",giatritam.c_str());
    /*while(file.available()){
        printf("%c",file.read());
    }
    printf("\n");*/
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        printf("Failed to open file for writing\n");
        return;
    }
    if(file.print(message)){
        printf("File written\n");
    } else {
        printf("Write failed\n");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        printf("Failed to open file for appending\n");
        return;
    }
    if(file.print(message)){
        printf("Message appended\n");
    } else {
        printf("Append failed\n");
    }
    file.close();
}
void createDir(fs::FS &fs, const char * path){
    printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        printf("Dir created\n");
    } else {
        printf("mkdir failed\n");
    }
}
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        printf("- failed to open directory\n");
        return;
    }
    if(!root.isDirectory()){
        printf(" - not a directory\n");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            printf("  DIR : %s \n",file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            printf("  FILE: %s \n",file.name());
        }
        file = root.openNextFile();
    }
}
