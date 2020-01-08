
#include "JT2850.h"
void IRAM_ATTR array_to_string(byte* array, unsigned int len, char* buffer)
{
	for (unsigned int i = 0; i < len; i++)
	{
		byte nib1 = (array[i] >> 4) & 0x0F;
		byte nib2 = (array[i] >> 0) & 0x0F;
		buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
		buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
	}
	buffer[len * 2] = '\0';
}
//////////////////////////////////////////////////////////////////
////// Task doc du lieu tu RFID  ////////////////////////////
//////////////////////////////////////////////////////////////////
void TaskRFID(void* pvParameters) {
	static byte myEPC[12];
	static byte myEPClength;
	struct Data_RFID dataRfidRo;
	struct Data_RFID dataRfidNV;
	struct RFID JT2850;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
	SerialRFID.begin(9600);
	JT2850.begin(SerialRFID);
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	statusPeripheral.updateRFID = true; // cai dat khi chay , check neu khong can thi bo
	for (;;) {
		if (statusPeripheral.updateRFID) {
			statusPeripheral.updateRFID = false;
			JT2850.set_mode_timming(2, rfidTimeOut); // Set mode eprom 0x70, mode timming
			JT2850.set_timing_message(0x05, rfidTimeOut); //0x00 -> 0x64
			JT2850.set_power(stateMachine.powerRFID, rfidTimeOut); // 00 -> 95
			JT2850.set_out_mode(1, rfidTimeOut);
			JT2850.set_time_ner(0x05, rfidTimeOut); // tna so gui 1->255s
			JT2850.set_reset_reader(rfidTimeOut);
		}
		if (JT2850.check() == true) {
			myEPClength = sizeof(myEPC);
			if (JT2850.parseResponse(myEPC, myEPClength)) {

				if (myEPC[0] == MaRo_RFID) {
					array_to_string(&myEPC[0], 12, dataRfidRo.id_RFID); //0->12 5->7

					if (GetSttKhuVuc() == sttKvSuaCaOUT) {
						strncpy(dataRfidRo.id_RFID_Old, dataRfidRo.id_RFID, sizeof(dataRfidRo.id_RFID));
						xQueueSend(QueueRfidRo, &dataRfidRo, xTicksToWait);

					}
					else if (strcmp(dataRfidRo.id_RFID, dataRfidRo.id_RFID_Old) != 0) {
						strncpy(dataRfidRo.id_RFID_Old, dataRfidRo.id_RFID, sizeof(dataRfidRo.id_RFID));
						xQueueSend(QueueRfidRo, &dataRfidRo, xTicksToWait);
					}
				}
				else if (myEPC[0] == MaNV_RFID) {
					array_to_string(&myEPC[0], 12, dataRfidNV.id_RFID);
					if (strcmp(dataRfidNV.id_RFID, "000000000000000000000000") != 0) { //00 00 00 00 00 00 00 00 00 00 00 00
						if (strcmp(dataRfidNV.id_RFID, dataRfidNV.id_RFID_Old) != 0) {
							strncpy(dataRfidNV.id_RFID_Old, dataRfidNV.id_RFID, sizeof(dataRfidNV.id_RFID));
							xQueueSend(QueueRfidNV, &dataRfidNV, xTicksToWait);
						}
					}
				}

			}
		}

		if (xSemaphoreTake(xreset_id_nv, 1)) { strncpy(dataRfidNV.id_RFID_Old, "", sizeof("")); }
		if (xSemaphoreTake(xResetRfidMaRo, 1)) { strncpy(dataRfidRo.id_RFID_Old, "", sizeof("")); }
		vTaskDelayUntil(&xLastWakeTime, 20);
	}
	vTaskDelete(NULL);
}
