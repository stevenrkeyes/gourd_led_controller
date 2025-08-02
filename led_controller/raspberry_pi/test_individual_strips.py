#!/usr/bin/env python3
"""
Individual LED strip test for troubleshooting hardware wiring
Tests each strip one by one with clear feedback
"""

import serial
import time
import struct

# Communication protocol constants
CMD_LED_PULSE = 0x01

# Pin mapping for reference
PIN_MAPPING = {
    0: 2,
    1: 14,
    2: 7,
    3: 8,
    4: 6,
    5: 20,
    6: 21,
    7: 5
}

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

def test_individual_strips():
    """Test each LED strip individually for wiring diagnosis"""
    
    # Connect to Teensy B
    try:
        teensy_b = serial.Serial('/dev/ttyACM0', 9600, timeout=0.1)
        print(f"✅ Connected to Teensy B on /dev/ttyACM0")
        
        # Wait for Teensy to complete startup
        print("⏳ Waiting for Teensy B to complete startup...")
        time.sleep(3)
        
        # Clear any startup messages
        while teensy_b.in_waiting > 0:
            teensy_b.readline()
            
    except Exception as e:
        print(f"❌ Failed to connect to Teensy B: {e}")
        return
    
    print("\n" + "="*60)
    print("🔧 INDIVIDUAL LED STRIP WIRING TEST")
    print("="*60)
    print("Testing each strip individually...")
    print("Watch for bright white pulses on each strip")
    print()
    print("Pin Mapping:")
    for strip, pin in PIN_MAPPING.items():
        print(f"  Strip {strip} → Pin {pin}")
    print("-" * 60)
    
    working_strips = []
    
    try:
        for strip_id in range(8):
            print(f"\n🧪 Testing Strip {strip_id} (Pin {PIN_MAPPING[strip_id]})...")
            print(f"   Look for a bright WHITE pulse traveling down strip {strip_id}")
            
            # Create LED pulse command
            packet = CommandPacket(CMD_LED_PULSE, 1, [strip_id])
            packet_bytes = packet.to_bytes()
            
            # Send command
            teensy_b.write(packet_bytes)
            
            # Wait for response
            time.sleep(0.5)
            
            # Read debug output
            response_received = False
            while teensy_b.in_waiting > 0:
                response = teensy_b.readline().decode('utf-8').strip()
                if response:
                    print(f"   🅱️  {response}")
                    if "Triggered pulse" in response:
                        response_received = True
            
            if response_received:
                print(f"   ✅ Command sent successfully to strip {strip_id}")
            else:
                print(f"   ⚠️  No response from Teensy B")
            
            # Ask user what they observed
            print(f"   ❓ Did you see a WHITE pulse on strip {strip_id}? (y/n): ", end="")
            user_input = input().lower().strip()
            
            if user_input in ['y', 'yes']:
                working_strips.append(strip_id)
                print(f"   🟢 Strip {strip_id} is working!")
            else:
                print(f"   🔴 Strip {strip_id} not working")
            
            print(f"   ⏳ Waiting 2 seconds before next test...")
            time.sleep(2)
        
        # Summary
        print("\n" + "="*60)
        print("📊 TEST SUMMARY")
        print("="*60)
        print(f"Working strips: {working_strips}")
        print(f"Non-working strips: {[i for i in range(8) if i not in working_strips]}")
        
        if len(working_strips) == 8:
            print("🎉 All strips working perfectly!")
        elif len(working_strips) > 0:
            print("🔧 Some strips working - check wiring on non-working strips")
            print("\nTroubleshooting tips:")
            for strip in range(8):
                if strip not in working_strips:
                    print(f"  • Strip {strip}: Check connection to Pin {PIN_MAPPING[strip]}")
        else:
            print("❌ No strips working - check power supply and main connections")
        
    except KeyboardInterrupt:
        print("\n⏹️  Test stopped by user")
    except Exception as e:
        print(f"❌ Error during test: {e}")
    finally:
        teensy_b.close()
        print("\n✅ Test complete!")

if __name__ == "__main__":
    test_individual_strips() 