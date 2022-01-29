#include <Arduino.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <TM1638plus.h> //include the library

//STR2INT errors
#define STR2INT_SUCCESS         0
#define STR2INT_INCONVERTIBLE   1
#define STR2INT_OVERFLOW        2
typedef uint8_t str2int_errno;

// Menu states
#define MST_PRESSURE_DELAY 1
#define MST_PAUSE 2
#define MST_ATMO_DELAY 3

// Key definitions
#define KEY_PRESSURE_DELAY 1
#define KEY_PAUSE 2
#define KEY_ATMO_DELAY 4
#define KEY_10_5 8
#define KEY_10_4 16
#define KEY_10_3 32
#define KEY_10_2 64
#define KEY_10_1 128

// Relay control pins
#define PRESSURE_RELAY 7
#define ATMO_RELAY 8

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
// pick on any I/O you want.
#define STROBE_TM 4     // strobe = GPIO connected to strobe line of module
#define CLOCK_TM 5      // clock = GPIO connected to clock line of module
#define DIO_TM 6        // data = GPIO connected to data line of module
bool high_freq = false; // default false, If using a high freq CPU > ~100 MHZ set to true.

void showMenu();
void updateFromCurrentValue();
void changeDecimalPlace(uint8_t place);
str2int_errno str2uint16(uint16_t *out, char *s);

// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM, high_freq);

char buffer[40];

uint16_t pressureDelay = 50;
uint16_t pauseDelay = 10;
uint16_t atmoDelay = 50;

uint16_t currentMenuValue = 0;

uint8_t menuState = MST_PRESSURE_DELAY;
uint8_t oldBtns = 0;

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
  const char *menuPrefix = "???";

  switch (menuState) {
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

  uint8_t btns = tm.readButtons();

  if (oldBtns == 0) {
    if (btns & KEY_PRESSURE_DELAY) {
      menuState = MST_PRESSURE_DELAY;
      currentMenuValue = pressureDelay;
    } else if (btns & KEY_PAUSE) {
      menuState = MST_PAUSE;
      currentMenuValue = pauseDelay;
    } else if (btns & KEY_ATMO_DELAY) {
      menuState = MST_ATMO_DELAY;
      currentMenuValue = atmoDelay;
    } else if (btns & KEY_10_5) {
      changeDecimalPlace(5);
      updateFromCurrentValue();
    } else if (btns & KEY_10_4) {
      changeDecimalPlace(4);
      updateFromCurrentValue();
    } else if (btns & KEY_10_3) {
      changeDecimalPlace(3);
      updateFromCurrentValue();
    } else if (btns & KEY_10_2) {
      changeDecimalPlace(2);
      updateFromCurrentValue();
    } else if (btns & KEY_10_1) {
      changeDecimalPlace(1);
      updateFromCurrentValue();
    }
  }

  oldBtns = btns;

  sprintf(buffer, "%s%05u", menuPrefix, currentMenuValue);
  tm.displayText(buffer);

  delay(100);
}

void changeDecimalPlace(uint8_t place) {
  sprintf(buffer, "%05u", currentMenuValue);
  
  char symbol = buffer[5 - place];

  if (symbol == '9') {
    symbol = '0';
  } else {
    symbol++;
  }

  buffer[5 - place] = symbol;

  uint16_t out = 0;
  str2int_errno resultCode = str2uint16(&out, buffer);
  if (resultCode == STR2INT_SUCCESS) {
    currentMenuValue = out;
  } else if (resultCode == STR2INT_OVERFLOW) {
    buffer[5 - place] = '0';
    resultCode = str2uint16(&out, buffer);
    
    if (resultCode == STR2INT_SUCCESS) {
      currentMenuValue = out;
    }
  }
}

void updateFromCurrentValue() {
  switch (menuState) {
  case MST_PRESSURE_DELAY:
    pressureDelay = currentMenuValue;
    break;
  case MST_PAUSE:
    pauseDelay = currentMenuValue;
    break;
  case MST_ATMO_DELAY:
    atmoDelay = currentMenuValue;
    break;
  }
}

str2int_errno str2uint16(uint16_t *out, char *s) {
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    long l = strtol(s, &end, 10);
    if (l > UINT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
}
