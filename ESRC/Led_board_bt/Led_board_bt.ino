#include <MD_MAX72xx.h>
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(0, 1); 
#include <SPI.h>
#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 11
#define CLK_PIN   13  
#define DATA_PIN  11  
#define CS_PIN    10  
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
#if USE_POT_CONTROL
#define SPEED_IN  A5
#else
#define SCROLL_DELAY  75 
#endif 
#define CHAR_SPACING  1 
#define BUF_SIZE  75
char curMessage[BUF_SIZE];
char curMessageBT[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;
uint16_t  scrollDelay; 
void readBt(void)
{
  static uint8_t  putIndex = 0;
  while (bluetooth.available())
  {
    newMessage[putIndex] = (char)bluetooth.read();
    if ((newMessage[putIndex] == '\n') || (putIndex >= BUF_SIZE-3)) 
    {
      newMessage[putIndex++] = '\n\r ';
      newMessage[putIndex] = '\0';
      putIndex = 0;
      newMessageAvailable = true;
    }
    else if (newMessage[putIndex] != '\n\r')
        putIndex++;
  }
}
void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col)
{
#if PRINT_CALLBACK
  Serial.print("\n cb ");
  Serial.print(dev);
  Serial.print(' ');
  Serial.print(t);
  Serial.print(' ');
  Serial.println(col);
#endif
}

uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t)
{
  static char   *p = curMessage;
  static uint8_t  state = 0;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[8];
  uint8_t colData;
  switch(state)
  {
    case 0:
      showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state++;
      if (*p == '\0')
      {
        p = curMessage;     
        if (newMessageAvailable)  
        {
          strcpy(curMessage, newMessage); 
          newMessageAvailable = false;
        }
      }
    case 1: 
      colData = cBuf[curLen++];
      if (curLen == showLen)
      {
        showLen = CHAR_SPACING;
        curLen = 0;
        state = 2;
      }
      break;

    case 2:
      colData = 0;
      curLen++;
      if (curLen == showLen)
        state = 0;
      break;

    default:
      state = 0;
  }
  return(colData);
}
 void scrollText(void)
{
  static uint32_t prevTime = 0;
  if (millis()-prevTime >= scrollDelay)
  {
    mx.transform(MD_MAX72XX::TSL);  
    prevTime = millis();   
  }
}
uint16_t getScrollDelay(void)
{
#if USE_POT_CONTROL
  uint16_t  t;
  t = analogRead(SPEED_IN);
  t = map(t, 0, 1023, 25, 250);
  return(t);
#else
  return(SCROLL_DELAY);
#endif
}
void setup()
{
  bluetooth.begin(9600);
  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.setShiftDataOutCallback(scrollDataSink);
#if USE_POT_CONTROL
  pinMode(SPEED_IN, INPUT);
#else
  scrollDelay = SCROLL_DELAY;
#endif
  strcpy(curMessage, "TEAM 05 ""The Robo Wizards"" ");
  newMessage[0] = '\0';
  Serial.begin(9600);
  bluetooth.print("\n[MD_MAX72XX Message Display]\nType a message for the scrolling display\nEnd message line with a newline");
}
void loop()
{
  scrollDelay = getScrollDelay();
  readBt();
  strcpy( curMessage, newMessage);
  scrollText();
}
