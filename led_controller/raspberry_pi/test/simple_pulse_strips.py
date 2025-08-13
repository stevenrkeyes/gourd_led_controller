#!/usr/bin/env python3
"""
Simple automatic LED strip pulse test
Just pulses each strip 0-7 in sequence, no user input required
"""

import serial
import serial.tools.list_ports
import time
import struct

# Communication protocol constants
CMD_LED_PULSE = 0x01

# Teensy B serial number
TEENSY_B_SERIAL = "4278530"

class CommandPacket:
    """Binary command packet for Teensy B"""
    def __init__(self, command=0, data_length=0, data=None):
        self.command = command
        self.data_length = data_length
        self.data = data if data is not None else [0] * 32
    
    def calculate_checksum(self):
        """Calculate XOR checksum"""
        checksum = 0
        checksum ^= self.command
        checksum ^= self.data_length
        for i in range(self.data_length):
            checksum ^= self.data[i]
        return checksum
    
    def to_bytes(self):
        """Convert packet to bytes for transmission"""
        checksum = self.calculate_checksum()
        return struct.pack('BB32sB', self.command, self.data_length, bytes(self.data), checksum)

def find_teensy():
    """Find Teensy B device"""
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if port.serial_number == TEENSY_B_SERIAL:
            return port.device
    return None

def main():
    # Connect to Teensy B
    teensy_port = find_teensy()
    if not teensy_port:
        print(f"❌ Could not find Teensy B with serial {TEENSY_B_SERIAL}")
        return
    
    teensy = serial.Serial(teensy_port, 9600, timeout=0.1)
    print(f"✅ Connected to Teensy B")
    
    # Wait for startup and clear buffer
    time.sleep(3)
    while teensy.in_waiting > 0:
        teensy.readline()
    
    print("🌈 Pulsing each strip...")
    
    try:
        # Pulse each strip 3 times
        for cycle in range(3):
            print(f"Cycle {cycle + 1}/3")
            
            for strip_id in range(8):
                print(f"  Strip {strip_id}...")
                
                # Send pulse command
                packet = CommandPacket(CMD_LED_PULSE, 1, [strip_id])
                teensy.write(packet.to_bytes())
                
                # Wait between strips
                time.sleep(1)
            
            print()
    
    except KeyboardInterrupt:
        print("Stopped by user")
    finally:
        teensy.close()
        print("✅ Done!")

if __name__ == "__main__":
    main() 