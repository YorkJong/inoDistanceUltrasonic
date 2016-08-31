/**
 * @file US100.ino
 *  US-100 ultrasonic distance sensor
 * @see https://sites.google.com/site/myscratchbooks/home/projects/
 *      project-09-ultrasonic-sensor
 *
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/06 (initial version)
 * @date 2016/08/31 (last revision)
 * @version 1.0
 */
#include <SoftwareSerial.h>


//-----------------------------------------------------------------------------
// US-100 in both modes
//-----------------------------------------------------------------------------

static uint8_t _echoPin;
static uint8_t _trigPin;
static SoftwareSerial *_serial;


/** Initializes the US-100 with Pulse Width mode.
 * @param echoPin assign the Echo/RX pin.
 * @param trigPin assign the Trig/TX pin.
 */
void US100_init(uint8_t echoPin, uint8_t trigPin)
{
    _echoPin = echoPin;
    _trigPin = trigPin;

    //pinMode(_echoPin, INPUT);    // set Echo pin input mode.
    //pinMode(_trigPin, OUTPUT);   // set Trig pin output mode.
    _serial = new SoftwareSerial(_echoPin, _trigPin);   // RX, TX
    _serial->begin(9600);
}


//-----------------------------------------------------------------------------
// US-100 in Serial Data mode (module has jumper cap on the back)
//-----------------------------------------------------------------------------

/** Steps/Updates distance measurement with Serial Data mode.
 * A measurement must call this function several times.
 * @param[out] len_mm the measured distance.
 * @retval true in the case of success to get the distance;
 * @retval fasle otherwise.
 * @note disconnect RX/Echo pin before upload the program.
 */
bool US100_stepSerialDistance(uint16_t *len_mm)
{
    enum {
        TIMEOUT = 500   // milliseconds
    };
    static uint8_t stage = 0;
    static unsigned long endMillis;

    switch (stage) {
    case 0:     // clear receive buffer of serial port
        //US100_initSerialIfNot();
        _serial->flush();
        ++stage;
        break;
    case 1:     // Trig US-100 begin to measure the distance
        _serial->write(0X55);
        endMillis = millis() + TIMEOUT;
        ++stage;
        break;

    case 2:     // Wait to receive 2 bytes
        if (_serial->available() >= 2)
           ++stage;

        if (millis() >= endMillis)
            stage = 0;
        break;
    case 3:     // Read and calculate the result
        // Millimeters = FirstByteRead * 256 + SecondByteRead
        *len_mm = _serial->read();    // Read high byte of distance
        *len_mm *= 256;
        *len_mm += _serial->read();   // Read and add low byte of distance

        stage = 0;
        return true;
    }

    return false;
}


/** Steps/Updates temperature measurement with Serial Data mode.
 * A measurement must call this function several times.
 * @param[out] deg the degree of the measured temperature.
 * @retval true in the case of success to get the temperature;
 * @retval fasle otherwise.
 * @note disconnect RX/Echo pin before upload the program.
 */
bool US100_stepSerialTemperature(int *deg)
{
    enum {
        TIMEOUT = 500   // milliseconds
    };
    static uint8_t stage = 0;
    static unsigned long endMillis;

    switch (stage) {
    case 0:     // clear receive buffer of serial port
        //US100_initSerialIfNot();
        _serial->flush();
        ++stage;
        break;
    case 1:     // Trig US-100 begin to measure the temperature
        _serial->write(0X50);
        endMillis = millis() + TIMEOUT;
        ++stage;
        break;

    case 2:     // Wait to receive 1 byte
        if (_serial->available() >= 1)
           ++stage;

        if (millis() >= endMillis)
            stage = 0;
        break;
    case 3:     // Read and calculate the result
        // Celsius = ByteRead - 45
        *deg = _serial->read() - 45;

        stage = 0;
        return true;
    }

    return false;
}


//-----------------------------------------------------------------------------
// US-100 in Pulse Width mode (removing the jumper cap on the back)
//-----------------------------------------------------------------------------

/** Measures a distance with Pulse Width mode.
 * @param[out] len_mm the measured distance.
 * @retval true in the case of success to get the distance;
 * @retval fasle otherwise.
 */
bool US100_measurePulseDistance(uint16_t *len_mm)
{
    unsigned long echo_us;

    // By the Trig / Pin sending pulses trigger US-100 ranging
    digitalWrite(_trigPin, HIGH);       // Send pulses begin by Trig / Pin
    delayMicroseconds(50);              // Set the pulse width of 50us (> 10us)
    digitalWrite(_trigPin, LOW);        // The end of the pulse

    // A pulse width calculating US-100 returned
    echo_us = pulseIn(_echoPin, HIGH);

    // pluseIn works on pulses from 10us to 180*1000000us in length.
    // Normal distance range: (1mm, 10000mm)
    // -> Pulse effective range: (10us, 60000us).
    if ((10 < echo_us) && (echo_us < 60000)) {
        // Millimeters = PulseWidth * 34 / 100 / 2
        *len_mm = echo_us * 34/100 / 2;

        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
// US-100 Utility Functions
//-----------------------------------------------------------------------------

/** Decides if a measureed distance is valid. */
bool US100_isValidDistance(uint16_t len_mm)
{
    // normal distance should between 1mm and 10000mm (1mm, 10m)
    enum {
        LEN_MIN = 20,
        LEN_MAX = 4500
    };  // length in mm

    if (len_mm < LEN_MIN)
        return false;
    if (len_mm > LEN_MAX)
        return false;

    return true;
}


/** Decides if a measureed temperature (in Celcius degree) is valid. */
bool US100_isValidTemperature(int deg)
{
    enum {
        DEG_MIN = -44,  // = 1 - 45
        DEG_MAX = 85    // = 130 - 45
    };  // Celcius temperature degree

    if (deg < DEG_MIN)
        return false;
    if (deg > DEG_MAX)
        return false;

    return true;
}


//-----------------------------------------------------------------------------

