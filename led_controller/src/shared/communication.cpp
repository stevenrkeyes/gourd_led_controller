#include "communication.h"
#include "config.h"

void initCommunication() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Communication initialized");
}

void sendCommand(uint8_t command, const uint8_t* data, uint8_t length) {
    CommandPacket packet;
    packet.command = command;
    packet.data_length = length;
    
    // Copy data
    for (int i = 0; i < length && i < 32; i++) {
        packet.data[i] = data[i];
    }
    
    // Calculate checksum
    packet.checksum = calculateChecksum(packet);
    
    // Send packet
    Serial.write((uint8_t*)&packet, sizeof(CommandPacket));
    Serial.flush();
}

bool receiveCommand(CommandPacket& packet) {
    if (Serial.available() >= sizeof(CommandPacket)) {
        Serial.readBytes((uint8_t*)&packet, sizeof(CommandPacket));
        
        // Verify checksum
        uint8_t expectedChecksum = calculateChecksum(packet);
        if (packet.checksum == expectedChecksum) {
            return true;
        } else {
            Serial.println("Checksum error");
            Serial.println(packet.command);
            Serial.println(packet.data);
            Serial.println(packet.data_length);
            return false;
        }
    }
    return false;
}

void sendHeartbeat() {
    static unsigned long lastHeartbeat = 0;
    unsigned long now = millis();
    
    if (now - lastHeartbeat > 1000) { // Send heartbeat every second
        lastHeartbeat = now;
        
        uint8_t heartbeatData[4];
        heartbeatData[0] = (now >> 24) & 0xFF;
        heartbeatData[1] = (now >> 16) & 0xFF;
        heartbeatData[2] = (now >> 8) & 0xFF;
        heartbeatData[3] = now & 0xFF;
        
        sendCommand(CMD_HEARTBEAT, heartbeatData, 4);
    }
}

uint8_t calculateChecksum(const CommandPacket& packet) {
    uint8_t checksum = 0;
    checksum ^= packet.command;
    checksum ^= packet.data_length;
    
    for (int i = 0; i < packet.data_length; i++) {
        checksum ^= packet.data[i];
    }
    
    return checksum;
} 