#ifndef SETTINGS_MODULE

#include <stdio.h>

#define SETTINGS_MODULE

#define ADR_PRESSURE_DELAY 0
#define ADR_PAUSE_DELAY 2
#define ADR_ATMO_DELAY 4
#define ADR_CHECKSUM 6

#define INIT_PRESSURE_DELAY 500
#define INIT_PAUSE_DELAY 100
#define INIT_ATMO_DELAY 700

typedef struct {
    uint16_t pressureDelay;
    uint16_t pauseDelay;
    uint16_t atmoDelay;
    uint8_t checkSum;
} Settings;

Settings* readSettings();
uint16_t readEepromUInt16(int adr);
void writeEepromUInt16(int adr, uint16_t value);
void writeSettings();

#endif