/**
 * @file US100.ino
 *  US-100 ultra-sonic distance sensor
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/06 (initial version)
 * @date 2016/08/06 (last revision)
 * @version 1.0
 */

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


void US100_stepSerial(void)
{
}


