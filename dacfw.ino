extern "C"{
  #include "defs.h"
}
#include <EEPROM.h>

SavedData_t saved = {0};

extern Button_t gBConf;
char VersionString[16];

unsigned int debounceMode = 0;
bool checkModeFlag = false;
void setup() {
  
  sprintf(VersionString, "FW:  %s", VERSION_STRING);
  saved.bytes.filter = EEPROM.read(SAVED_ADDR_FILTER);
  saved.bytes.input = EEPROM.read(SAVED_ADDR_INPUT);
  SetupLed();
  SetMainPwrOn();
  InitLCD();
  ButtonInit();
  LCD_Print(0,0, "Starting...", true);
  LCD_Print(0,1, VersionString,false);
  LCD_PritLogo(12,0);
  analogReference(DEFAULT); 
  analogWrite(BRIGHTNESS,BRIGHTNESS_VAL);
  InitSerial();
  GetGConfig()->inputType = UNINIT_VAL;
  GetGConfig()->mode = MODE_NORMAL;
  //DAC ON
  pinMode(DAC_PWR_ENABLE,OUTPUT);
  digitalWrite(DAC_PWR_ENABLE, HIGH);
  SetPwrRdyOn();
  WaitSerialChar(INFINIT);
  GetDACDataSerial();
  
  if (saved.bytes.filter == 0xFF)
  {
    saved.bytes.filter = GetGConfig()->FilterNum;
    saved.bytes.input = GetGConfig()->inputType;
    EEPROM.write(SAVED_ADDR_FILTER, saved.bytes.filter);
    EEPROM.write(SAVED_ADDR_INPUT, saved.bytes.input);
  } else
  {
    // Put saved data to struct and DAC
    SetFilter(&saved, true);
    SetInput(&saved, true);
  }
  GetDACDataSerial();
  PrintDisplay();
  debounceMode = millis();
}

void loop() {

  ButtonAction(&saved); // 3 time "if" for exit if no action required
  BlinkRdyLed(gBConf);  // one "if" in normal mode to exit 
  GetDACDataSerial();   // one "if" serial available in case no action required
  ReturnToMainMode();   // one "if" for no action case
  IndicatorAnalogs();   // one "if" in case no mode active
 
}
