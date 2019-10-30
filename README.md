# Mô tả
-ESP32 đọc dữ liệu từ cân, RFID 
-ESP32 chạy web setting.
-ESP32 hiển thị (..)
-ESP32 giao tiếp MQTT

Chương trình xử lý dùng SDK của esp32 trong Arduino   
```
TASK_Can: Chờ data từ uart của cân điện tử, xử lý tách dữ liệu và đưa vào struct
TASK_RFID: Config RFID, Chờ nhận data RFID từ UART2 và xử lýw
MainTask: Xử lý HTML, MQTT
TASK_display: Xử lý hiển thị vd: led
2UART ( UART1, UART2 ) xử lý ngắt nhận dữ liệu trong file UART_Inter
UART0 dùng debug chương trình
WEB_server : setting wifi, mqtt, firmware update...
Todo:

MQTT 
GET data RFID
```
![alt text](https://github.com/annhan/CMA_1/blob/master/imgae/main.png)
![alt text](https://github.com/annhan/CMA_1/blob/master/imgae/mqtt.png)
![alt text](https://github.com/annhan/CMA_1/blob/master/imgae/wifi.png)