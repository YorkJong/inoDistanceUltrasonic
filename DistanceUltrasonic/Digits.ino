/**
 * @file Digits.ino
 * Driver-without-delay of the 4 digit 7 segment LED display.
 * @see http://yehnan.blogspot.tw/2013/08/arduino_26.html
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2016/08/06 (initial version)
 * @date 2016/08/10 (last revision)
 * @version 1.0
 */
#include <assert.h>

typedef enum {
    PIN_3 = 6,
    PIN_a = 9,
    PIN_f = 8,
    PIN_2 = 5,
    PIN_1 = 4,
    PIN_b = 7,

    PIN_e = 11,
    PIN_d = 12,
    PIN_h = 13,
    PIN_c = A0,
    PIN_g = A1,
    PIN_0 = A2,
} Seg7X4Pin;


#define POS_NUM 4
#define SEG_NUM 8

/** The position table. */
static const uint8_t _posPins[POS_NUM] = {
    PIN_0, PIN_1, PIN_2, PIN_3
};

/** The 8-segment table. */
static const uint8_t _segPins[SEG_NUM] = {
    PIN_a, PIN_b, PIN_c, PIN_d, PIN_e, PIN_f, PIN_g, PIN_h
};


/** The digit to display table. */
static const uint8_t data[10][SEG_NUM] = {
//   a, b, c, d, e, f, g, h
    {1, 1, 1, 1, 1, 1, 0, 0},   // 0
    {0, 1, 1, 0, 0, 0, 0, 0},   // 1
    {1, 1, 0, 1, 1, 0, 1, 0},   // 2
    {1, 1, 1, 1, 0, 0, 1, 0},   // 3
    {0, 1, 1, 0, 0, 1, 1, 0},   // 4
    {1, 0, 1, 1, 0, 1, 1, 0},   // 5
    {1, 0, 1, 1, 1, 1, 1, 0},   // 6
    {1, 1, 1, 0, 0, 0, 0, 0},   // 7
    {1, 1, 1, 1, 1, 1, 1, 0},   // 8
    {1, 1, 1, 1, 0, 1, 1, 0},   // 9
};


/** Initializes the display relative pins as output mode. */
static void Digits_initPin(void)
{
    for(int i = 0; i < POS_NUM; i++)
        pinMode(_posPins[i], OUTPUT);

    for(int i = 0; i < SEG_NUM; i++)
        pinMode(_segPins[i], OUTPUT);
}


/** Clears the display. */
void Digits_clear(void)
{
    for(int i = 0; i < POS_NUM; i++)
        digitalWrite(_posPins[i], HIGH);

    for(int i = 0; i < SEG_NUM; i++)
        digitalWrite(_segPins[i], LOW);
}


/** Initializes the display. */
void Digits_init(void)
{
    Digits_initPin();
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

    for (int p = 0; p < POS_NUM; p++) {
        if (p == pos)
            digitalWrite(_posPins[p], LOW);
        else
            digitalWrite(_posPins[p], HIGH);
    }

    for (int i = 0; i < SEG_NUM; i++)
        digitalWrite(_segPins[i], data[digit][i] == 1 ? HIGH : LOW);
}

