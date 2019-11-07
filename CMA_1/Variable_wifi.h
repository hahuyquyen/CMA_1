/*
 * cma2018LHP515
 * 
 * 
 */

#define WIFI_CONF_FORMAT {0, 0, 0, 1}
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
#define WIFI_CONF_START 0

volatile uint8_t can_rxbuf[40];
volatile uint8_t canbuff=0;
uint8_t rfid_data[20];
static byte myEPC[12]; //Most EPCs are 12 bytes
static byte myEPClength;
unsigned long _time_lastconnect_mqtt=0;

static uint8_t counter_wifi_disconnect= 0;
static boolean status_wifi_connect_AP = true ; 




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
  char mqtt_subto2[64]; 
  char mqtt_subto3[64];
  char mqtt_choose_inout[5];
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
  "x",
  "x",
  "x",
  "IN"
};


SemaphoreHandle_t xCountingSemaphore;
SemaphoreHandle_t xSignal_FromRFID;
SemaphoreHandle_t xSignal_Display_check;
SemaphoreHandle_t xSignal_Display_checkdone;
QueueHandle_t Queue_can;
QueueHandle_t Queue_RFID;
QueueHandle_t Queue_RFID_NV;
QueueHandle_t Queue_mqtt;
QueueHandle_t Queue_display;
QueueHandle_t Queue_can_interrup;
QueueHandle_t Queue_Time_blink;
/**************************** 
 *  Struct Data 
 ***************************/
typedef struct Data_user{
  uint8_t id;
  char id_RFID[25];
  double data_weight;
  double data_tare;
} data_user;
typedef struct Data_TH{
  char id_RFID[25];
  char id_RFID_NV[25];
  double data_weight;
} Data_TH;

typedef struct Data_CAN{
  double data_can;
  unsigned long time_get;
} Data_CAN;
typedef struct Data_RFID{
  char id_RFID[25];
  char id_RFID_Old[25];
  unsigned long time_get;
} Data_RFID;
//char id_RFID_old[25];
typedef struct Display{
  uint8_t id;
  char name_nv[50];
  double cannang;
  double tiencong;
} display_NV;
 

/*
 * Web Server và MQTT
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
