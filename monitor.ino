#include "defs.h"

char * filters[] = {"linear", "mixed", "minimum", "soft"};

unsigned int SetInput(SavedData_t * saved, bool isFirst)
{
  unsigned char input_num;
  char comm[5]={0};
  input_num = GetGConfig()->inputType;
  if (isFirst)
  {
    if (saved->bytes.input != input_num)
    {
      input_num = saved->bytes.input;
    } else
    {
      return input_num;
    }
  }
  else
  {
    input_num++;
    if (input_num > I_AUTO)
    {
      input_num = I_USB;
    }
  }
 
 sprintf(comm, "I%d",input_num);
 PrintSerialLine(comm);
 if (!isFirst)
 {
   saved->bytes.input = input_num;
   EEPROM.update(SAVED_ADDR+1,saved->bytes.input);
 }
 return input_num; 
}

unsigned int SetFilter(SavedData_t * saved, bool isFirst)
{
 unsigned char filter_num;
 char comm[5]={0};
 filter_num = GetGConfig()->FilterNum;
 if (isFirst)
 {
   if (saved->bytes.filter != filter_num)
   {
      filter_num = saved->bytes.filter;
   } else
   {
     return filter_num;
   }
 } else
 {
   filter_num++;
   if (filter_num > MAX_FILTER_NUM)
   {
     filter_num = MIN_FILTER_NUM;
   }
 }
 
 sprintf(comm, "F%d",filter_num);
 PrintSerialLine(comm);
 if (!isFirst)
 {
   saved->bytes.filter = filter_num;
   EEPROM.update(SAVED_ADDR,saved->bytes.filter);
 }

 return filter_num; 
}
