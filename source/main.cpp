#include "MicroBit.h"

MicroBit uBit;
MicroBitSerial serial(USBTX, USBRX);

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    ManagedString message;
    while (true) {
        // Receive length of message as a string.
        // It is passed as a 3 digits int such as:
        // - 005
        // - 069
        // - 420
        ManagedString size = serial.read(3);
        uint8_t messageLength;
        if (size.length() == 3) {
            // Convert to int
            messageLength = atoi(size.toCharArray());
        }
        // Receive the actual message
        message = serial.read(messageLength);
        if (message.length() == messageLength) {
            // Print the message
            uBit.display.scroll(message);
            break;
        }
    }

    // Convert the message into usable data
    const char* messageArray = message.toCharArray();
    uint8_t iterationLength = strlen(messageArray) + 1;
    uint8_t data[5]; // Array that stores the message, in this case we know that the message is made of 5 unsigned integers
    uint8_t dataIndex = 0; // Counter for array item assignment
    int8_t tmpData = -1; // Temporary variable used to format multiple chars that form the same number
    for (int i = 0; i < iterationLength; i++) {
        if (messageArray[i] == '[' || messageArray[i] == ']' || messageArray[i] == ',') {
            if (tmpData > -1) {
                data[dataIndex] = tmpData;
                dataIndex++;
            }
            
            tmpData = -1;
        }
        else {
            if (tmpData < 0) {
                if (messageArray[i] != 0)
                    tmpData = messageArray[i] - '0'; // convert char to int and assign value to tmpData
                else
                    tmpData = 0;
            } else {
                /* Multiply by ten, reason:
                   Let's pick the number 1337, if split in a char array it
                   would look like ['1', '3', '3', '7']. So when iterating
                   through each char, multiply by 10 to make sure that for example
                   3 is not summed to 1 but instead 3 is summed to 10.
                   If you run the algorithm for the whole number you obtain
                   (((1 * 10) + 3) * 10) + 3) * 10) + 7 = 1337.
                */
                tmpData *= 10;
                if (messageArray[i] != 0)
                    tmpData += messageArray[i] - '0'; // convert char to int and sum to tmpData
                else
                    tmpData *= 10;
            }
        }
    }

    // Print the converted message to prove it worked
    for (int j = 0; j < sizeof(data); j++)
        uBit.display.scroll(ManagedString(data[j]));
    

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}