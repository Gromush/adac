#ifndef _DEFS_H
#define _DEFS_H

#define VERSION_STRING "V1.0.7" // current FW code version

#define SERIAL_SPEED 115200

#define INFINIT 0

// LCD
#define LCD_I2C_ADDRESS 0x27
#define LCD_SYMBOLS_NUM 16
#define LCD_LINES_NUM 2

#define LED_MAIN_PWR   2
#define LED_PWR_RDY    3
#define DAC_PWR_ENABLE 4
#define BUTTON_FILTER  5

#define FILTER_INDEX_OFFSET 4
#define UNINIT_VAL          0x55

#define BUTTON_DEBOUNCE_TIMEOUT   50   // 50 ms
#define BUTTON_LONG_PRESS_TIMEOUT 1000 // 1 s
#define RETURN_MAIN_MODE_TIMER    15000 // 15s

#define LED_FILTER_MODE_BLINK_TIMEOUT  200
#define LED_INPUT_MODE_BLINK_TIMEOUT   100

#define MAX_FILTER_NUM           7 // last filter
#define MIN_FILTER_NUM           4 // first filter

#define SAVED_ADDR_FILTER        0
#define SAVED_ADDR_INPUT         1

typedef enum
{
  I_USB,
  I_TOS,
  I_COAX,
  I_AUTO
} Input_Type_t;

typedef enum
{
  START,
  PARSE_STREAM,
  PARSE_FILTER,
  PARSE_INPUT,

} States_t;

typedef enum
{
  FOUND_NONE,
  FOUND_FIL,    // filter changed
  FOUND_STR,    // stream changed
  FOUND_IN      // input changed
} ParseResult_t;

typedef enum {
  S_PCM,
  S_DSD
} Streams_t;


typedef struct
{
  Streams_t streamType;
  int streamValue;
  unsigned char FilterNum;
  Input_Type_t inputType;
} Config_t;

// EEPROM
typedef union
{
  unsigned int all16;
  struct
  {
    unsigned char filter;
    unsigned char input;
  } bytes;
  
} SavedData_t; 


typedef enum 
{
  B_FILTER_CHANGE,
  B_INPUT_CHANGE,
  B_MAX_VALUE
} Button_t;



#endif
