#include "Modbus485.h"

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) {
	//Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, transactionId);
	return true;
}
void modbus485HMI::senDataToDevice( uint16_t address, uint16_t* kytu, uint8_t numByte) {
	if (!modbus485HMI::slave()){
		if (xSemaphoreTake(xMutexRS485, 1)){
			writeHreg(idHMI, address, kytu, numByte, cb); //ID man hinh
			while (modbus485HMI::slave()) { modbus485HMI::task(); }
			xSemaphoreGive(xMutexRS485);
		}
	}	
}
uint8_t modbus485HMI::send485Utf16(uint16_t address, char* kytu, uint8_t numByte) {
	wmemset(nameNvUtf16, 0x0000, numByte);
	size_t valuelen = utf8ztowchar(kytu, nameNvUtf16, numByte);
	if (valuelen > 100) valuelen = 100;
	senDataToDevice(address, (uint16_t*)nameNvUtf16, numByte);
	//senDataToDevice(address + 64, (uint16_t*)nameNvUtf16[64], 32);
	return (uint8_t)valuelen;
}
/*
void send485HMI(uint16_t address, uint16_t* kytu, uint8_t numByte) {
	if (!mb.slave()) {
		if (xSemaphoreTake(xMutexRS485, 1)) {
			mb.writeHreg(1, address, kytu, numByte, cb); //ID man hinh
			while (mb.slave()) { mb.task(); }
			xSemaphoreGive(xMutexRS485);
		}
	}
}
void send485Utf16(uint16_t address, char* kytu, uint8_t numByte) {
	wmemset(modbusData.nameNvUtf16, 0x0000, numByte);
	size_t valuelen = utf8towchar(kytu, SIZE_MAX, modbusData.nameNvUtf16, sizeof(modbusData.nameNvUtf16));
	send485HMI(address, (uint16_t* )modbusData.nameNvUtf16, numByte);
}*/
void modbus485HMI::send485char(uint16_t address, char* kytu, uint8_t numByte) {
	if (numByte > 64) numByte = 64;
	wmemset(nameNvUtf16, 0x0000, numByte);
	for (int j = 0; j < numByte * 2; j = j + 2) {
		nameNvUtf16[j / 2] = (((uint16_t)kytu[j + 1] << 8) | ((uint16_t)kytu[j]));
	}
	senDataToDevice(address, (uint16_t*)nameNvUtf16, numByte);
}