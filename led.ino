extern "C"{
  #include "defs.h"
}


void SetupLed(void)
{
  pinMode(LED_MAIN_PWR,OUTPUT);
  pinMode(LED_PWR_RDY,OUTPUT);
  digitalWrite(LED_MAIN_PWR, LOW);
  digitalWrite(LED_PWR_RDY, LOW);
}

void SetMainPwrOn(void)
{
  digitalWrite(LED_MAIN_PWR, HIGH);
}

void SetPwrRdyOn(void)
{
  digitalWrite(LED_PWR_RDY, HIGH);
}

void SetPwrRdyOff(void)
{
  digitalWrite(LED_PWR_RDY, LOW);
}

void BlinkRdyLed(Button_t gBConf)
{
  
  if (gBConf == B_MAX_VALUE) // no blink set to off
  {
    SetPwrRdyOn();
    return;
  }

  if (gBConf == B_FILTER_CHANGE)
  {
   if (IsTimeExpired(LED_FILTER_MODE_BLINK_TIMEOUT))
   {
     digitalWrite(LED_PWR_RDY, !digitalRead(LED_PWR_RDY));
   }
  } else
  {
    if (IsTimeExpired(LED_INPUT_MODE_BLINK_TIMEOUT))
   {
     digitalWrite(LED_PWR_RDY, !digitalRead(LED_PWR_RDY));
   }
  }
  
}
