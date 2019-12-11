#define debug_UART true
#define WIFI_CONF_FORMAT {0, 0, 0, 1}
#define WIFI_CONF_START 0

#define using_sta true

#define web_port 4999
#define time_2_lan_nhan_data 5000
#define time_cho_nhan_RFID_NV 8000


#define can_dau_cham 0x2C
#define can_dau_phay 0x2E
#define can_LF 0x0A
#define can_CR 0x0D
#define can_Minus 0x2D
#define canStartByte 0x3D
#define canStopByte 0x0D
#define canNULLByte 0x20

//#define uart_rfid_baud_rate 9600
//#define time_out_display_data 5000

#define MaRo_RFID 0xE2
#define MaNV_RFID 0x00
#define MaLoaiCa_RFID 0x01

#define button_left_pin 36
#define button_right_pin 34
#define button_ok_pin 35
#define buttonExitPin 39
#define buttonErrorPin 0
#define pinPower 32
#define pinReadPower 33

#define pinBuzzer 4
#define pinLedRed 5
#define pinLedGreen 2
#define pinAnalogPower 25

#define cheDoIN 1
#define cheDoOut 2
#define cheDoNotChoose 0

#define kvFille 1
#define kvSuaCa 2
#define kvNotChoose 0

#define stateChooseINOUT 0
#define stateChooseKV 1
#define stateChooseNhaCC 2
#define stateChooseThanhPham 3

#define deviceSetting 0
#define deviceRunning 1
#define deviceError 2
#define deviceLowPower 3

#define sttKvFillerIN 1
#define sttKvFillerOUT 2
#define sttKvSuaCaIN 3
#define sttKvSuaCaOUT 4
