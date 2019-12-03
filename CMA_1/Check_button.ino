#include <EasyButton.h>
EasyButton button_left(button_left_pin,80,true);
EasyButton button_right(button_right_pin,80,true);
EasyButton button_ok(button_ok_pin,80,true);
EasyButton buttonExit(buttonExitPin,80,true);
EasyButton buttonError(buttonErrorPin,80,true);
EasyButton buttonPower(pinReadPower,80,true);
void onPressedPower() {
 if (xTaskGetTickCount() > 1000){digitalWrite(pinPower, LOW);}
}
void onPressed_left() {
  //if ((stateMachine.bottonSelect == 0)&&(inforServer.PhanLoaiKV > 0)){ inforServer.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((inforServer.PhanLoaiKV - 1) % (PhanLoai::LANG_OUT+1));}
  //else  if ((stateMachine.bottonSelect == 1)&&(inforServer.userSelectLoaiCa > 0)){ inforServer.userSelectLoaiCa = inforServer.userSelectLoaiCa - 1 ;}
 // if ((stateMachine.bottonSelect == 0)&&(inforServer.PhanLoaiKV > 0)){
  if ((stateMachine.bottonSelect == 0)&&(giaiDoanCan.cheDoInOut > 0)){
    giaiDoanCan.cheDoInOut = giaiDoanCan.cheDoInOut- 1;
     
  }
  else if ((stateMachine.bottonSelect == 1)&&(giaiDoanCan.userSelecGiaiDoan > 0)){  //1 Chonj giai doan can
                  giaiDoanCan.userSelecGiaiDoan = giaiDoanCan.userSelecGiaiDoan-1  ;
    }
  else if ((stateMachine.bottonSelect == 2)&&(inforServer.userSelectNhaCC > 0)){
    inforServer.userSelectNhaCC = inforServer.userSelectNhaCC-1  ;
        /* for (int tangTam = 0 ; tangTam < inforServer.tongNhaCC ; tangTam ++){
                  inforServer.userSelectNhaCC = inforServer.userSelectNhaCC-1  ;
                  if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){    
                                    if (inforServer.userSelectNhaCC == 0) break;   // quua 1 vong
                                    if (inforServer.sttGdSoLo[inforServer.userSelectNhaCC] == 1) {break;} // dung du lieu cua khu fille
                  }
                  else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){    
                          if (inforServer.userSelectNhaCC == 0) break;
                          if (inforServer.sttGdSoLo[inforServer.userSelectNhaCC] == 2) {break;}
                  }
          } */
    }
  else if ((stateMachine.bottonSelect == 3)&&(inforServer.userSelectThanhPham > 0)){
    inforServer.userSelectThanhPham = inforServer.userSelectThanhPham - 1; 
          /*  for (int tangTam = 0 ; tangTam < inforServer.tongThanhPham ; tangTam ++){
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
  if (stateMachine.bottonSelect == 0){ giaiDoanCan.cheDoInOut = (giaiDoanCan.cheDoInOut > 1) ? 0 : (giaiDoanCan.cheDoInOut + 1);}
  else if ((stateMachine.bottonSelect == 1)&&(giaiDoanCan.tongGiaiDoan > 0)){ giaiDoanCan.userSelecGiaiDoan = (giaiDoanCan.userSelecGiaiDoan > (giaiDoanCan.tongGiaiDoan-1)) ? 0 : (giaiDoanCan.userSelecGiaiDoan + 1);}
  else if ((stateMachine.bottonSelect == 2)&&(inforServer.tongNhaCC > 0)){inforServer.userSelectNhaCC = (inforServer.userSelectNhaCC > (inforServer.tongNhaCC - 1)) ? 0 : (inforServer.userSelectNhaCC + 1);}
  else if ((stateMachine.bottonSelect == 3)&&(inforServer.tongThanhPham > 0)){inforServer.userSelectThanhPham = (inforServer.userSelectThanhPham > (inforServer.tongThanhPham-1)) ? 0 : (inforServer.userSelectThanhPham + 1);
      /*  for (int tangTam = 0 ; tangTam < inforServer.tongThanhPham ; tangTam ++){
                          if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){    
                               inforServer.userSelectThanhPham = (inforServer.userSelectThanhPham > inforServer.tongThanhPham) ? 0 : (inforServer.userSelectThanhPham + 1);
                              if (inforServer.userSelectThanhPham == 0) break;
                              if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 1) {break;}
                          }
                          else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){    
                              inforServer.userSelectThanhPham = (inforServer.userSelectThanhPham > inforServer.tongThanhPham) ? 0 : (inforServer.userSelectThanhPham + 1);
                              if (inforServer.userSelectThanhPham == 0) break;
                              if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 2) {break;}
                          }
          }*/
  }
  variLcdUpdate.numScroll = 0;
  variLcdUpdate.updateLCD = true;
}
void onPressed_ok() {
  if ((stateMachine.bottonSelect == 0)&& (giaiDoanCan.cheDoInOut == 0)) return;
  else if ((stateMachine.bottonSelect == 1)&& (giaiDoanCan.userSelecGiaiDoan == 0)) return;
  else if ((stateMachine.bottonSelect == 2)&& (inforServer.userSelectNhaCC == 0)) return;
  else if ((stateMachine.bottonSelect == 3)&& (inforServer.userSelectThanhPham == 0)) return;
  if  ((stateMachine.bottonSelect == 1)&& (giaiDoanCan.cheDoInOut == cheDoOut )) {stateMachine.bottonSelect = 3;}
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
      vTaskDelay(25);     
    }
    vTaskDelete(NULL) ;
}
