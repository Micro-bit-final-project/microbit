#include "MicroBit.h"
#include "serial_helper.h"

#define LOW 0

MicroBit uBit;
MicroBitSerial serial(USBTX, USBRX);

MicroBitPin UP(MICROBIT_ID_IO_P3, MICROBIT_PIN_P3, PIN_CAPABILITY_DIGITAL_IN);
MicroBitPin LEFT(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_DIGITAL_IN);
MicroBitPin DOWN(MICROBIT_ID_IO_P4, MICROBIT_PIN_P4, PIN_CAPABILITY_DIGITAL_IN);
MicroBitPin RIGHT(MICROBIT_ID_IO_P6, MICROBIT_PIN_P6, PIN_CAPABILITY_DIGITAL_IN);


bool receive = false;  // Whether or not to receive data from the computer

uint_fast8_t data[5];  // Array that stores the message, in this case we know that
                  // the message is made of 5 unsigned integers
uint_fast8_t dataIndex = 0;  // Counter for array item assignment
int_fast8_t tmpData = -1;    // Temporary variable used to format multiple chars that
                             // form the same number

int main() {
  // Initialise the micro:bit runtime.
  uBit.init();

  while (true) {
    serial.clearTxBuffer();
    uBit.accelerometer.requestUpdate(); // Request an accelerometer update to register a gesture.

    uint_fast8_t up = UP.getDigitalValue();
    uint_fast8_t left = LEFT.getDigitalValue();
    uint_fast8_t down = DOWN.getDigitalValue();
    uint_fast8_t right = RIGHT.getDigitalValue();
    uint_fast8_t buttonPressed = 0;
    if (up == LOW)
      buttonPressed = 1;
    else if (left == LOW)
      buttonPressed = 2;
    else if (down == LOW)
      buttonPressed = 3;
    else if (right == LOW)
      buttonPressed = 4;

    int info[] = {buttonPressed, uBit.accelerometer.getGesture()};
    send(info, (sizeof(info) / sizeof(info[0])), false);
  }

  // If main exits, there may still be other fibers running or registered event
  // handlers etc. Simply release this fiber, which will mean we enter the
  // scheduler. Worse case, we then sit in the idle task forever, in a power
  // efficient sleep.
  release_fiber();
}