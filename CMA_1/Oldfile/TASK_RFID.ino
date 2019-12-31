/*
   RFID mÃ¬nh dÃ¹ng module JT-2850, giao tiáº¿p uart
*/
//Calculate the checksum of the message and added as the last byte before sending it over
//to RFID reader
/*
   ESP-> 0xA0-lenght
   Reader->ESP: 0xE4-lenght hoáº·c 0xE0-lenght
*/
/*
[11:34:31 ---] A0 06 60 00 00 64 00 96 A0 06 60 00 00 CF 01 2A
[11:34:31 154] A0 06 60 00 00 65 96 FF -> Set nguon trong
A0 06 60 00 00 87 00 73
[11:34:31 156] A0 06 60 00 00 90 00 6A A0 06 60 00 00 8F 00 6B
[11:34:31 157] A0 0D 62 00 07 00 92 FF 07 00 C0 DF FF 03 B1
[11:35:31 ---] A0 06 60 00 00 64 00 96 A0 06 60 00 00 CF 01 2A
[11:35:31 156] A0 06 60 00 00 65 1C 79 A0 06 60 00 00 87 00 73
[11:35:31 156] A0 06 60 00 00 90 00 6A A0 06 60 00 00 8F 00 6B
[11:35:32 156] A0 0D 62 00 07 00 92 FF 07 00 C0 DF FF 03 B1
[11:37:08 ---] A0 06 60 00 00 64 00 96 A0 06 60 00 00 CF 01 2A
[11:37:08 156] A0 06 60 00 00 65 28 6D A0 06 60 00 00 87 00 73
[11:37:08 156] A0 06 60 00 00 90 00 6A A0 06 60 00 00 8F 00 6B
[11:37:08 157] A0 0D 62 00 07 00 92 FF 07 00 C0 DF FF 03 B1
*/
/*
#include "rfid.h"
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
	Data_RFID dataRfidRo;
	Data_RFID dataRfidNV;
	RFID JT2850;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
	SerialRFID.begin(9600);
	JT2850.begin(SerialRFID);
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		if (statusPeripheral.updateRFID) {
			statusPeripheral.updateRFID = false;
#ifdef debug_Web
			DebugData("Update RFID = 0");
#endif
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
					else {
#ifdef debug_Web
						DebugData("Error RFID = 0");
#endif
					}
				}

			}
		}

		if (xSemaphoreTake(xreset_id_nv, 1)) {strncpy(dataRfidNV.id_RFID_Old, "", sizeof(""));}
		if (xSemaphoreTake(xResetRfidMaRo, 1)) {strncpy(dataRfidRo.id_RFID_Old, "", sizeof(""));}
		vTaskDelayUntil(&xLastWakeTime, 20);
	}
	vTaskDelete(NULL);
}
*/