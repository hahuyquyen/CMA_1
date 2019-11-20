/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
USIng HSPI cho Thẻ Nhớ
15-cs
14-sck
12-miso
13-mosi



 *    
 *    
 *    Đây là mã chưa được kiểm tra để thêm vào tệp SD.h ở khoảng dòng 50 trong lớp Tệp.
Mã: [Chọn]

  boolean truncate(uint32_t length) {return _file && _file->truncate(length);}
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "RTClib.h"

RTC_DS3231 rtc;
SPIClass SDSPI(HSPI);
void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        createDir(SD,dirname);
        root.close();
        return;
    }
    if(!root.isDirectory()){
      root.close();
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
    root.close();
}
void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}
int16_t stt=0;
File root_CMA ;
void setup(){
    Serial.begin(115200);
    pinMode(23,INPUT_PULLUP);
    SDSPI.begin(14,27,13,15); ///SCK,MISO,MOSI,ss
    if(!SD.begin(15,SDSPI)){
        Serial.println("Card Mount Failed");
        ESP.restart();
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
              if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  
  }
  
    if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    //nam,thang,ngay,gio,phut,giay
     rtc.adjust(DateTime(2019, 11, 21,11, 20, 0));
  }
    listDir(SD, "/CMA", 0);
    root_CMA = SD.open("/CMA");
}

long _time_counting_send_heap = 0;
long _time_end_check = 0;
void loop(){
  if (Serial.available()){ 
       char incomingData = Serial.read();
       if (incomingData == '1'){
                DateTime now = rtc.now();
                  Serial.print(" since midnight 1/1/1970 = ");
                  Serial.println(now.unixtime());
       char textToWrite[ 16 ];
       sprintf(textToWrite,"/CMA/%lu", now.unixtime());
       writeFile(SD, textToWrite, "123456");
       stt++;
       Serial.print("Crea :");
       Serial.println(textToWrite);
       }
       else if (incomingData == '2')
       {
        DateTime now = rtc.now();
                  Serial.print(" since midnight 1/1/1970 = ");
                  Serial.println(now.unixtime());
        char textToWrite[ 16 ];
        if (stt > 0)sprintf(textToWrite,"/CMA/%lu", stt - 1);
        else sprintf(textToWrite,"/CMA/%lu", stt);
        deleteFile(SD,textToWrite);
       }
        else if (incomingData == '3'){
            File file = root_CMA.openNextFile();
            if(file){
                Serial.print("  FILE: ");
                Serial.print(file.name());
                readFile(SD,file.name());
            }
            else if (_time_end_check==0){
              _time_end_check=xTaskGetTickCount();
              root_CMA.close();
            }
            else if(xTaskGetTickCount() - _time_end_check > 60000){
              root_CMA = SD.open("/CMA");
            }
       }
       else if (incomingData == '4'){

       }
  }     
  if (xTaskGetTickCount()- _time_counting_send_heap > 5000){
                 
                  _time_counting_send_heap = xTaskGetTickCount();
                   printf("Free Heap %d\n",ESP.getFreeHeap());
          
      }
 delay(500);
}
