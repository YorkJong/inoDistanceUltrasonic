/**
 * @file US100.ino
 *  US-100 ultrasonic distance sensor
 * @see https://sites.google.com/site/myscratchbooks/home/projects/project-09-
 * ultrasonic-sensor
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
    enum {
        TIMEOUT = 500   // milliseconds
    };

    typedef enum {
        S_Flush,
        S_Trig,
        S_Receive,
        S_Read
    } Stage;
    static uint8_t stage = S_Flush;
    static unsigned long endMillis;

    switch (stage) {
    case S_Flush:   // clear receive buffer of serial port
        Serial.flush();
        ++stage;
        break;
    case S_Trig:    // Trig US-100 begin to measure the distance
        Serial.write(0X55);
        endMillis = millis() + TIMEOUT;
        ++stage;
        break;
    case S_Receive: // Wait to receive 2 bytes
        if (Serial.available() >= 2)
           ++stage;
        if (millis() >= endMillis)
            stage = S_Flush;
        break;
    case S_Read:    // Read and calculate the result
        *len_mm = Serial.read();    // read high byte of distance
        *len_mm *= 256;
        *len_mm += Serial.read();   // Add low byte of distance

        stage = S_Flush;
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
    enum {
        TIMEOUT = 500   // milliseconds
    };

    typedef enum {
        S_Flush,
        S_Trig,
        S_Receive,
        S_Read
    } Stage;
    static uint8_t stage = S_Flush;
    static unsigned long endMillis;

    switch (stage) {
    case S_Flush:   // clear receive buffer of serial port
        Serial.flush();
        ++stage;
        break;
    case S_Trig:    // Trig US-100 begin to measure the temperature
        Serial.write(0X50);
        endMillis = millis() + TIMEOUT;
        ++stage;
        break;
    case S_Receive: // Wait to receive 1 byte
        if (Serial.available() >= 1)
           ++stage;
        if (millis() >= endMillis)
            stage = S_Flush;
        break;
    case S_Read:    // Read and calculate the result
        *deg = Serial.read() - 45;

        stage = S_Flush;
        return true;
    }

    return false;

    if (Serial.available() >= 1) {  // when receive 1 bytes
        int deg45 = Serial.read(); // Get the received byte (temperature)

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

