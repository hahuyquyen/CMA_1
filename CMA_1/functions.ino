boolean compareWithZero(uint8_t* data) {
	if (*data == 0) return true;
	return false;
}

////////////////////////////////////////////////////////////////////
////// Tra ve STT Khu Vuc 1,2,3,4 thay vi 1-1 1-2 2-1 2-2 /////////
//////////////////////////////////////////////////////////////////
                          
uint8_t GetSttKhuVuc() {
	uint8_t bien = 0;
	switch (stateMachine.giaidoanKV) {
	case kvSuaCa:
		if (stateMachine.giaidoanINOUT == cheDoIN) bien = sttKvSuaCaIN;
		else bien = sttKvSuaCaOUT;
		break;
	case kvFille:
		if (stateMachine.giaidoanINOUT == cheDoIN) bien = sttKvFillerIN;
		else bien = sttKvFillerOUT;
		break;
	default: break;
	}
	return bien;
}
//////////////////////////////////////////////////////////////////
////// Save Time to RTC                ////////////////////////////
//////////////////////////////////////////////////////////////////
void SetTimeRtc(uint32_t timestampSave) {
#ifdef debug_Web
	DebugData("Write TimeStamp %lu", (unsigned long)timestampSave);
#endif
	uint32_t tam = (timestampSave > timeStamp.unixtime()) ? (timestampSave - timeStamp.unixtime()) : (timeStamp.unixtime() - timestampSave);
	if (tam < 1800) return;
	else rtc.adjust(DateTime(timestampSave));
}
