/**
 * @file DistanceUltrasonic.ino
 *  This program shows an ultrasonic measured distance on the 4 digit 7 segment
 *  LED display. This is for testing US-100: an ultrasonic distance sensor.
 *
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/05 (initial version)
 * @date 2016/08/31 (last revision)
 * @version 1.0
 */
//#define USE_PULSE_MODE


typedef enum {
    PIN_Echo = 2,   ///< Echo/Rx pin
    PIN_Trig = 3,   ///< Trig/Tx pin
} Pin;


void setup()
{
    uint8_t posPins[] = {
    //   0  1  2  3
        A2, 4, 5, 6
    };
    uint8_t segPins[] = {
    //  a  b  c   d   e   f  g   h
        9, 7, A0, 12, 11, 8, A1, 13
    };
    Digits_init(posPins, segPins);

    US100_init(PIN_Echo, PIN_Trig); // TX, RX

    initSerial();
}


void loop()
{
#ifdef USE_PULSE_MODE
    loop_pulse();
#else
    loop_serial();
#endif
}


void loop_pulse(void)
{
    enum {
        REPEAT_PERIOD = 2000 // in milliseconds
    };
    static unsigned long endMillis = 0;
    static uint16_t len_mm;

    if (millis() >= endMillis) {
        endMillis = millis() + REPEAT_PERIOD;

        if (!US100_measurePulseDistance(&len_mm) ||
            !US100_isValidDistance(len_mm)) {
            Serial.print(len_mm);
            Serial.println(" -- Out of Range");
            len_mm = 0; // lead to display clearing
            Digits_clear();
        }
        else {
            Serial.println(len_mm);
        }
    }

    if (len_mm != 0)
        Digits_step(len_mm);
}


void loop_serial(void)
{
    static uint16_t len_mm;

    if (US100_stepSerialDistance(&len_mm) && !US100_isValidDistance(len_mm)) {
        Digits_clear();
        return;
    }

    Digits_step(len_mm);
}


void initSerial(void)
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

