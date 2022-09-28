#include "defs.h"


#define FILTER_INDEX_OFFSET 4

char * filters[] = {"linear", "mixed", "minimum", "soft"};


#ifdef USE_MONITOR
bool MonitorON(void)
{
  // Start monitor
  PrintSerial(MONITOR_ON_STR);
  return true;
}


void MonitorOFF(void)
{
  PrintSerialLine(MONITOR_OFF_STR);
}



void SetMonitor(void)
{
 static unsigned char filter_num;
 filter_num = GetGConfig()->FilterNum;
 filter_num++;
 if (filter_num > MAX_FILTER_NUM)
 {
   filter_num = MIN_FILTER_NUM;
 }
 
 while(1)
 {
  
   if (filter_num == 4)
   {
     PrintSerialLine("set filter linear");
     break;
   }
   if (filter_num == 5)
   {
     PrintSerialLine("set filter mixed");
     break;
   }

   if (filter_num == 6)
   {
     PrintSerialLine("set filter minimum");   
     break;
   }

   if (filter_num == 7)
   {
     PrintSerialLine("set filter soft");    
     break;
   }
   PrintSerialLine("");
   break;
 }
  
}
#endif // USE_MONITOR
void SetFilter(SavedData_t * saved, bool isFirst)
{
 static unsigned char filter_num;
 char comm[5]={0};
 filter_num = GetGConfig()->FilterNum;
 if (saved->bytes.filter != filter_num)
 {
    if (isFirst)
    {
      filter_num = saved->bytes.filter;
    }
 }
 if (!isFirst)
 {
    filter_num++;
 }
 
 if (filter_num > MAX_FILTER_NUM)
 {
   filter_num = MIN_FILTER_NUM;
 }
 
 sprintf(comm, "F%d",filter_num);
 PrintSerialLine(comm);
 if (!isFirst)
 {
   saved->bytes.filter = filter_num;
   EEPROM.update(SAVED_ADDR,saved->all16);
 }
 
}
