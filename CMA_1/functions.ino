//////////////////////////////////////////////////////////////////
////// Tra ve STT Khu Vuc 1,2,3,4 thay vi 1-1 1-2 2-1 2-2 ////////////////////////////
//////////////////////////////////////////////////////////////////
uint8_t getSttKhuVuc(){
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
void set_RTC(uint32_t timestampSave){
#ifdef debug_UART
    Serial.println("Write RTC Time : ");
    Serial.println(timestampSave);
#endif
  uint32_t tam = (timestampSave > timeStamp.unixtime()) ? (timestampSave - timeStamp.unixtime()):(timeStamp.unixtime() - timestampSave );
  if (tam > 1800){
  rtc.adjust(DateTime(timestampSave));
   Serial.print("Set TimeStamp: ");
    Serial.println(tam);
  }
  else {
    Serial.print("Khong set TimeStamp: ");
    Serial.println(tam);
  }
}
