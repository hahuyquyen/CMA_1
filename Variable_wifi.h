#define WIFI_CONF_FORMAT {0, 0, 0, 1}
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
#define WIFI_CONF_START 0

typedef struct Data_user{
  uint16_t id_RFID;
  float data_weight;
  float data_tare;
  long time_detec;
} data_user;

struct WiFiConfStruct {
  uint8_t format[4];
  char sta_ssid[32];
  char sta_pwd[64];
  char sta_ip[15];
  char sta_gateway[15];
  char sta_subnet[15];
  char ap_ssid[32];
  char mqtt_server[64];
  uint16_t mqtt_port;
  char mqtt_user[64]; 
  char mqtt_pass[64]; 
} WiFiConf = {
  WIFI_CONF_FORMAT,
  "Bach Huu Phat",
  "0908777832",
  "192.168.1.240",
  "192.168.1.1",
  "255.255.255.0",
  "AP_SSID",
  "mqtt_server",
  1652,
  "admin",
  "pass"
};

IPAddress local_IP(192, 168, 1, 241);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
