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
  //
  //CheckMode();
  //
  ButtonAction(&saved);
  BlinkRdyLed(gBConf);
  GetDACDataSerial();
  ReturnToMainMode();
  IndicatorAnalogs();
 
}

void CheckMode(void)
{

  static unsigned int L[MODE_ARR_SIZE]={0}, R[MODE_ARR_SIZE]={0};
  static unsigned int cIndex = 0;
  unsigned int sumL=0, sumR=0;
  unsigned int waitL, waitR;
  unsigned int pinL, pinR;
  
  pinL = analogRead(LEFT_CHANNEL);
  pinR = analogRead(RIGHT_CHANNEL);

  L[cIndex] = pinL;
  R[cIndex] = pinR;
  cIndex++;
  if (cIndex == MODE_ARR_SIZE)
  {
    cIndex = 0;
  }
  for (int i=0; i<MODE_ARR_SIZE; i++)
  {
    sumL+=L[i];
    sumR+=R[i];
  }

  waitL = sumL/MODE_ARR_SIZE;
  waitR = sumR/MODE_ARR_SIZE;

  if ( (waitL > MODE_MUSIC_LEVEL) || (waitR > MODE_MUSIC_LEVEL) ) 
  {
    GetGConfig()->mode = MODE_MUSIC; 
  } else
  {
    if (GetGConfig()->mode == MODE_MUSIC)
    {
      PrintDisplay();
    }
    GetGConfig()->mode = MODE_NORMAL;   
  }
}

bool IsTimeExpired(unsigned int timeout)
{
  static unsigned int ts = 0, te = 0;
  bool result = false;
    
  if (ts ==0)
  {
    ts = millis();
  } else
  {
    te = millis();
    if ((te-ts) > timeout)
    {
      ts = 0;
      te = 0;
      result = true;
    }
  }
  return result;
}
