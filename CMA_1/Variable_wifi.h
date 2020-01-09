/*
   cma2018LHP515

*/

const char dataHtmlType[] PROGMEM = "text/plain";

const char ramChoDuLieu[] = "Chờ Dữ Liệu";
const char ramChuaChon[] = "Chưa Chọn";
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
TickType_t xLastWakeTimeLoop;
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
static IPAddress primaryDNS(8, 8, 8, 8); 
static IPAddress secondaryDNS(8, 8, 4, 4);
