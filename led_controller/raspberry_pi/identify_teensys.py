#!/usr/bin/env python3
"""
Script to identify which Teensy is on which port
"""

import serial
import time

def check_port(port, name):
    """Check what's connected to a specific port"""
    try:
        ser = serial.Serial(port, 9600, timeout=2)
        print(f"\n🔍 Checking {name} ({port})...")
        
        # Read a few lines to see what device responds
        start_time = time.time()
        messages = []
        
        while time.time() - start_time < 3:
            if ser.in_waiting > 0:
                try:
                    line = ser.readline().decode('utf-8').strip()
                    if line:
                        messages.append(line)
                        print(f"   📥 {line}")
                except:
                    pass
            time.sleep(0.1)
        
        ser.close()
        
        # Analyze messages to determine device type
        if any("Teensy B" in msg for msg in messages):
            print(f"   ✅ {name} = TEENSY B")
            return "B"
        elif any("BUTTON_PRESS" in msg or "PIN_STATES" in msg for msg in messages):
            print(f"   ✅ {name} = TEENSY A")
            return "A"
        elif any("Starting Teensy A" in msg for msg in messages):
            print(f"   ✅ {name} = TEENSY A")
            return "A"
        else:
            print(f"   ❓ {name} = UNKNOWN (no identifying messages)")
            return "?"
            
    except Exception as e:
        print(f"   ❌ {name} = ERROR: {e}")
        return None

def main():
    print("🔎 TEENSY PORT IDENTIFICATION")
    print("=" * 40)
    
    port0_type = check_port('/dev/ttyACM0', 'Port 0')
    port1_type = check_port('/dev/ttyACM1', 'Port 1')
    
    print("\n📋 SUMMARY:")
    print("-" * 40)
    if port0_type == "A":
        print("✅ /dev/ttyACM0 → Teensy A (buttons)")
    elif port0_type == "B":
        print("✅ /dev/ttyACM0 → Teensy B (LED controller)")
    
    if port1_type == "A":
        print("✅ /dev/ttyACM1 → Teensy A (buttons)")
    elif port1_type == "B":
        print("✅ /dev/ttyACM1 → Teensy B (LED controller)")
    
    print("\n💡 Use these assignments in your test scripts!")

if __name__ == "__main__":
    main() 