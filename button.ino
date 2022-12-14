extern "C"{
  #include "defs.h"
}

extern char *filters[];
extern char VersionString[16];
char * inputStr[] = {"USB", "TOS", "Coax", "Auto"}; 

Button_t gBConf;
unsigned int retTimer=0;

void ButtonInit(void)
{
  pinMode(BUTTON_FILTER,INPUT_PULLUP);
  gBConf = B_MAX_VALUE;
}

void ReturnToMainMode(void)
{
  unsigned int exittime;
  unsigned int wrapValue=0;
  if (retTimer != 0)
  {
    exittime = millis();
    wrapValue = exittime - retTimer;
    
    
    if (wrapValue > RETURN_MAIN_MODE_TIMER)
    {
       if (GetGConfig()->mode == MODE_NORMAL) 
       {
          PrintDisplay(true);
          retTimer = 0;
          gBConf = B_MAX_VALUE;
       }
    }    
  }
}

void ButtonAction(SavedData_t * saved)
{
  static unsigned int mstart=0, mcurr = 0;    // start timer
  static bool debounceDone = false;
  static bool isEnter = false;
  unsigned int timeout,len;
  char str[30]={0};
  
  if (!digitalRead(BUTTON_FILTER)) // Button pressed
  {

    if (!debounceDone)
    {
      if (mstart == 0) 
        mstart = millis();
      
      mcurr = millis();
      timeout = mcurr - mstart;
      if (!debounceDone)
      {
        if (timeout > BUTTON_DEBOUNCE_TIMEOUT)
        {
          debounceDone = true;
          return;
        }
      }
    }
    else
    {
      if (GetGConfig()->mode != MODE_NORMAL)
      {
        return;
      }
      mcurr = millis();
      timeout = mcurr - mstart;
      if (timeout > BUTTON_LONG_PRESS_TIMEOUT)
      {
         if (!isEnter)
         {      
           // Change current state to filter setup
           switch(gBConf)
           {
            case B_MAX_VALUE:
              gBConf = B_FILTER_CHANGE;
              LCD_Print(0,0, "Filter mode ON.", true);
              sprintf(str, "Filter: %s", filters[GetGConfig()->FilterNum - FILTER_INDEX_OFFSET]);
              LCD_Print(0,1, str, false);
              isEnter = true;
              retTimer = millis();
              break;
            case B_FILTER_CHANGE:
              gBConf = B_INPUT_CHANGE;
              LCD_Print(0,0, "Input mode ON.", true);
              sprintf(str, "Input: %s", inputStr[saved->bytes.input]);
              LCD_Print(0,1, str, false);
              isEnter = true;
              retTimer = millis();
              break;
            case B_INPUT_CHANGE:
              PrintDisplay(true);
              SetPwrRdyOn();
              gBConf = B_MAX_VALUE;
              isEnter = true;
              retTimer = 0;
              break;
           }
         }
      }
    }
     
  } else
  {
    // Button unpressed after long press
    if (isEnter == true)
    {
      isEnter = false;
    } else
    {
      // short time unpressing
      if (debounceDone) 
      {    
        switch (gBConf)
        {
          case B_MAX_VALUE: // short press only - show version
            if (GetGConfig()->mode == MODE_NORMAL)
            {
              GetGConfig()->mode = MODE_MUSIC;
              PrepareIndicators();
            } else
            {
              GetGConfig()->mode = MODE_NORMAL;
              PrintDisplay(true);
            }
                  
            break;
          case B_FILTER_CHANGE:
            sprintf(str, "Filter: %s", filters[SetFilter(saved, false) - FILTER_INDEX_OFFSET]);
            len = strlen(str);
            memcpy(&str[len],32,LCD_SYMBOLS_NUM-len);
            str[LCD_SYMBOLS_NUM] = '\0';
            LCD_Print(0,1, str, false);
            retTimer = millis();
            break;
          case B_INPUT_CHANGE:
            sprintf(str, "Input: %s", inputStr[SetInput(saved, false)]);
            len = strlen(str);
            memcpy(&str[len],32,LCD_SYMBOLS_NUM-len);
            str[LCD_SYMBOLS_NUM] = '\0';
            LCD_Print(0,1, str, false);
            retTimer = millis();
            break;
        }
      }
    }
    // always clean data by unpressing
    mstart = 0;
    mcurr = 0;   
    timeout = 0;
    debounceDone = false;
  }
}
