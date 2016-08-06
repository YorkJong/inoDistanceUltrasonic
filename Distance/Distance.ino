/**
 * @file Distance.ino
 *
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/05 (initial version)
 * @date 2016/08/06 (last revision)
 * @version 1.0
 */
#include <assert.h>


void setup()
{

    Seg7x4_init();
    US100_initSerial();
}


void loop()
{
    Seg7x4_step(8888);
    US100_stepSerial();
}

