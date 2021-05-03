/**
 * @file WatchDogESP8266.cpp
 * @copyright 2020 Stroud Water Research Center
 * Part of the EnviroDIY ModularSensors library for Arduino
 * @author Andreas Motl <andreas@hiveeyes.org>
 *
 * @brief Implements the extendedWatchDogESP8266 class.
 * @description In contrast to the other watchdog adapters, this does not
 *              implement early watchdog warnings yet.
 *
 */

#include "WatchDogESP8266.h"

// Be careful to use a platform-specific conditional include to only make the
// code visible for the appropriate platform.  Arduino will try to compile and
// link all .cpp files regardless of platform.
#if defined(ESP8266)

extendedWatchDogESP8266::extendedWatchDogESP8266() {}
extendedWatchDogESP8266::~extendedWatchDogESP8266() {
    disableWatchDog();
}


// One-time initialization of watchdog timer.
void extendedWatchDogESP8266::setupWatchDog(uint32_t resetTime_s) {
    _resetTime_s                          = resetTime_s;
    MS_DBG(F("Watch-dog timeout is set for"), _resetTime_s, F("sec"));
}


void extendedWatchDogESP8266::enableWatchDog() {
    MS_DBG(F("Enabling watch dog..."));
    ESP.wdtEnable(_resetTime_s * 1000);
}


void extendedWatchDogESP8266::disableWatchDog() {
    // Disable the watchdog
    ESP.wdtDisable();
}


void extendedWatchDogESP8266::resetWatchDog() {
    // Reset the watchdog.
    ESP.wdtFeed();
}

#endif
