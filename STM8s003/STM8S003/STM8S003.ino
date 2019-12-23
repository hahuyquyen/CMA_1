/*
Sua file boards
################################################################################
# Nucleo 8S Board

STM8S003f3p6.name=STM8S003f3p6

STM8S003f3p6.build.core=arduino
STM8S003f3p6.build.board=STM8S003f3p6
STM8S003f3p6.build.extra_flags=-d{build.product_line} {build.xSerial}

# STM8S003f3p6 board
# Support: Serial1 (USART1 on PA4, PA5) and Serial2 (USART2 on PD6, PD5)
STM8S003f3p6=Nucleo 8S208RB
STM8S003f3p6.device=STM8S208RB
STM8S003f3p6.node=NODE_8S208
STM8S003f3p6.upload.maximum_size=131072
STM8S003f3p6.upload.maximum_data_size=6144
STM8S003f3p6.build.mcu=8-bit
STM8S003f3p6.build.board=STM8S003f3p6
STM8S003f3p6.build.series=STM8Sxx
STM8S003f3p6.build.splibraryPrefix=_StdPeriph_Driver
STM8S003f3p6.build.product_line=STM8S003   //khI BAO
STM8S003f3p6.build.variant=STM8S003f3p6
STM8S003f3p6.build.f_cpu=16000000

STM8S003f3p6.menu.upload_method.MassStorage=Mass Storage
STM8S003f3p6.menu.upload_method.MassStorage.upload.protocol=
STM8S003f3p6.menu.upload_method.MassStorage.upload.tool=massStorageCopy

STM8S003f3p6.menu.upload_method.stm8Flasher=stm8Flasher
STM8S003f3p6.menu.upload_method.stm8Flasher.upload.protocol=stm8Tools
STM8S003f3p6.menu.upload_method.stm8Flasher.upload.tool=stm8Flasher
# Serialx activation
STM8S003f3p6.menu.xserial.generic=Enabled with generic Serial
STM8S003f3p6.menu.xserial.none=Enabled without generic Serial
STM8S003f3p6.menu.xserial.none.build.xSerial=-dHWSERIAL_NONE
STM8S003f3p6.menu.xserial.disabled=Disabled (No Serial)
STM8S003f3p6.menu.xserial.disabled.build.xSerial=-dNO_HWSERIAL




 */

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
