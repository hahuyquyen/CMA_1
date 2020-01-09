#include "TongHopData.h"


void http_re(void* pvParameters) {
	const TickType_t xTicksToWait = pdMS_TO_TICKS(1);
	struct dataCan Data_CAN_TH;
	struct Data_RFID dataRfidRoTH;
	struct Data_RFID dataRfidNvTH;
	struct Data_TH dataTHSend;
	char idRFID_OLD[25];
	unsigned long lastTimeGetQueueCan = 0;
	unsigned long lastTimeGetQueueRFID_Ro = 0;
	unsigned long lastTimeGetQueueRFID_NV = 0;
	unsigned long lastTimeGetData_RoVaCan = 0;
	unsigned long timeCompareMode1 = 10000;
	unsigned long timeCompareMode2 = 10000;
	double canDataOutOld = 0;
	unsigned long lastTimeLED = 0;
	boolean statusLED = true;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for (;;) {
		boolean baoLed = false;

		if (xQueueReceive(Queue_can, &Data_CAN_TH, (TickType_t)1) == pdPASS) {
			lastTimeGetQueueCan = xTaskGetTickCount();
		}

		if (xQueueReceive(QueueRfidRo, &dataRfidRoTH, (TickType_t)1) == pdPASS) {
			if (strcmp(dataRfidRoTH.id_RFID, "000000000000000000000000") != 0) {
				baoLed = true;
				if (stateMachine.giaidoanKV == kvSuaCa) {
					lastTimeGetQueueRFID_Ro = xTaskGetTickCount();
				}
			}
		}

		//Nhận mã RFID
		//Nếu là khu Filler chỉ nhận mã rỗ thì swap time tới mã rỗ để khỏi viết lại code

		if (xQueueReceive(QueueRfidNV, &dataRfidNvTH, (TickType_t)1) == pdPASS) {
			if (strcmp(dataRfidNvTH.id_RFID, "000000000000000000000000") != 0) {
				baoLed = true;
				if (stateMachine.giaidoanKV == kvFille) { lastTimeGetQueueRFID_Ro = xTaskGetTickCount(); }
				else lastTimeGetQueueRFID_NV = xTaskGetTickCount();
			}
			else {
#ifdef debug_Web
				DebugData("Error RFID = 0");
#endif
			}
		}
		//////////////////////////////////////////
		// Bao LED tin hieu nhan RFID ////////////
		//////////////////////////////////////////
		if (baoLed) {
			statusLED = false;
			digitalWrite(pinLedGreen, statusLED);
			lastTimeLED = xTaskGetTickCount();
		}
		else if ((xTaskGetTickCount() - lastTimeLED > 200) && (statusLED == false)) {
			statusLED = true;
			digitalWrite(pinLedGreen, statusLED);
		}
		///////////////////////////////////////////////////////////////
		////// Device Running //////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		if (stateMachine.deviceStatus == deviceRunning) {
			////////////////////////////////////////////////
			//// Check giai doan 1: Nhan ma NV va Kg ///////
			///////////////////////////////////////////////      
			if ((lastTimeGetQueueCan > lastTimeGetQueueRFID_Ro + 300)
				&& (lastTimeGetQueueRFID_Ro > 0))
			{ // chỉ nhận khi dữ liệu cân lớn hơn dữ liệu rfid 500 stick
				timeCompareMode1 = lastTimeGetQueueCan - lastTimeGetQueueRFID_Ro;
				////////////////////////////////////////////////
				//// Thoi gian giua 2 lan nhan du lieu ///////
				///////////////////////////////////////////////                 
				if (timeCompareMode1 < time_2_lan_nhan_data) { // 2 dữ liệu phải nhỏ hơn thời gian cài đặt mới là 1 cặp đúng
					if (stateMachine.giaidoanKV == kvFille) {
						strncpy(dataTHSend.id_RFID_NV, dataRfidNvTH.id_RFID, sizeof(dataRfidNvTH.id_RFID));
						strncpy(dataTHSend.id_RFID, "x", sizeof("x"));
					}
					else {
						strncpy(dataTHSend.id_RFID, dataRfidRoTH.id_RFID, sizeof(dataRfidRoTH.id_RFID));
						strncpy(dataTHSend.id_RFID_NV, "x", sizeof("x"));
					}
					dataTHSend.data_weight = Data_CAN_TH.data_can;
					boolean tt = true;
					///
					// Kiem tra giai doan sua ca ngo ra, co can 2 lan
					// Neu khac ro thi van can binh thuong
					// neu cung ma ro trong 2 lần lien tiep phải khac so kg
					
						if (GetSttKhuVuc() == sttKvSuaCaOUT) {
							tt = false;
							if (strcmp(dataTHSend.id_RFID, idRFID_OLD) != 0) {
								strncpy(idRFID_OLD, dataTHSend.id_RFID, sizeof(dataTHSend.id_RFID));
								if (Data_CAN_TH.data_can > 0.5) tt = true;
								canDataOutOld = Data_CAN_TH.data_can;
							}
							else {
								double tam = Data_CAN_TH.data_can > canDataOutOld ? Data_CAN_TH.data_can - canDataOutOld : canDataOutOld - Data_CAN_TH.data_can;
								if ((tam > 0.3) && (Data_CAN_TH.data_can > 0.5)) { tt = true; }
								canDataOutOld = Data_CAN_TH.data_can;
							}
						}
					////////////////
					if (tt) {
						xQueueSend(Queue_display, &dataTHSend, xTicksToWait);
						if (GetSttKhuVuc() == sttKvSuaCaIN) {   //Neu Sua Ca Ngo Vao thi reset ma Nhan vien
							xSemaphoreGive(xreset_id_nv);
						}
						else { // nêu khong có check 2 lan thi gui mqtt 
#ifdef debug_Web
							DebugData("CHECK OUT: Time: %ld - Kg: %f - RFID: %s", timeCompareMode1, dataTHSend.data_weight, dataTHSend.id_RFID);
#endif
							xQueueSend(Queue_mqtt, &dataTHSend, xTicksToWait);
						}
					}
					timeCompareMode1 = 10000;
					lastTimeGetQueueCan = 0;
					lastTimeGetQueueRFID_Ro = 0;
					lastTimeGetData_RoVaCan = xTaskGetTickCount();
				}
			}

			else if ((lastTimeGetQueueRFID_Ro > 0) && (xTaskGetTickCount() - lastTimeGetQueueRFID_Ro > time_2_lan_nhan_data)) {
#ifdef debug_Web
				DebugData("Over time: Ma Ro va Can");
#endif
				lastTimeGetQueueRFID_Ro = 0;
				xSemaphoreGive(xreset_id_nv);
				// if ((inforServer.giaiDoan.arrayType[inforServer.giaiDoan.userSelect] != kvSuaCa)||(inforServer.giaiDoan.cheDoInOut == cheDoOut)){
				if (GetSttKhuVuc() != sttKvSuaCaIN) {
					xSemaphoreGive(xSignal_Display_checkdone);
				}
			}
			////////////////////////////////////////////////////////////////
			//// Check giai doan 2: Chi dung cho giai doan sua ca IN ///////
			///////////////////////////////////////////////////////////////    
			if (GetSttKhuVuc() == sttKvSuaCaIN) {
				if ((lastTimeGetQueueRFID_NV > lastTimeGetData_RoVaCan)
					&& (lastTimeGetData_RoVaCan > 0))
				{
					timeCompareMode2 = lastTimeGetQueueRFID_NV - lastTimeGetData_RoVaCan;
					////////////////////////////////////////////////
					//// Thoi gian giua 2 lan nhan du lieu ///////
					///////////////////////////////////////////////                    
					if (timeCompareMode2 < time_cho_nhan_RFID_NV) {
						lastTimeGetData_RoVaCan = lastTimeGetQueueRFID_NV;
						strncpy(dataTHSend.id_RFID_NV, dataRfidNvTH.id_RFID, sizeof(dataRfidNvTH.id_RFID));
#ifdef debug_Web
						DebugData("CHECK IN: Time: %ld - Kg: %f - RFID: %s - RFID NV: %s", timeCompareMode2, dataTHSend.data_weight, dataTHSend.id_RFID, dataTHSend.id_RFID_NV);
#endif
						xQueueSend(Queue_display, &dataTHSend, xTicksToWait);
						xQueueSend(Queue_mqtt, &dataTHSend, xTicksToWait);
						timeCompareMode2 = 10000;
						lastTimeGetQueueRFID_NV = 0;
						lastTimeGetData_RoVaCan = 0;
					}
					else {
						xSemaphoreGive(xSignal_Display_checkdone);
						lastTimeGetQueueRFID_NV = 0;
#ifdef debug_Web
						DebugData("TIme out check rfid nv");
#endif
					}
				}
				else if ((lastTimeGetData_RoVaCan > 0)
					&& (xTaskGetTickCount() - lastTimeGetData_RoVaCan > time_cho_nhan_RFID_NV))
				{
					xSemaphoreGive(xResetRfidMaRo); // Gửi tín hiệu để reset mã rỗ
					lastTimeGetData_RoVaCan = 0;
#ifdef debug_Web
					DebugData("Over time: Ma NV va ");
#endif
					xSemaphoreGive(xSignal_Display_checkdone);
				}
			}
		}
		vTaskDelayUntil(&xLastWakeTime, 30);
	}
	vTaskDelete(NULL);
}