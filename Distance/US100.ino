/**
 * @file US100.ino
 *  US-100 ultrasonic distance sensor
 * @see https://sites.google.com/site/myscratchbooks/home/projects/project-09-
 * ultrasonic-sensor
 *
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/06 (initial version)
 * @date 2016/08/06 (last revision)
 * @version 1.0
 */

//-----------------------------------------------------------------------------
// US-100 in Serial Data mode (module has jumper cap on the back)
//-----------------------------------------------------------------------------

/** Initializes US-100 module with Serial Data mode. */
void US100_initSerial(void)
{
    enum {
        BAUD_RATE = 9600,
    };

    // Init serial
    Serial.begin(BAUD_RATE);

    Serial.print("Initialized Serial Port at ");
    Serial.print(BAUD_RATE);
    Serial.println(" baud rate.");
}


/** Steps distance measurement with Serial Data mode.
 * A measurement must call this function several times.
 * @param[out] len_mm the measured distance.
 * @retval true true in the case of success to get the distance;
 * @retval fasle otherwise.
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
        Serial.flush();
        ++stage;
        break;
    case 1:     // Trig US-100 begin to measure the distance
        Serial.write(0X55);
        endMillis = millis() + TIMEOUT;
        ++stage;
        break;

    case 2:     // Wait to receive 2 bytes
        if (Serial.available() >= 2)
           ++stage;

        if (millis() >= endMillis)
            stage = 0;
        break;
    case 3:     // Read and calculate the result
        *len_mm = Serial.read();    // Read high byte of distance
        *len_mm *= 256;
        *len_mm += Serial.read();   // Read and add low byte of distance

        stage = 0;
        return true;
    }

    return false;
}


/** Steps temperature measurement with Serial Data mode.
 * A measurement must call this function several times.
 * @param[out] deg the degree of the measured temperature.
 * @retval true true in the case of success to get the temperature;
 * @retval fasle otherwise.
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
        Serial.flush();
        ++stage;
        break;
    case 1:     // Trig US-100 begin to measure the temperature
        Serial.write(0X50);
        endMillis = millis() + TIMEOUT;
        ++stage;
        break;

    case 2:     // Wait to receive 1 byte
        if (Serial.available() >= 1)
           ++stage;

        if (millis() >= endMillis)
            stage = 0;
        break;
    case 3:     // Read and calculate the result
        *deg = Serial.read() - 45;

        stage = 0;
        return true;
    }

    return false;
}


//-----------------------------------------------------------------------------
// US-100 in Plus Width mode (removing the jumper cap on the back)
//-----------------------------------------------------------------------------

static uint8_t _trigPin;
static uint8_t _echoPin;

/** Initializes the US-100 with Plus Width mode.
 * @param trigPin assign the Trig pin.
 * @param echoPin assign the Echo pin.
 */
void US100_initPlus(uint8_t trigPin, uint8_t echoPin)
{
    _trigPin = trigPin;
    _echoPin = echoPin;

    pinMode(_trigPin, OUTPUT);   // set Trig pin output mode.
    pinMode(_echoPin, INPUT);    // set Echo pin input mode.
}


/** Measure a distance with Plus Width mode.
 * @param[out] len_mm the measured distance.
 * @retval true true in the case of success to get the distance;
 * @retval fasle otherwise.
 */
bool US100_measurePlusDistance(uint16_t *len_mm)
{
    unsigned long echo_us;

    // By the Trig / Pin sending pulses trigger US-100 ranging
    digitalWrite(_trigPin, HIGH);       // Send pulses begin by Trig / Pin
    delayMicroseconds(50);              // Set the pulse width of 50us (> 10us)
    digitalWrite(_trigPin, LOW);        // The end of the pulse
    echo_us = pulseIn(_echoPin, HIGH);  // A pulse width calculating US-100 returned

    if ((echo_us < 60000) && (echo_us > 1)) {   // Pulse effective range (1, 60000).
        // len_mm = (echo_us * 0.34mm/us) / 2 (mm)
        *len_mm = (echo_us*34/100) / 2;         // Calculating the distance by a pulse width.

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

