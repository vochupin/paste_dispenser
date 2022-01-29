#include <Arduino.h>
#include <EEPROM.h>
#include "settings.h"

Settings _settings;

Settings* readSettings() {
    uint8_t chk = 0;

    for (int i = 0; i < ADR_CHECKSUM; i++){
        chk += EEPROM.read(i);
    }

    if (EEPROM.read(ADR_CHECKSUM) == chk) {
        _settings.pressureDelay = readEepromUInt16(ADR_PRESSURE_DELAY);
        _settings.pauseDelay = readEepromUInt16(ADR_PAUSE_DELAY);
        _settings.atmoDelay = readEepromUInt16(ADR_ATMO_DELAY);
    } else {
        _settings.pressureDelay = INIT_PRESSURE_DELAY;
        _settings.pauseDelay = INIT_PAUSE_DELAY;
        _settings.atmoDelay = INIT_ATMO_DELAY;
    }


    return &_settings;
}

uint16_t readEepromUInt16(int adr) {
    return (((uint16_t)EEPROM.read(adr)) << 8) | ((uint16_t)EEPROM.read(adr));
}
