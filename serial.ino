
extern "C"{
  #include "defs.h"
}

void InitSerial(void)
{
  Serial.begin(SERIAL_SPEED);
  while(!Serial);
}

bool WaitSerialChar(int timeout)
{
  int ms = millis();
  int me = 0; 
  
  if (timeout == INFINIT)
  {
    while(!Serial.available());
    return true;
  } 
  while(!Serial.available())
  {
    me = millis();
    if ((me - ms) > timeout) return false;
  }
  return true;
}

char GetSerial(void)
{
 return Serial.read(); 
}

void PrintSerial(char * ch)
{
  Serial.flush();
  Serial.print(ch);
}

void PrintSerialLine(char * ch)
{
  Serial.flush();
  Serial.println(ch);
}
