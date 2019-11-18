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
SPIClass SDSPI(HSPI);
#define SD_CS 5

uint32_t FindLinesAndPositions(char* filename)
{
  File myFile;
  uint32_t i = 0;
  myFile = SD.open(filename);
  if (myFile)
  { while (myFile.available()){
      if (myFile.read() == '\n') i++;
    }
    myFile.close();
  }
  return i;
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
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
}

uint32_t bien=0;

void setup(){
    Serial.begin(115200);
    pinMode(23,INPUT_PULLUP);
    SDSPI.begin();
    if(!SD.begin(15,SDSPI)){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    listDir(SD, "/", 0);
}
void readFile(fs::FS &fs, const char * path, uint32_t* bientang){
  uint32_t tam = 0;
  File file = fs.open(path);
      if(!file){
        printf("Failed to open file for reading\n");
        return;
    }
    String giatritam = "";
  while (file.available()){
    giatritam="";
    giatritam = file.readStringUntil('\n');
    if (tam >= *bientang) {*bientang=*bientang+1 ; break;}
    tam=tam + 1;
  }
    printf("%s \n",giatritam.c_str());
    Serial.println(millis());
    file.close();
}
void CopyFiles(char* ToFile, char* FromFile)
{
  File myFileOrig;
  File myFileTemp;

  if (SD.exists(ToFile))
    SD.remove(ToFile);
  myFileTemp = SD.open(ToFile, FILE_WRITE);
  myFileOrig = SD.open(FromFile);
  if (myFileOrig)
  {
    while (myFileOrig.available())
    {
      myFileTemp.write( myFileOrig.read() ); // make a complete copy of the original file
    }
    myFileOrig.close();
    myFileTemp.close();
  }
}
void DeleteMultipleLinesFromFile(char * filename, uint32_t SLine, uint32_t ELine)
{
  File myFileOrig;
  File myFileTemp;
  if (SD.exists("/tempFile.txt"))
    SD.remove("/tempFile.txt");
  myFileTemp = SD.open("/tempFile.txt", FILE_WRITE);
  myFileOrig = SD.open(filename);
  uint32_t position = 0;
  String giatritam;
  boolean first_run = true;
  if (myFileOrig)
  {
     
    while (myFileOrig.available())
    { if (first_run){
      giatritam = myFileOrig.readStringUntil('\n');
      first_run = false;
      }
      else {
      char C = myFileOrig.read();
      myFileTemp.write(C);
      }
    }
    myFileOrig.close();
    myFileTemp.close();
  }
  Serial.println(giatritam);
  CopyFiles(filename, "/tempFile.txt");
  if (SD.exists("/tempFile.txt"))
    SD.remove("/tempFile.txt");
}
void appendFile(const char * path, const char * message){
   // printf("Appending to file: %s\n", path);

    File file = SD.open(path, FILE_APPEND);
    if(!file){
        printf("Failed to open file for appending\n");
        return;
    }
    if(file.print(message)){
       // printf("Message appended\n");
    } else {
      //  printf("Append failed\n");
    }
    file.close();
}
void loop(){
 DeleteMultipleLinesFromFile("/Nhan.txt",0,0);
 uint32_t numline = FindLinesAndPositions("/Nhan.txt");
 Serial.println(numline);
 if (numline == 0){
  for (int i =0 ;i<20; i ++){
    size_t size_needed = snprintf(NULL, 0, "dong %u \n",i); 
    char* msg1 = (char*)malloc(size_needed);
    if (msg1 != NULL) { sprintf(msg1, "dong %u \n",i) ;appendFile("/Nhan.txt", msg1);}
  }
 }
 delay(3000);

}
