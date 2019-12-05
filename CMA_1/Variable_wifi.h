/*
   cma2018LHP515


*/

const char ramChoDuLieu[] = "Chờ Dữ Liệu"; // dung can lây FPSTR(ramChoDuLieu) và strlcpy_P  tiet kiem RAM cho heap memory
const char ramChuaChon[] = "Chưa Chọn";
const char htmlPortMQTT[] PROGMEM = "PortMQTT"; // dung can lây FPSTR(htmlPortMQTT) và strlcpy_P  tiet kiem RAM cho heap memory
//const char khuVucFilletRa[] PROGMEM ="FILLET-Đầu Ra"; // FPSTR(khuVucFilletRa)



#define WIFI_CONF_FORMAT {0, 0, 0, 1}
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
#define WIFI_CONF_START 0

//uint8_t rfid_data[20];
static byte myEPC[12]; 
static byte myEPClength;
boolean statusGetAllSD = false;
unsigned long SD_lastTimeReadEnd = 0;
unsigned long SD_lastTimeSendMQTT = 0;
unsigned long MQTT_lastTimeGetDataConfig = 0;
static uint8_t counter_wifi_disconnect = 0;
static boolean status_wifi_connect_AP = true ;
static boolean status_mqtt_connect = false ;
//uint8_t firstGetDataFromServer = 0;

struct timeServerConf{
    unsigned long sdReadEnd = 0;
    unsigned long sdSendMQTT = 0;
    unsigned long mqttGetDataConf = 0;
    void updateTime(unsigned long* timeUpdate){
     *timeUpdate =  xTaskGetTickCount();
    }
}lastTimeServer;



static struct stateMachineConf {
  uint8_t bottonSelect;
  uint8_t deviceStatus;
  uint32_t idDevice;
  
  void getIdControl(){
    this->idDevice=EEPROM.readUInt(800);
  }
  
  void setIdControl(){
    EEPROM.writeUInt(800, this->idDevice);
    EEPROM.commit();
  }
  
} stateMachine;

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
        sprintf(topicGetStatusACK, "/data/ack/%lu", id_device) ;
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
/****************************
    Struct Data
 ***************************/
typedef struct Data_TH {
  char id_RFID[25];
  char id_RFID_NV[25];
  double data_weight;
} Data_TH;

typedef struct Data_CAN {
  double data_can;
} Data_CAN;


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
