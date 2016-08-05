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
    enum {
        BAUD_RATE = 9600,
    };

    Seg7x4_init();
    US100_init();

    // Init serial
    Serial.begin(BAUD_RATE);

    Serial.print("Initialized Serial Port at ");
    Serial.print(BAUD_RATE);
    Serial.println(" baud rate.");

    Seg7x4_setValue(8888);
}


void loop()
{
    Seg7x4_step();
    US100_step();
}

//----------------------------------------------------------------------------
// US-100 ultra-sonic distance sensor
//----------------------------------------------------------------------------

void US100_init(void)
{
}


void US100_step(void)
{
}

