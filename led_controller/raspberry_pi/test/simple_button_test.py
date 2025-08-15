#!/usr/bin/env python3
"""
Simple button test for Teensy A
Just prints when buttons are pressed
"""

import serial
import serial.tools.list_ports
import time

# Import centralized configuration
from config import TEENSY_A_SERIAL

def find_teensy_a():
    """Find Teensy A by serial number"""
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if port.serial_number == TEENSY_A_SERIAL:
            return port.device
    return None

def main():
    print("🔍 Looking for Teensy A...")
    
    # Find Teensy A
    port = find_teensy_a()
    if not port:
        print("❌ Teensy A not found!")
        return
    
    print(f"✅ Found Teensy A on {port}")
    
    try:
        # Connect
        teensy = serial.Serial(port, 9600, timeout=0.1)
        time.sleep(2)  # Wait for startup
        
        print("📋 Listening for button presses... (Press Ctrl+C to stop)")
        print("-" * 40)
        
        while True:
            if teensy.in_waiting > 0:
                line = teensy.readline().decode('utf-8').strip()
                # print(line)
                if "BUTTON_PRESS:" in line:
                    button_num = line.split(":")[1]
                    print(f"Button {button_num} pressed!")
                    
            time.sleep(0.1)
            
    except KeyboardInterrupt:
        print("\n👋 Stopping...")
    except Exception as e:
        print(f"❌ Error: {e}")
    finally:
        if 'teensy' in locals():
            teensy.close()
        print("✅ Done!")

if __name__ == "__main__":
    main() 