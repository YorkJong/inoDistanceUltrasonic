/**
 * @file US100.ino
 *  US-100 ultra-sonic distance sensor
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/06 (initial version)
 * @date 2016/08/06 (last revision)
 * @version 1.0
 */

/** Initializes with the serial mode. */
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

//-----------------------------------------------------------------------------

/** Steps distance measurement the serial mode.
 * A measurement must call this function several times.
 * @param[out] len_mm the measured distance.
 * @retval true true in the case of success to get the distance;
 * @retval fasle otherwise.
 */
bool US100_stepSerialDistance(uint16_t *len_mm)
{
    Serial.flush();                 // clear receive buffer of serial port
    Serial.write(0X55);             // begin to measure the distance
    delay(500);                     // delay 500ms to wait result

    if (Serial.available() >= 2) {      // when receive 2 bytes
        uint8_t hi = Serial.read();     // High byte of distance
        uint8_t lo = Serial.read();     // Low byte of distance
        *len_mm  = hi*256 + lo; // Calculate the distance

        return true;
    }

    return false;
}


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

//-----------------------------------------------------------------------------

/** Steps temperature measurement with serial mode.
 * A measurement must call this function several times.
 * @param[out] deg the degree of the measured temperature.
 * @retval true true in the case of success to get the temperature;
 * @retval fasle otherwise.
 */
bool US100_stepSerialTemperature(int *deg)
{
    Serial.flush();                 // clear receive buffer of serial port
    Serial.write(0X50);             // begin to measure the temperature
    delay(500);                     // delay 500ms to wait result

    if (Serial.available() >= 1) {  // when receive 1 bytes
        int deg45 = Serial.read(); // Get the received byte (temperature)
        *deg = deg45 - 45;

        return true;
    }

    return false;
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

