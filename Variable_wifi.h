#define WIFI_CONF_FORMAT {0, 0, 0, 1}
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
#define WIFI_CONF_START 0

typedef struct Data_user{
  uint16_t id_RFID;
  double data_weight;
  double data_tare;
  long time_detec;
} data_user;

static struct WiFiConfStruct {
  uint8_t format[4];
  char sta_ssid[32];
  char sta_pwd[64];
  char sta_ip[15];
  char sta_gateway[15];
  char sta_subnet[15];
  char ap_ssid[32];
  char choose_dhcp[4];
  char mqtt_server[64];
  char mqtt_port[6];
  char mqtt_user[64]; 
  char mqtt_pass[64]; 
  char mqtt_subto1[64]; 
  char mqtt_subto2[64]; 
  char mqtt_subto3[64]; 
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
  "x"
};

static IPAddress local_IP(192, 168, 1, 241);
static IPAddress gateway(192, 168, 1, 1);
static IPAddress subnet(255, 255, 255, 0);
static IPAddress primaryDNS(8, 8, 8, 8); //optional
static IPAddress secondaryDNS(8, 8, 4, 4); //optional
