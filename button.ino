extern "C"{
  #include "defs.h"
}

#ifdef USE_MONITOR
MonStates_t gMonState = MON_MAX_VALUE;
#endif

void ButtonInit(void)
{
  pinMode(BUTTON_FILTER,INPUT_PULLUP);
}



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
