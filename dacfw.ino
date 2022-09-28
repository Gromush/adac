extern "C"{
  #include "defs.h"
}
#include <EEPROM.h>

SavedData_t saved = {0};

void setup() {
  SetupLed();
  SetMainPwrOn();
  InitLCD();
  ButtonInit();
  LCD_Print(0,0,"A-DAC starting...", false);
  LCD_Print(3,1, "Please wait", false);
  InitSerial();
  
  //DAC ON
  pinMode(DAC_PWR_ENABLE,OUTPUT);
  digitalWrite(DAC_PWR_ENABLE, HIGH);
  SetPwrRdyOn();
  WaitSerialChar(INFINIT);

  LCD_Clear();
  
  GetDACDataSerial();
  
  saved.all16 = EEPROM.read(SAVED_ADDR);
  if (saved.all16 == 0xFFFFFFFF)
  {
    saved.bytes.filter = GetGConfig()->FilterNum;
    EEPROM.write(SAVED_ADDR, saved.all16);
  } else
  {
    // Put saved data to struct and DAC
    SetFilter(&saved, true);
  }

}

void loop() {
 
  ButtonAction(&saved);
  GetDACDataSerial();
  
}
