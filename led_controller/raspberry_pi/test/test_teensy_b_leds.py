#!/usr/bin/env python3
"""
Simple test script to verify Teensy B LED functionality
Sends LED pulse commands directly to Teensy B
"""

import serial
import time
import struct

# Communication protocol constants
CMD_LED_PULSE = 0x01

class CommandPacket:
    """Binary command packet for Teensy B"""
    def __init__(self, command=0, data_length=0, data=None, checksum=0):
        self.command = command
        self.data_length = data_length
        self.data = data if data is not None else [0] * 32
        self.checksum = checksum
    
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
        self.checksum = self.calculate_checksum()
        packet_bytes = struct.pack('BB32sB', 
                                 self.command, 
                                 self.data_length, 
                                 bytes(self.data), 
                                 self.checksum)
        return packet_bytes

def test_teensy_b_leds():
    """Test LED strips on Teensy B"""
    
    # Connect to Teensy B
    try:
        teensy_b = serial.Serial('/dev/ttyACM0', 9600, timeout=0.1)
        print(f"✅ Connected to Teensy B on /dev/ttyACM0")
    except Exception as e:
        print(f"❌ Failed to connect to Teensy B: {e}")
        return
    
    print("\n" + "="*50)
    print("🚀 TEENSY B LED TEST")
    print("="*50)
    print("Testing LED pulses on all 8 strips...")
    print("Each strip will show a bright white pulse")
    print("-" * 50)
    
    try:
        # Test each strip
        for strip_id in range(8):
            print(f"📤 Testing strip {strip_id}...")
            
            # Create LED pulse command
            packet = CommandPacket(CMD_LED_PULSE, 1, [strip_id])
            packet_bytes = packet.to_bytes()
            
            # Send command
            teensy_b.write(packet_bytes)
            
            # Wait to see the pulse
            time.sleep(1.0)  # 1 second between pulses
            
            # Read any debug output from Teensy B
            if teensy_b.in_waiting > 0:
                response = teensy_b.readline().decode('utf-8').strip()
                if response:
                    print(f"   🅱️ Teensy B: {response}")
        
        print("\n🎉 LED test complete!")
        print("💡 Did you see white pulses traveling down your LED strips?")
        
    except KeyboardInterrupt:
        print("\n⏹️ Test stopped by user")
    except Exception as e:
        print(f"❌ Error during test: {e}")
    finally:
        teensy_b.close()
        print("✅ Test complete!")

if __name__ == "__main__":
    test_teensy_b_leds() 