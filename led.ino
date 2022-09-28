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

void SetMainPwrOff(void)
{
  digitalWrite(LED_MAIN_PWR, LOW);
}

void SetPwrRdyOn(void)
{
  digitalWrite(LED_PWR_RDY, HIGH);
}

void SetPwrRdyOff(void)
{
  digitalWrite(LED_PWR_RDY, LOW);
}
