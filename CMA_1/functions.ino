//////////////////////////////////////////////////////////////////
////// Tra ve STT Khu Vuc 1,2,3,4 thay vi 1-1 1-2 2-1 2-2 ////////////////////////////
//////////////////////////////////////////////////////////////////
uint8_t GetSttKhuVuc(){
  uint8_t bien=0;
  switch (inforServer.giaiDoan.arrayType[inforServer.giaiDoan.userSelect]){
    case kvSuaCa:
      if (inforServer.giaiDoan.cheDoInOut == cheDoIN) bien = sttKvSuaCaIN;
      else bien = sttKvSuaCaOUT;
      break;
    case kvFille:
      if (inforServer.giaiDoan.cheDoInOut == cheDoIN) bien = sttKvFillerIN;
      else bien = sttKvFillerOUT;
      break;
    default: break;
  }
  return bien;
}
/////////////////////////////////////////////////////////////////////////
////// Save Time to RTC ////////////////////////////
//////////////////////////////////////////////////////////////////
void SetTimeRtc(uint32_t timestampSave){
#ifdef debug_UART
    Serial.println("Write RTC Time : ");
    Serial.println(timestampSave);
#endif
#ifdef debug_Web
    DebugData("Write TimeStamp %lu", (unsigned long)timestampSave);
#endif
  uint32_t tam = (timestampSave > timeStamp.unixtime()) ? (timestampSave - timeStamp.unixtime()):(timeStamp.unixtime() - timestampSave );
  if (tam > 1800){
  rtc.adjust(DateTime(timestampSave));

#ifdef debug_Web
    DebugData("Set TimeStamp %lu", (unsigned long)tam);
#else
  Serial.print("Set TimeStamp: ");
  Serial.println(tam);
#endif
  }
  else {

#ifdef debug_Web
    DebugData("Khong Set TimeStamp %lu", (unsigned long)tam);
#else
      Serial.print("Khong set TimeStamp: ");
      Serial.println(tam);
#endif
  }
}
