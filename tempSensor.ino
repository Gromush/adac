#include <microDS18B20.h>

MicroDS18B20 <TERMAL_PIN> ts;
float currentTemp = 0;

void ThermalInit(void)
{
  ts.setResolution(12);
}
void ThermalRequest(void)
{
  ts.requestTemp();
}

bool ThermalProceed(void)
{
  bool res = false;
  if (ts.readTemp())
  {
    float temp = DS_rawToFloat(ts.getRaw());
    if (currentTemp != temp)
    {
      currentTemp = temp;
      res = true;
    }
    ThermalRequest();
  }
  return res;
}

void GetTemperature(char * str)
{
    dtostrf(currentTemp, 4,2, str);
}
