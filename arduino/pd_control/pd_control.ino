
#include <TM1638plus.h> //include the library

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
//pick on any I/O you want.
#define  STROBE_TM 4 // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 3  // clock = GPIO connected to clock line of module
#define  DIO_TM 2 // data = GPIO connected to data line of module
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true. 
 
// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);

void setup()
{
  tm.displayBegin(); 
}

void loop()
{
  tm.displayText("12349879");
  delay(50);
}
