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

byte s_lg_1[] = {
  B01100,
  B01100,
  B11110,
  B00000,
  B00011,
  B00011,
  B11101,
  B11101
 };

 byte s_lg_2[] = {
  B00110,
  B00110,
  B01111,
  B00000,
  B11000,
  B11000,
  B10111,
  B10111
 };
void LCD_CreateCustomLogo()
{
  lcd.createChar(0,logo_up);
  lcd.createChar(1,logo_down);
  lcd.createChar(2,logo_c);
  lcd.createChar(3,s_lg_1);
  lcd.createChar(4,s_lg_2);
}


void LCD_PrintSmallLogo(int x, int y)
{
  lcd.setCursor(x,y);
  lcd.write(3);
  lcd.setCursor(x+1,y);
  lcd.write(4);
}




void LCD_PritLogo(int x, int y)
{
  lcd.setCursor(x,y);
  lcd.write(0);
  lcd.setCursor(x+2,y);
  lcd.write(0);
  lcd.setCursor(x+1,y+1);
  lcd.write(1);
  lcd.setCursor(x,y+1);
  lcd.write(2);
  lcd.setCursor(x+2,y+1);
  lcd.write(2);
  
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
