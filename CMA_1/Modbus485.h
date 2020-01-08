
	#include <ModbusRTU.h>
	#include "cutf.h"
	bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data);
	class modbus485HMI :public ModbusRTU {
	public:
		wchar_t nameNvUtf16[100];
		uint16_t readDataHMI(uint16_t address, uint8_t numByte);
		void senDataToDevice( uint16_t address, uint16_t* kytu, uint8_t numByte);
		uint8_t send485Utf16(uint16_t address, char* kytu, uint8_t numByte);
		void send485char(uint16_t address, char* kytu, uint8_t numByte);
	private:
		uint8_t idHMI = 1;
		SemaphoreHandle_t xMutexRS485 = xSemaphoreCreateMutex();
	};
