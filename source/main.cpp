#include "MicroBit.h"

MicroBit uBit;
MicroBitSerial serial(USBTX, USBRX);

bool receive = false; // Whether or not to receive data from the computer

uint8_t data[5]; // Array that stores the message, in this case we know that the message is made of 5 unsigned integers
uint8_t dataIndex = 0; // Counter for array item assignment
int8_t tmpData = -1; // Temporary variable used to format multiple chars that form the same number

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    while (true) {
        while (true) {
            int info[] = {uBit.accelerometer.getX(), uBit.accelerometer.getY(), uBit.accelerometer.getZ()};
            uBit.sleep(100); // Try to stay in sync with the computer
            ManagedString infoToSend("[");
            for (int k = 0; k < 3; k++) {
                if (k != 2)
                    infoToSend = infoToSend + ManagedString(info[k]) + ",";
                else
                    infoToSend = infoToSend + ManagedString(info[k]) + "]";
            }
            serial.send(infoToSend);
        
            ManagedString mContinue = serial.read(1, ASYNC);
            if (mContinue == "Y") {
                break;
            } else if (mContinue == "R") {
                receive = true;
                break;
            }
            
        }

        if (receive) {
            serial.clearRxBuffer(); // Clear the buffer in case it's not empty already

            serial.send("Y"); // Let them know we are ready to receive

            ManagedString message;
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

            serial.send("Y"); // Let them know we are ready to receive

            // Receive the actual message
            while (true) {
                // Wait for the message to come
                uBit.sleep(100); // Try to stay in sync with the computer
                message = serial.read(messageLength, ASYNC);
                if (message.length() == messageLength) {
                    break;
                }
            }

            // Convert the message into usable data
            const char* messageArray = message.toCharArray();
            uint8_t iterationLength = strlen(messageArray) + 1;
            dataIndex = 0;
            tmpData = -1;
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
            ManagedString messageConfirmation("[");
            for (int j = 0; j < 5; j++) {
                if (j != 4)
                    messageConfirmation = messageConfirmation + ManagedString(data[j]) + ",";
                else
                    messageConfirmation = messageConfirmation + ManagedString(data[j]) + "]";
            }
            serial.send(messageConfirmation);
            uBit.sleep(100);
            
            serial.send("Y"); // Let them know we are ready to continue

            receive = false;
        }
    }
    
    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}