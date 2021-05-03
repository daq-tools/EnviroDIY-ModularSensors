/**
 * @file WatchDogESP32.cpp
 * @copyright 2020 Stroud Water Research Center
 * Part of the EnviroDIY ModularSensors library for Arduino
 * @author Andreas Motl <andreas@hiveeyes.org>
 *
 * @brief Implements the extendedWatchDogESP32 class.
 * @description In contrast to the other watchdog adapters, this does not
 *              implement early watchdog warnings yet.
 * @source Derived from https://github.com/espressif/arduino-esp32/blob/5502879/libraries/ESP32/examples/Timer/WatchdogTimer/WatchdogTimer.ino
 *
 */

#include "WatchDogESP32.h"

// Be careful to use a platform-specific conditional include to only make the
// code visible for the appropriate platform.  Arduino will try to compile and
// link all .cpp files regardless of platform.
#if defined(ESP32)

#include "esp_system.h"


void ARDUINO_ISR_ATTR resetModule() {
  ets_printf("WDT: Reboot\n");
  esp_restart();
}

hw_timer_t *timer = NULL;


extendedWatchDogESP32::extendedWatchDogESP32() {}
extendedWatchDogESP32::~extendedWatchDogESP32() {
    disableWatchDog();
}


// One-time initialization of watchdog timer.
void extendedWatchDogESP32::setupWatchDog(uint32_t resetTime_s) {
    _resetTime_s                          = resetTime_s;
    timer = timerBegin(0, 80, true);                  //timer 0, div 80
    timerAttachInterrupt(timer, &resetModule, true);  //attach callback
    timerAlarmWrite(timer, _resetTime_s * 1000 * 1000, false); //set time in us
    MS_DBG(F("Watch-dog timeout is set for"), _resetTime_s, F("sec"));
}


void extendedWatchDogESP32::enableWatchDog() {
    MS_DBG(F("Enabling watch dog..."));
    // Enable interrupt
    timerAlarmEnable(timer);
}


void extendedWatchDogESP32::disableWatchDog() {
    // Disable the watchdog
    timerAlarmDisable(timer);
}


void extendedWatchDogESP32::resetWatchDog() {
    // Feed the watchdog (reset the timer)
    timerWrite(timer, 0);
}

#endif
