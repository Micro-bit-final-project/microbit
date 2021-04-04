#include "shared_ubit.h"
#include "shift_register_helper.h"
#include "MicroBit.h"

#define LOW 0
#define HIGH 1

MicroBitPin SHIFREGISTER_CLOCK(MICROBIT_ID_IO_P14, MICROBIT_PIN_P14, PIN_CAPABILITY_DIGITAL_OUT); // Clock of the shift register
MicroBitPin SHIFTREGISTER_DATA(MICROBIT_ID_IO_P15, MICROBIT_PIN_P15, PIN_CAPABILITY_DIGITAL_OUT); // Clock serial data
int8_t lives = SHIFTREGISTER_BITS; // Number of lives / LEDs

void shift_register_all_leds() {
    // Light up all the LEDs
    SHIFTREGISTER_DATA.setDigitalValue(HIGH);
    SHIFREGISTER_CLOCK.setDigitalValue(LOW);
    for (int i = 0; i < SHIFTREGISTER_BITS; i++) {
        SHIFREGISTER_CLOCK.setDigitalValue(HIGH);
        SHIFREGISTER_CLOCK.setDigitalValue(LOW);
    }
    SHIFREGISTER_CLOCK.setDigitalValue(HIGH);
    SHIFREGISTER_CLOCK.setDigitalValue(LOW);
}

void decrease_lives(MicroBitEvent e) {

    SHIFTREGISTER_DATA.setDigitalValue(LOW);
    SHIFREGISTER_CLOCK.setDigitalValue(LOW);
    for (int i = 0; i < SHIFTREGISTER_BITS; i++) {
        if ((SHIFTREGISTER_BITS - lives) == i) {
            SHIFTREGISTER_DATA.setDigitalValue(HIGH);
        }
        SHIFREGISTER_CLOCK.setDigitalValue(HIGH);
        SHIFREGISTER_CLOCK.setDigitalValue(LOW);
    }
    SHIFREGISTER_CLOCK.setDigitalValue(HIGH);
    SHIFREGISTER_CLOCK.setDigitalValue(LOW);
}