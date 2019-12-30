<<<<<<< Updated upstream
#define debug_UART true
=======
#ifndef CONFIG_H
#define CONFIG_H
#endif
//#define debug_UART true
#define debug_Web true
#define DebugData debugD
#define DebugError debugE
#define DebugInfor debugI
//#define DebugWarning debugW


>>>>>>> Stashed changes

#define SerialCan Serial1
#define SerialRFID Serial2
#define WIFI_CONF_FORMAT {0, 0, 0, 1}
#define WIFI_CONF_START 0

#define using_sta true

#define web_port 4999
#define time_2_lan_nhan_data 5000
#define time_cho_nhan_RFID_NV 8000

<<<<<<< Updated upstream

=======
/*
#define canNOD1 0x40
#define canNOD2 0x41
#define canNOD3 0x44
#define canNOD4 0x45
#define canNOD5 0x48
#define canOD1 0x42
#define canOD2 0x43
#define canOD3 0x47
#define canOD4 0x46
>>>>>>> Stashed changes
#define can_dau_cham 0x2C
#define can_dau_phay 0x2E
#define can_LF 0x0A
#define can_CR 0x0D
#define can_Minus 0x2D
#define canStartByte 0x3D
#define canStopByte 0x0D
#define canNULLByte 0x20
<<<<<<< Updated upstream

//#define uart_rfid_baud_rate 9600
//#define time_out_display_data 5000

=======
*/
>>>>>>> Stashed changes
#define MaRo_RFID 0xE2
#define MaNV_RFID 0x00
#define MaLoaiCa_RFID 0x01

#define button_left_pin 36
#define button_right_pin 34
#define button_ok_pin 35
#define buttonExitPin 39
#define buttonErrorPin 0
#define pinPower 32
#define pinReadPower 25

#define pinBuzzer 4
#define pinLedRed 2
#define pinLedGreen 5

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
#define deviceTurnOff 4

#define sttKvFillerIN 1
#define sttKvFillerOUT 2
#define sttKvSuaCaIN 3
#define sttKvSuaCaOUT 4
<<<<<<< Updated upstream
=======

#define mqttGetNhaCC 1
#define mqttGetKV 2
#define mqttGetThanhPham 3
#define mqttGetTimeStamp 4
#define mqttGetResetCa 5

/*
RS485
*/
#define pageInfor 10
#define pageNhaCC 12
#define pageThanhPham 13
#define pageComfirm 14
#define pageRunning 15
#define pagePopup 16
#define rs485AddressNameNhaCc 8000
#define rs485AddressNameThanhPham 8032
#define rs485AddressMaNV 8150
#define rs485AddressMaRo 8156
#define rs485AddressNameNhanVien 8162
#define rs485AddressSoKg 7910
#define rs485AddressSoKgStatic 7898
#define rs485AddressNameSSID 7924
#define rs485AddressIPAddress 7920
#define rs485AddressPage 7900
>>>>>>> Stashed changes
