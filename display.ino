#include <LiquidCrystal_I2C.h>
extern "C"{
  #include "defs.h"
}
// definitions 


LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_SYMBOLS_NUM, LCD_LINES_NUM);

void InitLCD(void)
{
  lcd.init();
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
#if 0
void LCD_ShiftLeft(void)
{
  lcd.scrollDisplayLeft();
}

void LCD_ShiftRight(void)
{
  lcd.scrollDisplayRight();
}
#endif //0

void LCD_Clear(void)
{
  lcd.clear();
}
