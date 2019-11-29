#include <EasyButton.h>
EasyButton button_left(button_left_pin,200,true);
EasyButton button_right(button_right_pin,200,true);
EasyButton button_ok(button_ok_pin,200,true);
EasyButton buttonExit(buttonExitPin,200,true);
EasyButton buttonError(buttonErrorPin,200,true);
EasyButton buttonPower(pinReadPower,200,true);
void onPressedPower() {
 if (xTaskGetTickCount() > 1000){digitalWrite(pinPower, LOW);}
}
void onPressed_left() {
  if ((Status_setting.state_select == 0)&&(inforServer.PhanLoaiKV > 0)){ inforServer.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((inforServer.PhanLoaiKV - 1) % (PhanLoai::LANG_OUT+1));}
  //else  if ((Status_setting.state_select == 1)&&(inforServer.userSelectLoaiCa > 0)){ inforServer.userSelectLoaiCa = inforServer.userSelectLoaiCa - 1 ;}
  else if ((Status_setting.state_select == 2)&&(inforServer.userSelectNhaCC > 0)){
         for (int tangTam = 0 ; tangTam < inforServer.tongNhaCC ; tangTam ++){
                  inforServer.userSelectNhaCC = inforServer.userSelectNhaCC-1  ;
                  if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){    
                                    if (inforServer.userSelectNhaCC == 0) break;   // quua 1 vong
                                    if (inforServer.sttGdSoLo[inforServer.userSelectNhaCC] == 1) {break;} // dung du lieu cua khu fille
                  }
                  else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){    
                          if (inforServer.userSelectNhaCC == 0) break;
                          if (inforServer.sttGdSoLo[inforServer.userSelectNhaCC] == 2) {break;}
                  }
          } 
    }
  else if ((Status_setting.state_select == 3)&&(inforServer.userSelectThanhPham > 0)){
            for (int tangTam = 0 ; tangTam < inforServer.tongThanhPham ; tangTam ++){
                           inforServer.userSelectThanhPham = inforServer.userSelectThanhPham - 1; 
                          if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){    
                              if (inforServer.userSelectThanhPham == 0) break;
                              if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 1) {break;}
                          }
                          else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){    
                              if (inforServer.userSelectThanhPham == 0) break;
                              if (inforServer.sttGdThanhPham[inforServer.userSelectThanhPham] == 2) {break;}
                          }
          }
    
    }
  variLcdUpdate.numScroll = 0;
  variLcdUpdate.updateLCD = true;
}
void onPressed_right() {
  if (Status_setting.state_select == 0){inforServer.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((inforServer.PhanLoaiKV + 1) % (PhanLoai::LANG_OUT+1));}
 // else if ((Status_setting.state_select == 1)&&(inforServer.tongLoaiCa > 0)){inforServer.userSelectLoaiCa = (inforServer.userSelectLoaiCa > inforServer.tongLoaiCa )? 0 : (inforServer.userSelectLoaiCa + 1); }
  else if ((Status_setting.state_select == 2)&&(inforServer.tongNhaCC > 0)){
     for (int tangTam = 0 ; tangTam < inforServer.tongNhaCC ; tangTam ++){
                  if ((inforServer.PhanLoaiKV == PhanLoai::Fil_IN)||(inforServer.PhanLoaiKV == PhanLoai::Fil_OUT)){    
                                    inforServer.userSelectNhaCC = (inforServer.userSelectNhaCC > inforServer.tongNhaCC) ? 0 : (inforServer.userSelectNhaCC + 1);
                                    if (inforServer.userSelectNhaCC == 0) break;   // quua 1 vong
                                    if (inforServer.sttGdSoLo[inforServer.userSelectNhaCC] == 1) {break;} // dung du lieu cua khu fille
                  }
                  else if ((inforServer.PhanLoaiKV == PhanLoai::LANG_IN)||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT)){    
                          inforServer.userSelectNhaCC = (inforServer.userSelectNhaCC > inforServer.tongNhaCC) ? 0 : (inforServer.userSelectNhaCC + 1);
                          if (inforServer.userSelectNhaCC == 0) break;
                          if (inforServer.sttGdSoLo[inforServer.userSelectNhaCC] == 2) {break;}
                  }
    }
  }
  else if ((Status_setting.state_select == 3)&&(inforServer.tongThanhPham > 0)){
        for (int tangTam = 0 ; tangTam < inforServer.tongThanhPham ; tangTam ++){
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
          }
  }
  variLcdUpdate.numScroll = 0;
  variLcdUpdate.updateLCD = true;
}
void onPressed_ok() {
  if ((Status_setting.state_select == 0)&& (inforServer.PhanLoaiKV == 0)) return;
 // if ((Status_setting.state_select == 1)&& (inforServer.userSelectLoaiCa == 0)) return;
  else if ((Status_setting.state_select == 2)&& (inforServer.userSelectNhaCC == 0)) return;
  else if ((Status_setting.state_select == 3)&& (inforServer.userSelectThanhPham == 0)) return;
  if  ((Status_setting.state_select == 0)&& ((inforServer.PhanLoaiKV == PhanLoai::Fil_OUT )||(inforServer.PhanLoaiKV == PhanLoai::LANG_OUT ))) {Status_setting.state_select = 3;
  }
  else{Status_setting.state_select = (Status_setting.state_select == 2) ? 4:(Status_setting.state_select == 0 )? 2 : Status_setting.state_select+ 1;}
  //else{Status_setting.state_select = (Status_setting.state_select == 2) ? 4:Status_setting.state_select + 1;}
  if (Status_setting.state_select > 4){state_Running_conf::state_Running = state_Running_conf::Running;}
  variLcdUpdate.numScroll = 0;
  variLcdUpdate.updateLCD = true;
}
void onPressedExit() {
  Status_setting.state_select = 0 ;
  state_Running_conf::state_Running = state_Running_conf::Setting;
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
      if (state_Running_conf::state_Running == state_Running_conf::Setting){
        button_left.read();
        button_right.read();
        button_ok.read();
      }
      buttonExit.read();
      buttonError.read();
      buttonPower.read();
        vTaskDelay(25);   
           //   vTaskDelay(5000); 
    //  printf("Task BUTOON StackHigh %d, Free Heap = %d\n",uxTaskGetStackHighWaterMark(NULL),ESP.getFreeHeap());    
    }
    vTaskDelete(NULL) ;
}
