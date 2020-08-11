/**
 * @file MaxBotixSonar.h
 * @copyright 2020 Stroud Water Research Center
 * Part of the EnviroDIY ModularSensors library for Arduino
 * @author Sara Geleskie Damiano <sdamiano@stroudcenter.org>
 *
 * @brief Contains the MaxBotixSonar sensor subclass and the MaxBotixSonar_Range
 * variable subclass.
 *
 * These are for the MaxBotix HRXL-MaxSonar ultrasonic range finders.
 *
 * @defgroup maxbotics_group MaxBotix MaxSonar
 * Classes for the @ref maxbotics_page
 *
 * @copydoc maxbotics_page
 */
/* clang-format off */
/**
 * @page maxbotics_page MaxBotix MaxSonar
 *
 * @tableofcontents
 *
 * @section maxbotics_intro Introduction
 *
 * The IP67 rated HRXL-MaxSonar-WR ultrasonic rangefinders offer 1mm
 * resolution, 2.7-5.5VDC operation, a narrow beam pattern, high power output,
 * noise rejection, automatic calibration, and temperature compensation.
 * Depending on the precise model, the range finders have ranges between 300 and
 * 9999mm and read rates of 6-7.5Hz.  This library supports TTL or RS323 sensor
 * output, though an RS232-to-TTL adapter is needed for the RS232 models. Analog
 * and pulse-width outputs are not supported.  The MaxBotix sensors require
 * a 2.7V - 5.5V power supply to pin 6 on the sensor (which can be turned off
 * between measurements) and the level of the TTL returned by the MaxSonar will
 * match the power level it is supplied with.  The digital TTL or RS232 output
 * is sent out on pin 5 on the sensor.  Pin 7 of the MaxSonar must be connected
 * to power ground and pin 4 can optionally be used to trigger the MaxSonar.
 *
 * If you are using the
 * [MaxBotix HR-MaxTemp](https://www.maxbotix.com/Ultrasonic_Sensors/MB7955.htm)
 * MB7955 temperature compensator on your MaxBotix (which greatly improves data
 * quality), the red wire from the MaxTemp should be attached to pin 1
 * (square) on the MaxSonar.  The white and shield (bare silver) wires from the
 * MaxTemp should both be attached to Pin 7 (GND).  The MaxTemp communicates
 * directly with the MaxSonar and there is no need to make any changes on the
 * Aruduino itself to accomodate it.  It is not possible to read the temperature
 * data from the MaxTemp.
 *
 * The MaxBotix sensor have two different modes: free-ranging and triggered.
 * Unless the trigger pin is externally held low, the sensor will continuously
 * take readings at a rate of 6Hz or greater and immediate report each result
 * over the digital output pin.  (That is, it will be in free-ranging mode.)
 * When continuously powered and operating in free-range mode, the data output
 * is automatically filtered to help improve accuracy.  If you are turning the
 * power to the sensor off between readings, there is no advantage to using the
 * free-ranging because many readings must be taken before the filter becomes
 * effective.  In this case, you may save a very small amount of power by
 * setting up a trigger pin and manually trigger individual readings.
 *
 * Please see the section
 * "[Notes on Arduino Streams and Software Serial](https://github.com/EnviroDIY/ModularSensors/wiki/Arduino-Streams)"
 * for more information about what streams can be used along with this library.
 *
 * This library supports using multiple MaxBotix sensors on the same logger,
 * with a few caveats:
 *  - Any sensor operating in free-ranging mode (powered at the same time as any
 * other sensors with the trigger pins unconnected) must have a dedicated stream
 * instance/serial port.
 *  - To have two sensors operating in free-ranging mode, they must each have a
 * dedicated stream instance/serial port *AND* you must specify a unique
 * _negative_ pin number for the trigger pin. Giving a negative pin number
 * ensures that the Arduino will not attempt to trigger trigger individual
 * readings but will still be able to tell the sensors apart. (Software-wise,
 * simply specifying the different streams is not enough!)  Keep in mind that
 * two or more free ranging sensors must be spaced far enough apart in the field
 * to prevent interference between the sonar beams.
 *  - Two or more sensors may send data to the same stream instance/serial port
 * if both sensors are being triggered and each is triggered by a different
 * trigger pin.
 *  - "Daisy chaining" sensors so the pulse-width output of one sensor acts as
 * the trigger for a second sensor *is not supported*.
 *
 * @section maxbotics_datasheet Sensor Datasheet
 * - [HRXL WR Datasheet](https://github.com/EnviroDIY/ModularSensors/wiki/Sensor-Datasheets/Maxbotix-HRXL-MaxSonar-WR-Datasheet.pdf)
 * - [HRXL WRS Datasheet](https://github.com/EnviroDIY/ModularSensors/wiki/Sensor-Datasheets/Maxbotix-HRXL-MaxSonar-WRS-Datasheet.pdf)
 * - [MaxTemp Datasheet](https://github.com/EnviroDIY/ModularSensors/wiki/Sensor-Datasheets/Maxbotix-HR-MaxTemp-Datasheet.pdf)
 * - [Wiring Guide](https://github.com/EnviroDIY/ModularSensors/wiki/Sensor-Datasheets/Maxbotix-MaxSonar-MB7954-Datasheet-ConnectWire.pdf)
 *
 * @section maxbotics_sensor The MaxBotix MaxSonar %Sensor
 * @ctor_doc{MaxBotixSonar, Stream* stream, int8_t powerPin, int8_t triggerPin, uint8_t measurementsToAverage}
 * @subsection maxbotics_timing Sensor Timing
 * - Warm up time to completion of header:  160ms
 *
 * @section maxbotix_range Range Output
 * @variabledoc{MaxBotixSonar,Range}
 *   - Range is 300 to 5000mm or 500 to 9999mm, depending on model
 *   - Accuracy is ±1%
 *   - Result stored in sensorValues[0]
 *   - Resolution is 1mm
 *   - Reported as millimeters (mm)
 *   - Default variable code is SonarRange
 *
 * ___
 * @section maxbotics_examples Example Code
 * The MaxBotix MaxSonar is used in the @menulink{maxbotics} example.
 *
 * @menusnip{maxbotics}
 */
