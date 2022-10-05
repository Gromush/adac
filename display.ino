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
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
 };

 byte indUp[] = {
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
 };

  byte indL[] = {
  B00000,
  B00000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11110,
  B00000
 }; 
 
 byte indR[] = {
  B00000,
  B11100,
  B10010,
  B11100,
  B11000,
  B10100,
  B10010,
  B00000
 };
 
void LCD_CreateCustomLogo()
{
  lcd.createChar(LOGO_UP,logo_up);
  lcd.createChar(LOGO_DWN,logo_down);
  lcd.createChar(LOGO_C,logo_c);
  lcd.createChar(IND_CNAR_NORM,indN);
  lcd.createChar(IND_CNAR_MAX, indUp);
  lcd.createChar(IND_CNAR_L,indL);
  lcd.createChar(IND_CNAR_R, indR);
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
  unsigned int left, right;
  static unsigned int maxl=0, maxr=0;
  unsigned int xl=1,xr=1, xlup=0, xrup=0;
  byte exitFlag=0;

  if (GetGConfig()->mode == MODE_NORMAL)
  {
    return;
  }
  //char ss[5];
  lcd.setCursor(0,0);
  lcd.write(IND_CNAR_L);
  lcd.setCursor(0,1);
  lcd.write(IND_CNAR_R);
  
  left = analogRead(LEFT_CHANNEL);
  right = analogRead(RIGHT_CHANNEL);
  if (maxl< left) { maxl=left;}
  if (maxr< right) { maxr=right;}
  if (left > IND_MAX_VALUE) left = IND_MAX_VALUE;
  if (right > IND_MAX_VALUE) right = IND_MAX_VALUE;
  xlup = left / IND_DELIMITER;
  xrup = right / IND_DELIMITER;
//  sprintf(ss, "%d", xlup);
//  LCD_Print(0,0, ss, false);
// sprintf(ss, "%d", xrup);
// LCD_Print(0,1, ss, false);

  while(exitFlag!=3)
  {
    lcd.setCursor(xl,0);
    if (xl<=xlup)
    {
      if (xl >= (IND_MAX_POINT-3))
      {
        lcd.write(IND_CNAR_MAX);
      } else {
      lcd.write(IND_CNAR_NORM);
      }
    } else
    {
      lcd.print(" ");
    }
    lcd.setCursor(xr,1);
    if (xr<=xrup)
    {
      if (xr >= (IND_MAX_POINT-3))
      {
        lcd.write(IND_CNAR_MAX);
      } else{
        lcd.write(IND_CNAR_NORM);
      }

    } else
    {
      lcd.print(" ");
    }
    if (xl>=IND_MAX_POINT)
    {
      exitFlag |= 1;
    }
    if (xr>=IND_MAX_POINT)
    {
      exitFlag |= (1<<1);
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
