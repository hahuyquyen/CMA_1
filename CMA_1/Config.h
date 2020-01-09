//#define debug_UART true
#define debug_Web true
#define DebugData debugD
#define DebugError debugE
#define DebugInfor debugI
//#define DebugWarning debugW
#define using_sta true
#define SerialRFID Serial2
#define WIFI_CONF_FORMAT {0, 0, 0, 1}
#define WIFI_CONF_START 0

#define web_port	4999
#define time_2_lan_nhan_data	5000
#define time_cho_nhan_RFID_NV	8000

#define button_left_pin		39
#define button_right_pin	34
#define buttonExitPin		36
#define button_ok_pin		35
#define buttonErrorPin		0
#define pinPower			32
#define pinReadPower		25

#define pinBuzzer		4
#define pinLedRed		2
#define pinLedGreen		5

#define cheDoIN			1
#define cheDoOut		2
#define cheDoNotChoose	0

#define kvFille			1
#define kvSuaCa			2
#define kvNotChoose		0

#define sttKvFillerIN	1
#define sttKvFillerOUT	2
#define sttKvSuaCaIN	3
#define sttKvSuaCaOUT	4

#define stateChooseINOUT		0
#define stateChooseKV			1
#define stateChooseNhaCC		2
#define stateChooseThanhPham	3
#define stateChooseConfirm		4
#define stateChooseRunning		5

#define deviceSetting	0
#define deviceRunning	1
#define deviceError		2
#define deviceLowPower	3
#define deviceTurnOff	4

#define pageInfor		10
#define pageNhaCC		12
#define pageThanhPham	13
#define pageComfirm		14
#define pageRunning		15
#define pagePopup		16

#define mqttGetNhaCC		1
#define mqttGetKV			2
#define mqttGetThanhPham	3
#define mqttGetTimeStamp	4
#define mqttGetResetCa		5


#define statusDisplayPopup		0
#define statusDisplayRunning	1
#define statusDisplayInfor		2
#define statusDisplayNhaCC		4
#define statusDisplayThanhPham	5
#define statusDisplayXacNhan	6

#define rs485HMIAddresStatusApMode	7832
#define rs485HMIAddresKgPopup		7898
#define rs485HMIAddresPage			7900
#define rs485HMIAddressKg			7910
#define rs485HMIAddresIP			7920
#define rs485HMIAddresSSID			7924
#define rs485HMIAddresNameNhaCC		8000
#define rs485HMIAddresNameThanhPham 8032
#define rs485HMIAddresRFIDNV		8150
#define rs485HMIAddresRFIDRo		8156
#define rs485HMIAddresNameNhanVien	8162
