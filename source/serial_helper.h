#ifndef SERIAL_HELPER_H
#define SERIAL_HELPER_H
#include "MicroBit.h"

extern MicroBit uBit;
extern MicroBitSerial serial;

extern bool receive;

extern uint8_t data[5];
extern uint8_t dataIndex;
extern int8_t tmpData;

bool sendAndWait(int info[], size_t iterationSize);
ManagedString receiveMessage();
void parseMessage(ManagedString message);

#endif // SERIAL_HELPER_H