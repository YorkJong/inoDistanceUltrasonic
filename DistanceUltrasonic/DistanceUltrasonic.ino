/**
 * @file DistanceUltrasonic.ino
 *  This program shows an ultrasonic measured distance on the 4 digit 7 segment
 *  LED display. This is for testing US-100: an ultrasonic distance sensor.
 *
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/05 (initial version)
 * @date 2016/08/19 (last revision)
 * @version 1.0
 */
#define USE_PULSE_MODE

#ifdef USE_PULSE_MODE
typedef enum {
    PIN_Trig = 1,   ///< Trig/Tx pin
    PIN_Echo = 0    ///< Echo/Rx pin
} Pin;
#endif

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

#ifdef USE_PULSE_MODE
    US100_initPulse(PIN_Trig, PIN_Echo);
#else
    US100_initSerial();
#endif
}


void loop()
{
    uint16_t len_mm;

#ifdef USE_PULSE_MODE
    loop_pulse();
#else
    loop_serial();
#endif
}


void loop_pulse(void)
{
    uint16_t len_mm;

    if (!US100_measurePulseDistance(&len_mm) ||
        !US100_isValidDistance(len_mm)) {
        unsigned long endMillis = millis() + 1000;
        Digits_clear();
        while (millis() < endMillis)
            ;
        return;
    }

    unsigned long endMillis = millis() + 1000;
    while (millis() < endMillis)
        Digits_step(len_mm);
}


void loop_serial(void)
{
    uint16_t len_mm;

    if (US100_stepSerialDistance(&len_mm) && !US100_isValidDistance(len_mm)) {
        Digits_clear();
        return;
    }

    Digits_step(len_mm);
}
