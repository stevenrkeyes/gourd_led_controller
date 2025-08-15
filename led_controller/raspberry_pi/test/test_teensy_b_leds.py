#!/usr/bin/env python3
"""
Simple test script to verify Teensy B LED functionality
Sends LED pulse commands directly to Teensy B
"""

import serial
import time
import sys
import os

# Add parent directory to path when running directly
if __name__ == "__main__":
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# Import centralized configuration and protocol
from utils import find_teensy, create_led_pulse_packet

def test_teensy_b_leds():
    """Test LED strips on Teensy B"""
    
    # Find and connect to Teensy B using standardized approach
    teensy_port = find_teensy("b")
    if not teensy_port:
        return
    
    try:
        teensy_b = serial.Serial(teensy_port, 9600, timeout=0.1)
        print(f"✅ Connected to Teensy B on {teensy_port}")
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
            
            # Create LED pulse command using standardized function
            packet = create_led_pulse_packet(strip_id)
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