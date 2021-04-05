#include "shared_ubit.h"
#include "shift_register_helper.h"
#include "MicroBit.h"

#define LOW 0
#define HIGH 1

MicroBitPin SHIFREGISTER_CLOCK(MICROBIT_ID_IO_P14, MICROBIT_PIN_P14, PIN_CAPABILITY_DIGITAL_OUT); // Clock of the shift register
MicroBitPin SHIFTREGISTER_DATA(MICROBIT_ID_IO_P15, MICROBIT_PIN_P15, PIN_CAPABILITY_DIGITAL_OUT); // Clock serial data
int8_t lives = SHIFTREGISTER_BITS; // Number of lives / LEDs

void shift_register_all_leds() {
    /*
    This function lights up all the lives LEDs by interacting with
    the shift register.
    */
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
    /*
    This function is called whenever the decrease_lives_evt event
    is fired. It interacts with the shift register to decrease the number
    of turned on LEDs by one every time. The LED controlled by the
    last register is the first to turn off.
    - e: The MicroBitEvent passed to this function. Ignored.
    */
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