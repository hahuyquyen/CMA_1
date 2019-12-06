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