/* clang-format on */

// Header Guards
#ifndef SRC_SENSORS_MAXBOTIXSONAR_H_
#define SRC_SENSORS_MAXBOTIXSONAR_H_

// Debugging Statement
// #define MS_MAXBOTIXSONAR_DEBUG

#ifdef MS_MAXBOTIXSONAR_DEBUG
#define MS_DEBUGGING_STD "MaxBotixSonar"
#endif

// Included Dependencies
#include "ModSensorDebugger.h"
#undef MS_DEBUGGING_STD
#include "VariableBase.h"
#include "SensorBase.h"

// Sensor Specific Defines

/// Sensor::_numReturnedValues; the HRXL can report 1 value.
#define HRXL_NUM_VARIABLES 1
/// Sensor::_warmUpTime_ms; the HRXL warms up in 160ms.
#define HRXL_WARM_UP_TIME_MS 160
/// Sensor::_stabilizationTime_ms; the HRXL is stable after 0ms.
#define HRXL_STABILIZATION_TIME_MS 0
/// Sensor::_measurementTime_ms; the HRXL takes 166ms to complete a measurement.
#define HRXL_MEASUREMENT_TIME_MS 166
/// Decimals places in string representation; range should have 2.
#define HRXL_RESOLUTION 0
/// Variable number; range is stored in sensorValues[0].
#define HRXL_VAR_NUM 0

/* clang-format off */
/**
 * @brief The Sensor sub-class for the @ref maxbotics_page ultrasonic range finder.
 *
 * @ingroup maxbotics_group
 */
