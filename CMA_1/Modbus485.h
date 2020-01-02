#include <ModbusRTU.h>
#include "cutf.h"

//extern SemaphoreHandle_t xMutexRS485;
class modbus485HMI :public ModbusRTU {
public:
	wchar_t nameNvUtf16[100];
	void senDataToDevice( uint16_t address, uint16_t* kytu, uint8_t numByte);
	uint8_t send485Utf16(uint16_t address, char* kytu, uint8_t numByte);
	void send485char(uint16_t address, char* kytu, uint8_t numByte);
private:
	uint8_t idHMI = 1;
	SemaphoreHandle_t xMutexRS485 = xSemaphoreCreateMutex();
};