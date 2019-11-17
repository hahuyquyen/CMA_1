#include <EasyButton.h>
#define button_mode_ca_pin 32
#define button_khuvuc_pin 34
#define button_error_pin 35
#define button_vitri_pin 36
#define button_du_phong_pin 39

EasyButton button_mode_ca(button_mode_ca_pin,1000,true);
EasyButton button_khuvuc(button_khuvuc_pin,1000,true);
EasyButton button_error(button_error_pin,1000,true);
EasyButton button_vitri(button_vitri_pin,1000,true);
EasyButton button_du_phong(button_du_phong_pin,1000,true);
void onPressed_mode_ca() {
  chonloaica.STT_user_choose ++ ;
  if (chonloaica.STT_user_choose > chonloaica.SL_LoaiCa) {chonloaica.STT_user_choose = 0;}
  Serial.println("onPressed_mode_ca");
}
void onPressed_khuvuc() {
  chonloaica.PhanLoaiKV = static_cast<PhanLoai::PhanLoai>((chonloaica.PhanLoaiKV + 1) % (PhanLoai::LANG_OUT+1));
  Serial.print("onPressed_khuvuc");
  Serial.println(chonloaica.PhanLoaiKV);
}
void onPressed_error() {
Serial.println("onPressed_error");

}
void onPressed_vitri() {
Serial.println("onPressed_vitri");
}
void onPressed() {
Serial.println("onPressed");
}
void Check_button( void * pvParameters ){    
    button_mode_ca.begin();
    button_khuvuc.begin();
    button_error.begin();
    button_vitri.begin();
    button_du_phong.begin();
    button_mode_ca.onPressed(onPressed_mode_ca);
    button_khuvuc.onPressed(onPressed_khuvuc);
    button_error.onPressed(onPressed_error);
    button_vitri.onPressed(onPressed_vitri);
    button_du_phong.onPressed(onPressed);
    for (;;){
        button_mode_ca.read();
        button_khuvuc.read();
        button_error.read();
        button_vitri.read();
        vTaskDelay(25);   
    }
    vTaskDelete(NULL) ;
}

