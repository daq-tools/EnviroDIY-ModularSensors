/**
 * @file WatchDogESP8266.h
 * @copyright 2020 Stroud Water Research Center
 * Part of the EnviroDIY ModularSensors library for Arduino
* @author Andreas Motl <andreas@hiveeyes.org>
  *
 * @brief Contains the extendedWatchDogESP8266 class
 * @description In contrast to the other watchdog adapters, this does not
 *              implement early watchdog warnings yet.
 *
 */

// Header Guards
#ifndef SRC_WATCHDOGS_WATCHDOGESP8266_H_
#define SRC_WATCHDOGS_WATCHDOGESP8266_H_

// Debugging Statement
// #define MS_WATCHDOGESP8266_DEBUG

#ifdef MS_WATCHDOGESP8266_DEBUG
#define MS_DEBUGGING_STD "WatchDogESP8266"
#endif

// Included Dependencies
#include "ModSensorDebugger.h"
#undef MS_DEBUGGING_STD

/**
 * @brief The extendedWatchDogESP8266 class implements the minimal things
 * needed to make the code compile.
 *
 * FIXME: This situation should be improved in order to align with the other
 *        watchdog adapters. We should look at a) maximum sleep intervals
 *        and b) early watchdog warnings.
 *
 * @ingroup base_classes
 */
class extendedWatchDogESP8266 {
public:
    /**
     * @brief Construct a new extended watch dog object for ESP8266 processors.
     */
    extendedWatchDogESP8266();
    /**
     * @brief Destroy the extended watch dog object for ESP8266 processors.
     */
    ~extendedWatchDogESP8266();

    /**
     * @brief One-time initialization of watchdog timer.
     *
     * @param resetTime_s The length of time in seconds between resets of the
     * watchdog before the entire board is reset.
     */
    void setupWatchDog(uint32_t resetTime_s);
    /**
     * @brief Enable the watchdog.
     */
    void enableWatchDog();
    /**
     * @brief Disable the watchdog.
     */
    void disableWatchDog();

    /**
     * @brief Reset the watchdog's clock to prevent the board from resetting.
     */
    void resetWatchDog();

private:
    uint32_t _resetTime_s;
};

#endif  // SRC_WATCHDOGS_WATCHDOGESP8266_H_
