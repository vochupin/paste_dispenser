#include <Arduino.h>
#include <TM1638plus.h> //include the library

void showMenu();

//Menu states
#define MST_PRESSURE_DELAY  1
#define MST_PAUSE           2
#define MST_ATMO_DELAY      3

//Key definitions
#define KEY_PRESSURE_DELAY    1
#define KEY_PAUSE             2
#define KEY_ATMO_DELAY        4

//Relay control pins
#define PRESSURE_RELAY  7
#define ATMO_RELAY      8

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
//pick on any I/O you want.
#define  STROBE_TM 4 // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 5  // clock = GPIO connected to clock line of module
#define  DIO_TM 6 // data = GPIO connected to data line of module
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true. 
 
// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);

char buffer[40];

uint16_t pressureDelay = 50;
uint16_t pauseDelay = 10;
uint16_t atmoDelay = 50;

uint8_t menuState = MST_PRESSURE_DELAY;

void setup() {
  pinMode(PRESSURE_RELAY, OUTPUT);
  pinMode(ATMO_RELAY, OUTPUT);

  digitalWrite(PRESSURE_RELAY, LOW);
  digitalWrite(ATMO_RELAY, LOW);

  tm.displayBegin(); 
}

void loop() {
  showMenu();

  // digitalWrite(PRESSURE_RELAY, HIGH);
  // delay(1000);
  // digitalWrite(PRESSURE_RELAY, LOW);

  // delay(500);

  // digitalWrite(ATMO_RELAY, HIGH);
  // delay(1000);
  // digitalWrite(ATMO_RELAY, LOW);

  // delay(500);
}

void showMenu() {
  uint8_t btns = tm.readButtons();

  if (btns & KEY_PRESSURE_DELAY) {
    menuState = MST_PRESSURE_DELAY;
  } else if (btns & KEY_PAUSE) {
    menuState = MST_PAUSE;
  } else if (btns & KEY_ATMO_DELAY) {
    menuState = MST_ATMO_DELAY;
  }

  uint16_t currentMenuValue = 0;

  const char* menuPrefix = "???";
  switch (menuState)
  {
  case MST_PRESSURE_DELAY:
    menuPrefix = "Pd ";
    currentMenuValue = pressureDelay;
    break;
  case MST_PAUSE:
    menuPrefix = "PA ";
    currentMenuValue = pauseDelay;
    break;
  case MST_ATMO_DELAY:
    menuPrefix = "Ad ";
    currentMenuValue = atmoDelay;
    break;
  }

  sprintf(buffer, "%s%05d", menuPrefix, currentMenuValue);

  tm.displayText(buffer);
  
  delay(100);
}