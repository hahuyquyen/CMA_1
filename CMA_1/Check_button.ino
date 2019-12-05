#include <EasyButton.h>
EasyButton button_left(button_left_pin,80,true);
EasyButton button_right(button_right_pin,80,true);
EasyButton button_ok(button_ok_pin,80,true);
EasyButton buttonExit(buttonExitPin,80,true);
EasyButton buttonError(buttonErrorPin,80,true);
EasyButton buttonPower(pinReadPower,1000,true);
void onPressedPower() {
 if (xTaskGetTickCount() > 1000){digitalWrite(pinPower, LOW);}
}
void onPressed_left() {
  if ((stateMachine.bottonSelect == 0)&&(inforServer.giaiDoan.cheDoInOut > 0)){
    inforServer.giaiDoan.cheDoInOut = inforServer.giaiDoan.cheDoInOut- 1;
  }
  else if ((stateMachine.bottonSelect == 1)&&(inforServer.giaiDoan.userSelect > 0)){  //1 Chonj giai doan can
                  inforServer.giaiDoan.userSelect = inforServer.giaiDoan.userSelect-1  ;
    }
  else if ((stateMachine.bottonSelect == 2)&&(inforServer.nhaCC.userSelect > 0)){
    inforServer.nhaCC.userSelect = inforServer.nhaCC.userSelect-1  ;
        /* for (int tangTam = 0 ; tangTam < inforServer.nhaCC.total ; tangTam ++){
                  inforServer.nhaCC.userSelect = inforServer.nhaCC.userSelect-1  ;
                  if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){    
                                    if (inforServer.nhaCC.userSelect == 0) break;   // quua 1 vong
                                    if (inforServer.sttGdSoLo[inforServer.nhaCC.userSelect] == 1) {break;} // dung du lieu cua khu fille
                  }
                  else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){    
                          if (inforServer.nhaCC.userSelect == 0) break;
                          if (inforServer.sttGdSoLo[inforServer.nhaCC.userSelect] == 2) {break;}
                  }
          } */
    }
  else if ((stateMachine.bottonSelect == 3)&&(inforServer.thanhPham.userSelect > 0)){
    inforServer.thanhPham.userSelect = inforServer.thanhPham.userSelect - 1; 
          /*  for (int tangTam = 0 ; tangTam < inforServer.thanhPham.total ; tangTam ++){
                           inforServer.userSelectThanhPham = inforServer.userSelectThanhPham - 1; 
                          if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){    
                              if (inforServer.userSelectThanhPham == 0) break;
                              if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 1) {break;}
                          }
                          else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){    
                              if (inforServer.userSelectThanhPham == 0) break;
                              if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 2) {break;}
                          }
          }*/
    
    }
  variLcdUpdate.numScroll = 0;
  variLcdUpdate.updateLCD = true;
}
void onPressed_right() {
  //if (stateMachine.bottonSelect == 0){inforServer.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((inforServer.PhanLoaiKV + 1) % (PhanLoai::LANG_OUT+1));}
 // else if ((stateMachine.bottonSelect == 1)&&(inforServer.tongLoaiCa > 0)){inforServer.userSelectLoaiCa = (inforServer.userSelectLoaiCa > inforServer.tongLoaiCa )? 0 : (inforServer.userSelectLoaiCa + 1); }
  if (stateMachine.bottonSelect == 0){ inforServer.giaiDoan.cheDoInOut = (inforServer.giaiDoan.cheDoInOut > 1) ? 0 : (inforServer.giaiDoan.cheDoInOut + 1);}
  else if ((stateMachine.bottonSelect == 1)&&(inforServer.giaiDoan.total > 0)){ inforServer.giaiDoan.userSelect = (inforServer.giaiDoan.userSelect > (inforServer.giaiDoan.total-1)) ? 0 : (inforServer.giaiDoan.userSelect + 1);}
  else if ((stateMachine.bottonSelect == 2)&&(inforServer.nhaCC.total > 0)){inforServer.nhaCC.userSelect = (inforServer.nhaCC.userSelect > (inforServer.nhaCC.total - 1)) ? 0 : (inforServer.nhaCC.userSelect + 1);}
  else if ((stateMachine.bottonSelect == 3)&&(inforServer.thanhPham.total > 0)){inforServer.thanhPham.userSelect = (inforServer.thanhPham.userSelect > (inforServer.thanhPham.total-1)) ? 0 : (inforServer.thanhPham.userSelect + 1);
      /*  for (int tangTam = 0 ; tangTam < inforServer.thanhPham.total ; tangTam ++){
                          if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){    
                               inforServer.thanhPham.userSelect = (inforServer.userSelectThanhPham > inforServer.thanhPham.total) ? 0 : (inforServer.userSelectThanhPham + 1);
                              if (inforServer.userSelectThanhPham == 0) break;
                              if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 1) {break;}
                          }
                          else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){    
                              inforServer.userSelectThanhPham = (inforServer.userSelectThanhPham > inforServer.thanhPham.total) ? 0 : (inforServer.userSelectThanhPham + 1);
                              if (inforServer.userSelectThanhPham == 0) break;
                              if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 2) {break;}
                          }
          }*/
  }
  variLcdUpdate.numScroll = 0;
  variLcdUpdate.updateLCD = true;
}
void onPressed_ok() {
  if ((stateMachine.bottonSelect == 0)&& (inforServer.giaiDoan.cheDoInOut == 0)) return;
  else if ((stateMachine.bottonSelect == 1)&& (inforServer.giaiDoan.userSelect == 0)) return;
  else if ((stateMachine.bottonSelect == 2)&& (inforServer.nhaCC.userSelect == 0)) return;
  else if ((stateMachine.bottonSelect == 3)&& (inforServer.thanhPham.userSelect == 0)) return;
  if  ((stateMachine.bottonSelect == 1)&& (inforServer.giaiDoan.cheDoInOut == cheDoOut )) {stateMachine.bottonSelect = 3;}
  else{stateMachine.bottonSelect = (stateMachine.bottonSelect == 2) ? 4:(stateMachine.bottonSelect == 0 )? 1 : stateMachine.bottonSelect+ 1;}
  //else{stateMachine.bottonSelect = (stateMachine.bottonSelect == 2) ? 4:stateMachine.bottonSelect + 1;}
  if (stateMachine.bottonSelect > 4){stateMachine.deviceStatus = deviceRunning;}
  variLcdUpdate.numScroll = 0;
  variLcdUpdate.updateLCD = true;
}
void onPressedExit() {
  stateMachine.bottonSelect = 0 ;
 stateMachine.deviceStatus = deviceSetting;
  Serial.println("onPressed_vitri");
}
void onPressedError() {
Serial.println("onPressed");
}
void Check_button( void * pvParameters ){    
    button_left.begin();
    button_right.begin();
    buttonExit.begin();
    button_ok.begin();
    buttonError.begin();
    buttonPower.begin();
    button_left.onPressed(onPressed_left);
    button_right.onPressed(onPressed_right);
    buttonExit.onPressed(onPressedExit);
    button_ok.onPressed(onPressed_ok);
    buttonError.onPressed(onPressedError);
    buttonPower.onPressed(onPressedPower);
    for (;;){
      if (stateMachine.deviceStatus == deviceSetting){
        button_left.read();
        button_right.read();
        button_ok.read();
      }
      buttonExit.read();
      buttonError.read();
      buttonPower.read();
      vTaskDelay(50);     
    }
    vTaskDelete(NULL) ;
}
