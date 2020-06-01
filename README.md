# Mô tả
-ESP32 đọc dữ liệu từ cân, RFID    
-ESP32 chạy web setting.   
-ESP32 hiển thị (..)   
-ESP32 giao tiếp MQTT   
-xây dựng thư viện cho module RFID:   
+Set parameter và chờ thông báo trả lời ok của module , có timeout.   
+Kiểm tra check và nhận mã ID RFID theo 3 thông số trả về:

- The temperature label output format is as follows:
- The EPC G2 tag variable byte output format is as follows:
- The EPC G2 tag output format is as follows:

Dùng FreeRTOS
Chương trình xử lý dùng SDK của esp32 trong Arduino cho việc cài đặt ngắt UART.
  
```
TASK_Can: Chờ data từ uart của cân điện tử, xử lý tách dữ liệu và đưa vào struct
TASK_RFID: Config RFID, Chờ nhận data RFID từ UART2 và xử lýw
MainTask: Xử lý HTML, MQTT
TASK_display: Xử lý hiển thị vd: led
2UART ( UART1, UART2 ) xử lý ngắt nhận dữ liệu trong file UART_Inter
UART0 dùng debug chương trình
WEB_server : setting wifi, mqtt, firmware update...
MQTT publish Data jSON: Topic: RFID card
ví dụ:
E3006019D26D1CE9AABBCCDD là ID RFID
-94.27,610.63 là 2 số đo của cân
thì data
{"id":"E3006019D26D1CE9AABBCCDD","data":[-94.27,610.63]}
và topic
E3006019D26D1CE9AABBCCDD
Todo:
Thêm thư viện set parameter cho module RFID VD: POWER ....
MQTT : Get thông tin sau khi tính toán từ server hiển thị màn hình
GET data RFID
```
![alt text](https://github.com/annhan/CMA_1/blob/master/imgae/main.png)
![alt text](https://github.com/annhan/CMA_1/blob/master/imgae/mqtt.png)
![alt text](https://github.com/annhan/CMA_1/blob/master/imgae/wifi.png)
![alt text](https://github.com/annhan/CMA_1/blob/master/imgae/mqtt_web.png)

# Cài đặt thiết bị cân:
- Cài đặt device ID: Sử dụng tool " Advanced rest client" của google để cài đặt device ID
