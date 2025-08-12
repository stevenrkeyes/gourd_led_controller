#!/usr/bin/env python3
"""
Ring LED test for pin 22 on Teensy A
Tests the ring LEDs with various patterns and colors
"""

import serial
import serial.tools.list_ports
import time
import struct

# Communication protocol constants
CMD_RING_LED_TEST = 0x30  # New command for ring LED testing

# Teensy A serial number (from the other test files)
TEENSY_A_SERIAL = "14094100"  # Button/ring LED controller

class CommandPacket:
    """Binary command packet for Teensy A"""
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

def find_teensy_a():
    """Find Teensy A device by serial number"""
    print(f"🔍 Searching for Teensy A with serial number: {TEENSY_A_SERIAL}")
    
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if port.serial_number == TEENSY_A_SERIAL:
            print(f"✅ Found Teensy A at {port.device}")
            return port.device
        if port.serial_number:
            print(f"   Found device: {port.device} (Serial: {port.serial_number})")
    
    return None

def test_ring_leds():
    """Test ring LEDs with various patterns"""
    
    # Connect to Teensy A
    teensy_port = find_teensy_a()
    if not teensy_port:
        print(f"❌ Could not find Teensy A with serial {TEENSY_A_SERIAL}")
        print("Available devices:")
        ports = serial.tools.list_ports.comports()
        for port in ports:
            print(f"  {port.device}: {port.description} (Serial: {port.serial_number})")
        return
    
    try:
        teensy_a = serial.Serial(teensy_port, 9600, timeout=0.1)
        print(f"✅ Connected to Teensy A on {teensy_port}")
        
        # Wait for startup and clear buffer
        print("⏳ Waiting for Teensy A startup...")
        time.sleep(3)
        while teensy_a.in_waiting > 0:
            teensy_a.readline()
            
    except Exception as e:
        print(f"❌ Failed to connect to Teensy A: {e}")
        return
    
    print("\n" + "="*60)
    print("💍 RING LED TEST - PIN 22")
    print("="*60)
    print("Testing ring LEDs connected to pin 22 on Teensy A")
    print("-" * 60)
    
    test_patterns = [
        {"name": "All Red", "data": [0, 255, 0, 0]},      # pattern_id=0, R=255, G=0, B=0
        {"name": "All Green", "data": [1, 0, 255, 0]},    # pattern_id=1, R=0, G=255, B=0
        {"name": "All Blue", "data": [2, 0, 0, 255]},     # pattern_id=2, R=0, G=0, B=255
        {"name": "All White", "data": [3, 255, 255, 255]}, # pattern_id=3, R=255, G=255, B=255
        {"name": "Rainbow", "data": [4, 0, 0, 0]},        # pattern_id=4, rainbow pattern
        {"name": "Chase", "data": [5, 255, 255, 255]},    # pattern_id=5, white chase
        {"name": "Off", "data": [99, 0, 0, 0]},           # pattern_id=99, turn off
    ]
    
    try:
        for i, pattern in enumerate(test_patterns):
            print(f"\n🌈 Test {i+1}/{len(test_patterns)}: {pattern['name']}")
            print(f"   Sending pattern data: {pattern['data']}")
            
            # Create command packet
            packet = CommandPacket(CMD_RING_LED_TEST, 4, pattern['data'])
            
            # Send command
            teensy_a.write(packet.to_bytes())
            
            # Wait and read any response
            time.sleep(0.5)
            response_received = False
            while teensy_a.in_waiting > 0:
                response = teensy_a.readline().decode('utf-8').strip()
                if response:
                    print(f"   🅰️  {response}")
                    response_received = True
            
            if not response_received:
                print(f"   ⚠️  No response from Teensy A")
            
            # Ask user what they observed
            print(f"   ❓ Did you see the '{pattern['name']}' pattern on the ring? (y/n/s to skip): ", end="")
            user_input = input().lower().strip()
            
            if user_input == 's':
                print("   ⏭️  Skipping to next pattern...")
            elif user_input in ['y', 'yes']:
                print(f"   ✅ {pattern['name']} pattern working!")
            else:
                print(f"   ❌ {pattern['name']} pattern not working")
            
            if user_input != 's':
                print(f"   ⏳ Waiting 2 seconds before next pattern...")
                time.sleep(2)
        
        print("\n" + "="*60)
        print("🎯 RING LED TEST COMPLETE")
        print("="*60)
        print("If patterns didn't work, check:")
        print("• Ring LEDs are connected to pin 22 on Teensy A")
        print("• Power supply is adequate for the ring LEDs")
        print("• Ground connection is secure")
        print("• Data line is connected properly")
        print("• Ring LEDs are WS2812/NeoPixel compatible")
        
    except KeyboardInterrupt:
        print("\n⏹️  Test stopped by user")
    except Exception as e:
        print(f"❌ Error during test: {e}")
    finally:
        # Turn off LEDs before closing
        try:
            off_packet = CommandPacket(CMD_RING_LED_TEST, 4, [99, 0, 0, 0])
            teensy_a.write(off_packet.to_bytes())
            time.sleep(0.1)
        except:
            pass
        
        teensy_a.close()
        print("\n✅ Ring LED test complete!")

if __name__ == "__main__":
    test_ring_leds()
