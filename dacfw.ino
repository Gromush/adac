extern "C"{
  #include "defs.h"
}
#include <EEPROM.h>

SavedData_t saved = {0};

void setup() {
  saved.bytes.filter = EEPROM.read(SAVED_ADDR);
  saved.bytes.input = EEPROM.read(SAVED_ADDR+1);
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
  GetDACDataSerial();
  if (saved.bytes.filter == 0xFF)
  {
    saved.bytes.filter = GetGConfig()->FilterNum;
    saved.bytes.filter = GetGConfig()->inputType;
    EEPROM.write(SAVED_ADDR, saved.bytes.filter);
    EEPROM.write(SAVED_ADDR+1, saved.bytes.input);
  } else
  {
    // Put saved data to struct and DAC
    SetFilter(&saved, true);
    SetInput(&saved, true);
  }
  LCD_Clear();
  GetDACDataSerial();
}

void loop() {
 
  ButtonAction(&saved);
  GetDACDataSerial();
  ReturnToMainMode();
  
}
