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
    int info[] = {uBit.accelerometer.getX(), uBit.accelerometer.getY(),
                  uBit.accelerometer.getZ()};
    receive = sendAndWait(info, (sizeof(info) / sizeof(info[0])));

    if (receive) {
      ManagedString message = receiveMessage();
      parseMessage(message);

      // Print the converted message to prove it worked
      ManagedString messageConfirmation("[");
      for (int j = 0; j < 5; j++) {
        if (j != 4)
          messageConfirmation =
              messageConfirmation + ManagedString(data[j]) + ",";
        else
          messageConfirmation =
              messageConfirmation + ManagedString(data[j]) + "]";
      }
      serial.send(messageConfirmation);
      uBit.sleep(100);

      serial.send("Y");  // Let them know we are ready to continue

      receive = false;
    }
  }

  // If main exits, there may still be other fibers running or registered event
  // handlers etc. Simply release this fiber, which will mean we enter the
  // scheduler. Worse case, we then sit in the idle task forever, in a power
  // efficient sleep.
  release_fiber();
}