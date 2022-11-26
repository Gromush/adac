#include <LiquidCrystal_I2C.h>
extern "C"{
  #include "defs.h"
}
// definitions 


LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_SYMBOLS_NUM, LCD_LINES_NUM);

// LOGO Upper triangle
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

// LOGO lower triangle
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
// LOGO eyes
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

// Indicator normal line part
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

// Indicator max part for slow down
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

// Indicator empty
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
// Indicator overflow (last 3 points) only when active
byte indO[] = {
  B11111,
  B11111,
  B00000,
  B11011,
  B11011,
  B00000,
  B11111,
  B11111
 }; 
 
void LCD_CreateCustomChars()
{
  lcd.createChar(LOGO_UP,logo_up);
  lcd.createChar(LOGO_DWN,logo_down);
  lcd.createChar(LOGO_C,logo_c);
  lcd.createChar(IND_CNAR_NORM,indN);
  lcd.createChar(IND_CNAR_E, indE);
  lcd.createChar(IND_CHAR_M, indMax);
  lcd.createChar(IND_CHAR_O, indO);
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

unsigned int maxL=0, maxR=0;
unsigned int index=0;
unsigned int MR=0, ML=0;
unsigned int SummL=0, SummR=0;
unsigned int cL=0, cR = 0;

void PrepareIndicators(void)
{
  lcd.setCursor(0,0);
  lcd.print("L");
  lcd.setCursor(0,1);
  lcd.print("R");
  maxL=0, maxR=0;
  index=0;
  MR=0, ML=0;
  SummL=0, SummR=0;
  cL=0, cR = 0;
}

void debugP(int x, int y, char * s, unsigned int value)
{
  char str[7];
  sprintf(str, s, value);
  LCD_Print(x,y,str,false);
}
bool CheckAuto(unsigned int left, unsigned int right)
{
  bool result;
  static unsigned int autoStart =0, autoEnd=0;
  static unsigned int zeroC=0; 
  //debugP(0,0, "L:   %u",left);
  //debugP(8,0, "R:   %u",right);
  if (GetGConfig()->mode != MODE_AUTO)
  {
    return true;
  }

  if (GetGConfig()->autoMode == MODE_NORMAL)
  {
    result = false;
    if ( (left > AUTO_MODE_IN) || (right > AUTO_MODE_IN) )
    {
      zeroC = 0;
      if (autoStart == 0)
      {
        autoStart = millis();
      }

      autoEnd = millis();
      
      // Check if music ON
      if ( (autoEnd - autoStart) > AUTO_MODE_TIME)
      {
        GetGConfig()->autoMode = MODE_MUSIC;
        result = true;
        autoStart = 0;
        PrepareIndicators();
      }
    } else
    {
      // In case small level found try to calc new tieout
      zeroC++;
      if (zeroC > AUTO_ZERO_MAX)
      {
        autoStart = 0;
      }
      //debugP(0,1, "Set: %u",zeroC++);
    }
    
  } else 
  {
    result = true;
    if ( (left < AUTO_MODE_OUT) && (right < AUTO_MODE_OUT) )
    {
      
      if (autoStart == 0)
      {
        autoStart = millis();
      }
      autoEnd = millis();
      // Check if music OFF
      if ( (autoEnd - autoStart) > AUTO_MODE_TIME)
      {
        GetGConfig()->autoMode = MODE_NORMAL;
        result = false;
        autoStart = 0;
        PrintDisplay(true);
      }
    } else
    {
      autoStart = 0;
    }
  }

  return result;
}

void IndicatorAnalogs(void)
{
  unsigned int xl=1,xr=1, xlup=0, xrup=0, anleft, anright;
  int i;
  
  if (GetGConfig()->mode == MODE_NORMAL)
  {
    return;
  }
  
  /* Start to get avarage value */
  SummL += (analogRead(LEFT_CHANNEL)*2);
  SummR += (analogRead(RIGHT_CHANNEL)*2);
  
  index++;
  if (index >= MODE_ARR_SIZE) 
  { 
    index = 0;
  } else {
    return;
  }

  /* End to get avarage value */
  anleft  = ((SummL) / MODE_ARR_SIZE );
  anright = ((SummR) / MODE_ARR_SIZE);
  
  
  if (CheckAuto(anleft, anright) == false)
  {
    SummL = 0;
    SummR = 0;
    return;
  }
  
  xlup = anleft / IND_DELIMITER;
  xrup = anright / IND_DELIMITER;
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
  
  while(xl<IND_MAX_POINT)
  {
    lcd.setCursor(xl,0);
    if (maxL != xl)
    {
      if (xl<=xlup)
      {   
          if (xl >= (IND_MAX_POINT-3))
          {
            lcd.write(IND_CHAR_O);
          }else
          {
            lcd.write(IND_CNAR_NORM);
          }
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
        if (xr >= (IND_MAX_POINT-3))
        {
          lcd.write(IND_CHAR_O); 
        } else
        {
          lcd.write(IND_CNAR_NORM);
        }
             
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
  SummL = SummL >> 1;
  SummR = SummR >> 1;

}

void InitLCD(void)
{
  lcd.init();
  LCD_CreateCustomChars();
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
