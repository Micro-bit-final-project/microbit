#include "serial_helper.h"

#include "MicroBit.h"

bool send(int info[], size_t iterationSize, bool wait) {
  /*
  This function sends an int array through serial
  after converting it to a ManagedString. It then
  waits for further instruction from the computer
  if the parameter wait is true.
  If an "R" is received then the Micro:bit will
  call the receiveMessage() function and wait
  for data to come in the buffer. If an "Y" is received
  the function returns false, and receiveMessage()
  is not called.
  - info[]: The int array to send.
  - iterationSize: Iterations to convert info into a ManageString.
  - wait: Whether or not to wait for a response.
  */
  while (true) {
    uBit.sleep(100);  // Try to stay in sync with the computer
    ManagedString infoToSend("[");
    for (int k = 0; k < iterationSize; k++) {
      if (k != (iterationSize - 1))
        infoToSend = infoToSend + ManagedString(info[k]) + ",";
      else
        infoToSend = infoToSend + ManagedString(info[k]) + "]";
    }
    serial.send(infoToSend);

    if (wait) {
      //ManagedString mContinue = serial.read(1, ASYNC);
      ManagedString mContinue = serial.read(1, SYNC_SPINWAIT);
      if (mContinue == "Y") {
        return false;
      } else if (mContinue == "R") {
        lives_decrease_evt.fire();
        return false;
      }
    } else {
      return false;
    }
  }
}

ManagedString receiveMessage() {
  /*
  This function listen for data coming to the serial buffer
  and provides confirmation if the data is received.
  Returns:
  - message: A ManagedString containing the received message
  */
  serial.clearRxBuffer();  // Clear the buffer in case it's not empty already

  serial.send("Y");  // Let them know we are ready to receive

  ManagedString message;
  // Receive length of message as a string.
  // It is passed as a 3 digits int such as:
  // - 005
  // - 069
  // - 420
  ManagedString size = serial.read(3);
  uint8_t messageLength = 0;
  if (size.length() == 3) {
    // Convert to int
    messageLength = atoi(size.toCharArray());
  }

  serial.send("Y");  // Let them know we are ready to receive

  // Receive the actual message
  while (true) {
    // Wait for the message to come
    uBit.sleep(100);  // Try to stay in sync with the computer
    message = serial.read(messageLength, ASYNC);
    if (message.length() == messageLength) {
      break;
    }
  }

  return message;
}

void parseMessage(ManagedString message) {
  /*
  This function converts the received message from a
  ManagedString to an uint8_t array.
  */
  const char *messageArray = message.toCharArray();
  uint8_t iterationLength = strlen(messageArray) + 1;
  dataIndex = 0;
  tmpData = -1;
  for (int i = 0; i < iterationLength; i++) {
    if (messageArray[i] == '[' || messageArray[i] == ']' ||
        messageArray[i] == ',') {
      if (tmpData > -1) {
        data[dataIndex] = tmpData;
        dataIndex++;
      }

      tmpData = -1;
    } else {
      if (tmpData < 0) {
        if (messageArray[i] != 0)
          tmpData = messageArray[i] -
                    '0';  // convert char to int and assign value to tmpData
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
          tmpData +=
              messageArray[i] - '0';  // convert char to int and sum to tmpData
        else
          tmpData *= 10;
      }
    }
  }
}
