#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

struct CommandPacket {
    uint8_t command;
    uint8_t data_length;
    uint8_t data[32];
    uint8_t checksum;
};

void initCommunication();
void sendCommand(uint8_t command, const uint8_t* data, uint8_t length);
bool receiveCommand(CommandPacket& packet);
void sendHeartbeat();
uint8_t calculateChecksum(const CommandPacket& packet);

#endif // COMMUNICATION_H 