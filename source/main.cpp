#include "MicroBit.h"
#include "serial_helper.h"

MicroBit uBit;
MicroBitSerial serial(USBTX, USBRX);

bool receive = false;  // Whether or not to receive data from the computer

uint8_t data[5];  // Array that stores the message, in this case we know that
                  // the message is made of 5 unsigned integers
uint8_t dataIndex = 0;  // Counter for array item assignment
int8_t tmpData = -1;    // Temporary variable used to format multiple chars that
                        // form the same number

int main() {
  // Initialise the micro:bit runtime.
  uBit.init();

  while (true) {
    serial.clearTxBuffer();
    uBit.accelerometer.requestUpdate(); // Request an accelerometer update to register a gesture.
    int info[] = {uBit.accelerometer.getGesture(), 0}; // 0 needed for data parsing on PC side.
    send(info, (sizeof(info) / sizeof(info[0])), false);
  }

  // If main exits, there may still be other fibers running or registered event
  // handlers etc. Simply release this fiber, which will mean we enter the
  // scheduler. Worse case, we then sit in the idle task forever, in a power
  // efficient sleep.
  release_fiber();
}