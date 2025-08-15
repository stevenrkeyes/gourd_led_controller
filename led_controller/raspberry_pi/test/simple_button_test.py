#!/usr/bin/env python3
"""
Simple button test for Teensy A
Just prints when buttons are pressed
"""

import serial
import time

# Import centralized configuration and device utilities
from utils import find_teensy

def main():
    port = find_teensy("a")
    if not port:
        return
    
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