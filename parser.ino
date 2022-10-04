extern "C"{
  #include "defs.h"
}


Config_t gConfig = {UNINIT_VAL};
extern Button_t gBConf;

// Get
Config_t *GetGConfig(void)
{
  return &gConfig;
}


void PrintDisplay(void)
{
  
  LCD_Clear();
  LCD_PrintSmallLogo(0,0);
  printFilter(gConfig.FilterNum);
  printStream(gConfig.streamValue);
  printInput(gConfig.inputType);
}

/////////// Print stream found value
void printStream(int num)
{
  char str[20]={0};
  LCD_Print(0,1, "        ", false);
  if (gConfig.streamType ==  S_PCM)
  {
    sprintf(str, "PCM %dK",num);
  } else
  {
    sprintf(str, "DSD %dM",num);
  }
  LCD_Print(0,1, str, false);
  
}

/// print filter found value
void printFilter(int num)
{
  char str[40]={0};
  int len,i;
  sprintf(str, "Fltr: %s",filters[num-FILTER_INDEX_OFFSET]);
  len = strlen(str);
  
  do{
    str[len] = ' ';
    len++;
  } while (len <= 16);
 
  LCD_Print(3,0, str, false);
}

// print active input
void printInput(int num)
{
 lcd.setCursor(8,0);
 switch (num)
 {
  case 3:
    LCD_Print(8,1, "In: Auto", false);
    break;
  case 2:
    LCD_Print(8,1, "In: Coax", false);
    break;
  case 1:
    LCD_Print(8,1, "In: TOS ", false);
    break;
  case 0:
    LCD_Print(8,1, "In: USB ", false);
    break;
 }
}

int currentCounter;

// Main parser function
ParseResult_t GetParse(char *ch)
{
  static States_t gState = START;
  static String inString = "";
  
  switch (gState)
  {
    case START: 
      inString = "";
      currentCounter = 0;
      if (ch == 'F')
      {
        gState = PARSE_FILTER;
        break;
      }
      if (ch == 'I')
      {
        gState = PARSE_INPUT;
        break;
      }
      if (ch == 'L')
      {
        gState = PARSE_STREAM;
      } 
      break;
      
    case PARSE_FILTER:
      if (ch != '\r') 
      {
         inString += (char)ch;
         currentCounter++;
         return FOUND_NONE;
      }
      if (currentCounter > 1)
      {
         gState = START; // big part after F not filter output   
         return FOUND_NONE;
      }
      gConfig.FilterNum = inString.toInt();
      gState = START;
      return FOUND_FIL;
      
    case PARSE_INPUT:
      
      if (ch != '\r') 
      {
         inString += (char)ch;
         currentCounter++;
         return FOUND_NONE;
      }
      if (currentCounter > 1)
      {
         gState = START; // big part after F not filter output
         
         return FOUND_NONE;
      }
      gConfig.inputType = inString.toInt();   
      gState = START;
      return FOUND_IN;
      
    case PARSE_STREAM:
      
      if (ch !='\r')
      {
        inString += (char)ch;
        currentCounter++;
        return FOUND_NONE;
      }
      if (currentCounter > 4)
      {
         gState = START; // big part after F not filter output
         return FOUND_NONE;
      }
      gConfig.streamValue = inString.toInt();
      gState = START;
      return FOUND_STR;
          
    default:
       return FOUND_NONE;
  }

  return FOUND_NONE;
}


/// Main Get and print serial data 
void GetDACDataSerial()
{
  ParseResult_t parser = FOUND_NONE;
  
  while(Serial.available())
  {
    
   parser = GetParse(GetSerial());
   if (gBConf == B_MAX_VALUE)
   {
     switch (parser)
      {
        case FOUND_FIL:
          printFilter(gConfig.FilterNum);
          break;
        case FOUND_STR:
          printStream(gConfig.streamValue);
          break;
        case FOUND_IN:
          printInput(gConfig.inputType);
          break;
        default:
          break;
      }
   }
  } // while
}
