/**
 * @file Distance.ino
 *
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/05 (initial version)
 * @date 2016/08/06 (last revision)
 * @version 1.0
 */


void setup()
{
    Seg7x4_init();
    US100_initSerial();
}


void loop()
{
    uint16_t len_mm;

    if (!US100_stepSerialDistance(&len_mm))
        return;

    if (US100_isValidDistance(len_mm))
        Seg7x4_step(len_mm);
    else
        Seg7x4_clear();
}

