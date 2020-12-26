#ifndef SERIAL_HELPER_H
#define SERIAL_HELPER_H
#include "MicroBit.h"

extern MicroBit uBit;
extern MicroBitSerial serial;

extern bool receive;

extern uint_fast8_t data[5];
extern uint_fast8_t dataIndex;
extern int_fast8_t tmpData;

bool send(int info[], size_t iterationSize, bool wait);
ManagedString receiveMessage();
void parseMessage(ManagedString message);

#endif // SERIAL_HELPER_H