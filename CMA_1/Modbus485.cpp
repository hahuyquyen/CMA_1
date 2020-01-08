#include "Modbus485.h"

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) {
	//Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, transactionId);
	return true;
}

uint16_t modbus485HMI::readDataHMI(uint16_t address, uint8_t numByte) {
	uint16_t tv = 0;
	if (!slave()) {
		readHreg(idHMI, address, &tv, numByte, cb);
		while (slave()) { task(); }
	}
	return tv;
}
void modbus485HMI::senDataToDevice(uint16_t address, uint16_t* kytu, uint8_t numByte) {
	if (!slave()) {
		if (xSemaphoreTake(xMutexRS485, 1)) {
			writeHreg(idHMI, address, kytu, numByte, cb); //ID man hinh
			while (slave()) { task(); }
			xSemaphoreGive(xMutexRS485);
		}
	}
}
uint8_t modbus485HMI::send485Utf16(uint16_t address, char* kytu, uint8_t numByte) {
	wmemset(nameNvUtf16, 0x0000, numByte);
	size_t valuelen = utf8ztowchar(kytu, nameNvUtf16, numByte);
	if (valuelen > 100) valuelen = 100;
	senDataToDevice(address, (uint16_t*)nameNvUtf16, numByte);
	return (uint8_t)valuelen;
}
void modbus485HMI::send485char(uint16_t address, char* kytu, uint8_t numByte) {
	if (numByte > 64) numByte = 64;
	wmemset(nameNvUtf16, 0x0000, numByte);
	for (int j = 0; j < numByte * 2; j = j + 2) {
		nameNvUtf16[j / 2] = (((uint16_t)kytu[j + 1] << 8) | ((uint16_t)kytu[j]));
	}
	senDataToDevice(address, (uint16_t*)nameNvUtf16, numByte);
}