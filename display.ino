#include <LiquidCrystal_I2C.h>
extern "C"{
  #include "defs.h"
}
// definitions 


LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_SYMBOLS_NUM, LCD_LINES_NUM);

byte logo_up[] = {
  B00000,
  B00100,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B00000
 };

byte logo_down[] = {
  B00000,
  B11111,
  B11111,
  B01110,
  B01110,
  B00100,
  B00100,
  B00000
 };

 byte logo_c[] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
 };

 byte indN[] = {
  B00001,
  B00000,
  B00000,
  B11011,
  B11011,
  B00000,
  B00000,
  B00001
 };

  byte indMax[] = {
  B00001,
  B00000,
  B00000,
  B00101,
  B00101,
  B00000,
  B00000,
  B00001
 }; 
 


  byte indE[] = {
  B00001,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00001
 }; 
 
void LCD_CreateCustomLogo()
{
  lcd.createChar(LOGO_UP,logo_up);
  lcd.createChar(LOGO_DWN,logo_down);
  lcd.createChar(LOGO_C,logo_c);
  lcd.createChar(IND_CNAR_NORM,indN);
  lcd.createChar(IND_CNAR_E, indE);
  lcd.createChar(IND_CHAR_M, indMax);
  
  
}

void LCD_PritLogo(int x, int y)
{
  lcd.setCursor(x,y);
  lcd.write(LOGO_UP);
  lcd.setCursor(x+2,y);
  lcd.write(LOGO_UP);
  lcd.setCursor(x+1,y+1);
  lcd.write(LOGO_DWN);
  lcd.setCursor(x,y+1);
  lcd.write(LOGO_C);
  lcd.setCursor(x+2,y+1);
  lcd.write(LOGO_C);
  
}

void IndicatorAnalogs(void)
{
  unsigned int xl=1,xr=1, xlup=0, xrup=0;
  int i;
  
  static unsigned int maxL=0, maxR=0;
  static unsigned int L[MODE_ARR_SIZE]={0}, R[MODE_ARR_SIZE]={0};
  static unsigned int index=0;
  static unsigned int MR=0, ML=0;
  static unsigned int SummL=0, SummR=0;
  static unsigned int cL=0, cR = 0;

  if (GetGConfig()->mode == MODE_NORMAL)
  {
    return;
  }

  lcd.setCursor(0,0);
  lcd.print("L");
  lcd.setCursor(0,1);
  lcd.print("R");
  
 
  
  /* Start to get avarage value */
  SummL -= L[index];
  SummR -= R[index];
  L[index] = analogRead(LEFT_CHANNEL);
  R[index] = analogRead(RIGHT_CHANNEL);
  SummL += L[index];
  SummR += R[index];
  
  index++;
  if (index >= MODE_ARR_SIZE) 
  { 
    index = 0;
  } else {
    return;
  }
  
  /* End to get avarage value */
  
  xlup = ((SummL*2) / MODE_ARR_SIZE ) / IND_DELIMITER;
  xrup = ((SummR*2) / MODE_ARR_SIZE) / IND_DELIMITER;
  
  if (maxL < xlup)
    {
      maxL = xlup;
      ML = millis();
      cL=0;
    }
  else
  {
    unsigned int MCL = millis();
    unsigned int maxVL; 
    if (cL == 0)
    {maxVL = IND_MAX_TIMEOUT_F;}
    else
    {maxVL = IND_MAX_TIMEOUT;}
    if ((MCL - ML) > maxVL)
    {
      cL++;
      if (maxL > 0) {maxL--;  }
      ML = millis();  
    }
  }
  
  if (maxR < xrup)
  {
    maxR= xrup;
    MR = millis();
    cR = 0;
  }
  else
  {
    unsigned int maxVR; 
    if (cR == 0)
    {maxVR = IND_MAX_TIMEOUT_F;}
    else
    {maxVR = IND_MAX_TIMEOUT;}
    unsigned int MCR = millis();
    if ((MCR - MR) > maxVR)
    {
      cR++;
      if (maxR > 0) {maxR--;}
      MR = millis();
    }
  }
  
  while(xl<=IND_MAX_POINT)
  {
    lcd.setCursor(xl,0);
    if (maxL != xl)
    {
      if (xl<=xlup)
      {
          lcd.write(IND_CNAR_NORM);
      } else
      {
          lcd.write(IND_CNAR_E);
      } 
    } else
    {
      lcd.write(IND_CHAR_M);
    }
    
    lcd.setCursor(xr,1);
    if (maxR != xr)
    {
      if (xr<=xrup)
      { 
        lcd.write(IND_CNAR_NORM);   
      } else
      {
        lcd.write(IND_CNAR_E);
      }
    } else
    {
      lcd.write(IND_CHAR_M);
    }
    xl++;
    xr++;
    
  }

}

void InitLCD(void)
{
  lcd.init();
  LCD_CreateCustomLogo();
  lcd.begin(LCD_SYMBOLS_NUM, LCD_LINES_NUM);
  lcd.clear();         
  lcd.backlight();
}

void LCD_Print(int x, int y, char * txt, bool toClear)
{
  if (toClear)
  {
    lcd.clear();
  }
  lcd.setCursor(x,y); 
  lcd.print(txt);
}


void LCD_Clear(void)
{
  lcd.clear();
}
