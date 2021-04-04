#ifndef SHIFT_REGISTER_HELPER_H
#define SHIFT_REGISTER_HELPER_H

#include "MicroBit.h"

#define DECREASE_LIVES_ID 1337
#define SHIFTREGISTER_BITS 8

extern int8_t lives;

extern MicroBitEvent decrease_lives_evt;

void shift_register_all_leds();
void decrease_lives(MicroBitEvent e);

#endif // SHIFT_REGISTER_HELPER_H