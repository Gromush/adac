extern "C"{
  #include "defs.h"
}

extern char *filters[];
char * inputStr[] = {"USB", "TOS", "Coax", "Auto"}; 
#ifdef USE_MONITOR
MonStates_t gMonState = MON_MAX_VALUE;
#endif

Button_t gBConf;

void ButtonInit(void)
{
  pinMode(BUTTON_FILTER,INPUT_PULLUP);
  gBConf = B_MAX_VALUE;
}


void ButtonAction(SavedData_t * saved)
{
  static unsigned int mstart=0, mcurr = 0;;    // start timer
  static bool debounceDone = false;
  static bool isEnter = false;
  unsigned int timeout,len;
  char str[30]={0};
  
  if (!digitalRead(BUTTON_FILTER)) // Button pressed
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
     if (debounceDone)
     {
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
                break;
              case B_FILTER_CHANGE:
                gBConf = B_INPUT_CHANGE;
                LCD_Print(0,0, "Input mode ON.", true);
                sprintf(str, "Input: %s", inputStr[GetGConfig()->inputType]);
                LCD_Print(0,1, str, false);
                isEnter = true;
                break;
              case B_INPUT_CHANGE:
                PrintDisplay();
                gBConf = B_MAX_VALUE;
                isEnter = true;
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
            sprintf(str, "FW Ver: %s", VERSION_STRING);
            LCD_Print(0,0, str, true);
            delay(1500);
            PrintDisplay(); 
            break;
          case B_FILTER_CHANGE:
            
            sprintf(str, "Filter: %s", filters[SetFilter(saved, false) - FILTER_INDEX_OFFSET]);
            len = strlen(str);
            do{
              str[len] = ' ';
              len++;
            } while (len <= 16);
            LCD_Print(0,1, str, false);
            break;
          case B_INPUT_CHANGE:
            sprintf(str, "Input: %s", inputStr[SetInput(saved, false)]);
            len = strlen(str);
            do{
              str[len] = ' ';
              len++;
            } while (len <= 16);
            LCD_Print(0,1, str, false);
            
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
/*

void ButtonAction(SavedData_t * saved)
{
   static unsigned int mstart=0;
   static bool isSent=false;
   static bool button_state = false;
   
   if ((button_state) && (!isSent))
   {
      
#ifdef USE_MONITOR
      
      switch(gMonState)
      {
        case MON_START:
          MonitorON();
          WaitSerialChar(5000);
          gMonState = MON_SEND_F;
          break;
        case MON_SEND_F:
          SetMonitor();
          WaitSerialChar(5000);
          gMonState = MON_EXIT;
          break;
        case MON_EXIT:
          MonitorOFF();
          WaitSerialChar(5000);
          gMonState = MON_MAX_VALUE;
          isSent = true;
          break;
        default:
          break;
      }
     

#else
      SetFilter(saved, false);
      isSent = true;
#endif   
      return;
   }
   
   if (!digitalRead(BUTTON_FILTER))
   {
     
       // Buttone pressed
       if (mstart == 0)
       {
         mstart = millis();
       }
       if ( (millis() - mstart) > BUTTON_DEBOUNCE_TIMEOUT)
       {
          // debounce finished set up button action
          if ((button_state == false) && (!isSent))
          {
             
#ifdef USE_MONITOR
             gMonState = MON_START;
#endif
             button_state = true;
             SetPwrRdyOff();

          }
          
       } 
   } else
   {
      mstart = 0;
      button_state = false;
      isSent = false;
      SetPwrRdyOn();
   }
}
*/
