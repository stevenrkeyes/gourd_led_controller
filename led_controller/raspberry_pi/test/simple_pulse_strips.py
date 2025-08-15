#!/usr/bin/env python3
"""
Simple automatic LED strip pulse test
Just pulses each strip 0-7 in sequence, no user input required
"""

import serial
import time

# Import centralized configuration and utilities
from config import TEENSY_B_SERIAL, CMD_LED_PULSE
from device_utils import find_teensy_b
from protocol import create_led_pulse_packet

def main():
    # Connect to Teensy B using centralized utility
    teensy_port = find_teensy_b(verbose=True)
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
                
                # Send pulse command using centralized protocol
                packet = create_led_pulse_packet(strip_id)
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