/**
 * @file Distance.ino
 *  This program show an ultrasonic measured distance on the 4 digital 7
 *  segment LED display.
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/05 (initial version)
 * @date 2016/08/10 (last revision)
 * @version 1.0
 */

#define USE_PULSE_MODE

void setup()
{
    Seg7x4_init();
#ifdef USE_PULSE_MODE
    US100_initPulse(1, 0);  // init Trig/Tx, Echo/Rx pin
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
        Seg7x4_clear();
        while (millis() < endMillis)
            ;
        return;
    }

    unsigned long endMillis = millis() + 1000;
    while (millis() < endMillis)
        Seg7x4_step(len_mm);
}


void loop_serial(void)
{
    uint16_t len_mm;

    if (US100_stepSerialDistance(&len_mm) && !US100_isValidDistance(len_mm)) {
        Seg7x4_clear();
        return;
    }

    Seg7x4_step(len_mm);
}
