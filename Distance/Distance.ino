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

//----------------------------------------------------------------------------
// 7 segments, 4 digits LED display
// ref. http://yehnan.blogspot.tw/2013/08/arduino_26.html
//----------------------------------------------------------------------------

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
static const byte _posPins[POS_NUM] = {
    PIN_0, PIN_1, PIN_2, PIN_3
};

/** The 8-segment table. */
static const byte _segPins[SEG_NUM] = {
    PIN_a, PIN_b, PIN_c, PIN_d, PIN_e, PIN_f, PIN_g, PIN_h
};


static word _7X4Val;    ///< current value to display

/** The digit to display table. */
static const byte data[10][SEG_NUM] = {
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


/** Initializes the LED relative pins as output mode. */
void Seg7x4_initPin(void)
{
    for(int i = 0; i < POS_NUM; i++)
        pinMode(_posPins[i], OUTPUT);

    for(int i = 0; i < SEG_NUM; i++)
        pinMode(_segPins[i], OUTPUT);
}


/** Clears the LEDs. */
void Seg7x4_clear(void)
{
    for(int i = 0; i < POS_NUM; i++)
        digitalWrite(_posPins[i], HIGH);

    for(int i = 0; i < SEG_NUM; i++)
        digitalWrite(_segPins[i], LOW);
}


/** Initializes the LEDs. */
void Seg7x4_init(void)
{
    Seg7x4_initPin();
    Seg7x4_clear();
}


/** Sets a 4-digit value to show. */
void Seg7x4_setValue(uint16_t val)
{
    assert (val <= 9999);

    _7X4Val = val;
}


/** Step function of the 7-segment, 4-digit LED display. */
void Seg7x4_step(void)
{
    enum {
        INTERVAL = 5    // milli-seconds
    };
    static word num_bak = 9999;
    static word remainder, divisor, pos;
    unsigned long currMillis;
    static unsigned long endMillis = 0;

    if ((num_bak != _7X4Val) || (pos == 0)) {
        remainder = num_bak = _7X4Val;
        divisor = 1000;
        pos = 4;
    }

    currMillis = millis();
    if (currMillis >= endMillis) {
        endMillis = currMillis + INTERVAL;

        --pos;
        Seg7x4_setDigit(pos, remainder / divisor);
        remainder %= divisor;
        divisor /= 10;
    }
}


/** Shows a digit at given position.
 * @param pos the position (0..3)
 * @param digit the digit (0..9)
 */
static void Seg7x4_setDigit(int pos, int digit)
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


//----------------------------------------------------------------------------