/* clang-format on */
class MaxBotixSonar : public Sensor {
 public:
    /**
     * @brief Construct a new MaxBotix Sonar object
     *
     * @param stream An Arduino data stream for TTL or RS232 communication.  See
     * [notes](https://github.com/EnviroDIY/ModularSensors/wiki/Arduino-Streams)
     * for more information on what streams can be used.
     * @param powerPin The pin on the mcu controlling power to the MaxSonar. Use
     * -1 if it is continuously powered.
     * - The MaxSonar requires a 2.7V - 5.5V power supply.
     * @param triggerPin The pin on the mcu controlling the "trigger" for the
     * MaxSonar.  Use -1 or omit for continuous ranging.
     * @param measurementsToAverage The number of measurements to take and
     * average before giving a "final" result from the sensor; optional with a
     * default value of 1.
     */
    MaxBotixSonar(Stream* stream, int8_t powerPin, int8_t triggerPin = -1,
                  uint8_t measurementsToAverage = 1);
    /**
     * @copydoc MaxBotixSonar::MaxBotixSonar
     */
    MaxBotixSonar(Stream& stream, int8_t powerPin, int8_t triggerPin = -1,
                  uint8_t measurementsToAverage = 1);
    /**
     * @brief Destroy the MaxBotix Sonar object
     */
    ~MaxBotixSonar();

    /**
     * @copydoc Sensor::getSensorLocation()
     */
    String getSensorLocation(void) override;

    /**
     * @brief Do any one-time preparations needed before the sensor will be able
     * to take readings.
     *
     * This sets pin mode on the trigger pin.  It also sets the expected stream
     * timeout for modbus and updates the #_sensorStatus.  No sensor power is
     * required.  This will always return true.
     *
     * @return **bool** True if the setup was successful.
     */
    bool setup(void) override;
    /**
     * @brief Wake the sensor up, if necessary.  Do whatever it takes to get a
     * sensor in the proper state to begin a measurement.
     *
     * Verifies that the power is on and updates the #_sensorStatus.  This also
     * sets the #_millisSensorActivated timestamp.
     *
     * For the MaxSonar, this also reads and dumps any returned "header" lines
     * from the sensor.
     *
     * @note This does NOT include any wait for sensor readiness.
     *
     * @return **bool** True if the wake function completed successfully.
     */
    bool wake(void) override;

    /**
     * @copydoc Sensor::addSingleMeasurementResult()
     */
    bool addSingleMeasurementResult(void) override;

 private:
    int8_t  _triggerPin;
    Stream* _stream;
};


/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [range output](@ref maxbotix_range) from a
 * [MaxBotix HRXL-MaxSonar ultrasonic range finder](@ref maxbotics_page).
 *
 * @ingroup maxbotics_group
 */
/* clang-format on */
class MaxBotixSonar_Range : public Variable {
 public:
    /**
     * @brief Construct a new MaxBotixSonar_Range object.
     *
     * @param parentSense The parent MaxBotixSonar providing the result
     * values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of SonarRange
     */
    explicit MaxBotixSonar_Range(MaxBotixSonar* parentSense,
                                 const char*    uuid    = "",
                                 const char*    varCode = "SonarRange")
        : Variable(parentSense, (const uint8_t)HRXL_VAR_NUM,
                   (uint8_t)HRXL_RESOLUTION, "distance", "millimeter", varCode,
                   uuid) {}
    /**
     * @brief Construct a new MaxBotixSonar_Range object.
     *
     * @note This must be tied with a parent MaxBotixSonar before it can be
     * used.
     */
    MaxBotixSonar_Range()
        : Variable((const uint8_t)HRXL_VAR_NUM, (uint8_t)HRXL_RESOLUTION,
                   "distance", "millimeter", "SonarRange") {}
    /**
     * @brief Destroy the MaxBotixSonar_Range object - no action needed.
     */
    ~MaxBotixSonar_Range() {}
};

#endif  // SRC_SENSORS_MAXBOTIXSONAR_H_
