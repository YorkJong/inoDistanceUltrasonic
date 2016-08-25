/**
 * @file Digits.ino
 * Driver-without-delay of the 4 digit 7 segment LED display.
 * @see http://yehnan.blogspot.tw/2013/08/arduino_26.html
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/06 (initial version)
 * @date 2016/08/25 (last revision)
 * @version 1.0
 */
#include <assert.h>

//-----------------------------------------------------------------------------
// 4 digit display
//-----------------------------------------------------------------------------

/** Clears the display. */
void Digits_clear(void)
{
    SegLed_setPositions(0x00);
    SegLed_setSegments(0x00);
}


/** Initializes the display.
 * @param posPins the list of position pins (0..3)
 * @param segPins the list of segment pins (a..h)
 */
void Digits_init(const uint8_t posPins[], const uint8_t segPins[])
{
    SegLed_init(posPins, segPins);
    Digits_clear();
}


/** Steps the 4 digit 7 segment display. This function show at most one digit
 * at once and show 4 digits in turn. Each show is with a 5ms interval. The
 * interval is controlled witout calling delay function.
 * @param number a 4-digit number to show
 */
void Digits_step(uint16_t number)
{
    enum {
        INTERVAL = 5    // milli-seconds
    };
    static uint16_t num_bak = 9999;
    static uint16_t remainder, divisor, pos;
    unsigned long currMillis;
    static unsigned long endMillis = 0;

    if (number > 9999) {
        Digits_clear();
        return;
    }

    if ((num_bak != number) || (pos == 0)) {
        remainder = num_bak = number;
        divisor = 1000;
        pos = 4;
    }

    currMillis = millis();
    if (currMillis >= endMillis) {
        endMillis = currMillis + INTERVAL;

        --pos;
        Digits_showDigit(pos, remainder / divisor);
        remainder %= divisor;
        divisor /= 10;
    }
}


/** Shows a digit at a given position.
 * @param pos the position (0..3)
 * @param digit the digit (0..9)
 */
static void Digits_showDigit(int pos, int digit)
{
    assert ((0 <= pos) && (pos <= 3));
    assert ((0 <= digit) && (digit <=9));

    // Layout of LED segments of a digit:
    //       a
    //       -
    //     f| |b
    //       - g
    //     e| |c
    //       -   .dp(h)
    //       d
    static const uint8_t digit2seg[] = {
        //            hgfe dcba
        0x3F,   // 0: 0011 1111
        0x06,   // 1: 0000 0110
        0x5B,   // 2: 0101 1011
        0x4F,   // 3: 0100 1111
        0x66,   // 4: 0110 0110
        0x6D,   // 5: 0110 1101
        0x7D,   // 6: 0111 1101
        0x07,   // 7: 0000 0111
        0x7F,   // 8: 0111 1111
        0x6F,   // 9: 0110 1111
    };

    SegLed_setPositions(1 << pos);
    SegLed_setSegments(digit2seg[digit]);
}


//-----------------------------------------------------------------------------
// 4-digit segment LEDs: a display of common-cathode LEDs
//-----------------------------------------------------------------------------

enum {
    SEG_NUM = 8,
    POS_NUM = 4
};

static uint8_t _segPins[SEG_NUM];
static uint8_t _posPins[POS_NUM];

/** Initializes the 4-digit segment LEDs.
 * @param posPins the list of position pins (0..3)
 * @param segPins the list of segment pins (a..h)
 */
static void SegLed_init(const uint8_t posPins[], const uint8_t segPins[])
{
    for (int i=0; i<POS_NUM; ++i)
        _posPins[i] = posPins[i];
    for (int i=0; i<SEG_NUM; ++i)
        _segPins[i] = segPins[i];

    for(int i=0; i<POS_NUM; ++i)
        pinMode(_posPins[i], OUTPUT);
    for(int i=0; i<SEG_NUM; ++i)
        pinMode(_segPins[i], OUTPUT);
}


/** Sets positions of common-cathode LEDs
 * @param pos the bitmap of positions
 */
static void SegLed_setPositions(uint8_t bitmap)
{
    for (int i=0; i<POS_NUM; ++i) {
        if (bitmap & 0x01)
            digitalWrite(_posPins[i], LOW);     // common-cathode
        else
            digitalWrite(_posPins[i], HIGH);
        bitmap >>= 1;
    }
}


/** Sets segments of common-cathode LEDs
 * @param bitmap the bitmap of the 8 segments.
 */
static void SegLed_setSegments(uint8_t bitmap)
{
    for (int i=0; i<SEG_NUM; ++i) {
        if (bitmap & 0x01)
            digitalWrite(_segPins[i], HIGH);
        else
            digitalWrite(_segPins[i], LOW);
        bitmap >>= 1;
    }
}

//-----------------------------------------------------------------------------

