/*
   cma2018LHP515


*/

const char ramChoDuLieu[] = "Chờ Dữ Liệu"; // dung can lây FPSTR(ramChoDuLieu) và strlcpy_P  tiet kiem RAM cho heap memory
const char ramChuaChon[] = "Chưa Chọn";
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
TickType_t xLastWakeTimeLoop;

struct statusPeripheralConf{
    struct mqttConf{
      unsigned long lastTimeGetDataConfig; 
      unsigned long  timeTruyenMQTT;
      boolean statusMqttConnect ;  
    }mqtt={0,1000,false};
    struct wifiConf{
      boolean ApConnect; 
      unsigned long lastTimeConnect;
      uint8_t counterWifiDisconnect;
      boolean statusConnectAP ;
    }wifi ={false,0,0,true};
    struct rtcConf{
      boolean statusConnect;
      unsigned long  lastTimeGetTimeStamp;
    }RTC={false,0};    
    struct sdCardConf{
      unsigned long lastTimeReadEnd;
      unsigned long lastTimeReInit;
      unsigned long lastTimeSendMQTT;
      boolean statusConnect;
      boolean statusGetAllFile;
    }sdCard={0,0,false,false};
    int rssiWifi;
    int powerValue;
    uint32_t timeStampServer;
}statusPeripheral;

struct timeServerConf{
    unsigned long sdReadEnd = 0;
    unsigned long sdSendMQTT = 0;
    unsigned long mqttGetDataConf = 0;
    void updateTime(unsigned long* timeUpdate){
     *timeUpdate =  xTaskGetTickCount();
    }
}lastTimeServer;


/////////////////////////////////////////////////////////////
///// Data status Button nhan           //////////////////////
////////////////////////////////////////////////////////////
static struct stateMachineConf {
  uint8_t bottonSelect;
  uint8_t deviceStatus;
  uint32_t idDevice;
  uint8_t powerRFID;
  void getPowerRFID(){
    this->powerRFID=EEPROM.readUInt(810);
  }
  void getIdControl(){
    this->idDevice=EEPROM.readUInt(800);
  }
  void setPowerRFID(){
    EEPROM.write(810, this->powerRFID);
    EEPROM.commit();
  }  
  void setIdControl(){
    EEPROM.writeUInt(800, this->idDevice);
    EEPROM.commit();
  }
} stateMachine;
/////////////////////////////////////////////////////////////
///// Data trang thai LCD           //////////////////////
////////////////////////////////////////////////////////////
struct variLcdUpdateConf {
  int numScroll;
  boolean updateLCD ;
  uint8_t stateDisplayLCD;
  boolean firstDisplay;
} variLcdUpdate = {
  0,
  false,
  0
};
/////////////////////////////////////////////////////////////
///// Data luu du lieu nhan tu server //////////////////////
////////////////////////////////////////////////////////////
static struct inforServerStruct {
  char nameCheDoInOut[3][30] = {"Chưa Chọn", "Đầu Vào", "Đầu Ra"};
  
  struct nhaCCConf{
    uint8_t total = 0;
    uint8_t userSelect;
    uint16_t arrayType[15];
    char arrayName[15][120];
  }nhaCC;
  
  struct thanhPhamConf{
    uint8_t total;
    uint8_t userSelect;
    uint16_t arrayType[15];
    char arrayName[15][120];
  }thanhPham;
  
  struct giaiDoanConf{
      uint8_t cheDoInOut;
      uint8_t total;
      uint8_t userSelect;
      uint8_t arrayType[10];
      char arrayName[10][30];
  }giaiDoan;
  
  struct mqttConfigConf{
      char topicGetStatusACK[25];
      char topicGetConfig[25];
      void setTopicACK(unsigned long id_device){
        sprintf(this->topicGetStatusACK, "/data/ack/%lu", id_device) ;
      }
     void setTopicGetConfig(unsigned long id_device){
        sprintf(this->topicGetConfig, "/config/%lu", id_device) ;
      }
  }mqttConfig;
  void changeData(boolean chedo, uint8_t* userSelect, uint8_t totaldata=0){
    if (chedo) { *userSelect = (*userSelect > (totaldata - 1)) ? 0 : (*userSelect + 1);}
    else *userSelect = *userSelect - 1;
  }
  
} inforServer;
/////////////////////////////////////////////////////////////
///// Data wifi, server mqtt           //////////////////////
////////////////////////////////////////////////////////////
static struct WiFiConfStruct {
  uint8_t format[4];
  char sta_ssid[32];
  char sta_pwd[64];
  char sta_ip[16];
  char sta_gateway[16];
  char sta_subnet[16];
  char ap_ssid[32];
  char choose_dhcp[4];
  char mqtt_server[64];
  char mqtt_port[6];
  char mqtt_user[64];
  char mqtt_pass[64];
  char mqtt_subto1[64];
} WiFiConf = {
  WIFI_CONF_FORMAT,
  "Bach Huu Phat",
  "0908777832",
  "192.168.1.240",
  "192.168.1.1",
  "255.255.255.0",
  "AP_SSID",
  "1",
  "mqtt_server",
  "0",
  "admin",
  "pass",
  "x"
};
/////////////////////////////////////////////////////////////
///// Data Queue  FreeRTOS          //////////////////////
////////////////////////////////////////////////////////////

SemaphoreHandle_t xCountingSemaphore;
SemaphoreHandle_t xSignal_FromRFID;
SemaphoreHandle_t xSignal_Display_check;
SemaphoreHandle_t xSignal_Display_checkdone;
SemaphoreHandle_t xreset_id_nv;
SemaphoreHandle_t xResetRfidMaRo;
QueueHandle_t Queue_can;
QueueHandle_t Queue_RFID;
QueueHandle_t Queue_RFID_NV;
QueueHandle_t Queue_mqtt;
QueueHandle_t Queue_display;
QueueHandle_t Queue_Time_blink;
/////////////////////////////////////////////////////////////
///// Data Tong Hop Task Checkdata           //////////////////////
////////////////////////////////////////////////////////////
typedef struct Data_TH {
  char id_RFID[25];
  char id_RFID_NV[25];
  double data_weight;
} Data_TH;
/////////////////////////////////////////////////////////////
///// Data Can DJ28SS           //////////////////////
////////////////////////////////////////////////////////////
typedef struct Data_CAN {
  double data_can;
} Data_CAN;
/////////////////////////////////////////////////////////////
///// Data RFID           //////////////////////
////////////////////////////////////////////////////////////
typedef struct Data_RFID {
  char id_RFID[25];
  char id_RFID_Old[25];
} Data_RFID;


/*
   Web Server vÃ  MQTT
*/

//############################
const char* update_path = "/firmware";
const char* update_username = "CMA";
const char* update_password = "123456";
//##########################################
static IPAddress local_IP(192, 168, 1, 241);
static IPAddress gateway(192, 168, 1, 1);
static IPAddress subnet(255, 255, 255, 0);
static IPAddress primaryDNS(8, 8, 8, 8); //optional
static IPAddress secondaryDNS(8, 8, 4, 4); //optional
