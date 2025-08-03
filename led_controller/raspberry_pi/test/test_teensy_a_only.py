#!/usr/bin/env python3
"""
Test script for testing up to 16 buttons on Teensy A
Identifies Teensy A by serial number and provides detailed button testing
"""

import serial
import serial.tools.list_ports
import time

# Teensy A serial number
TEENSY_A_SERIAL = "14094100"

def find_teensy_by_serial(target_serial):
    """Find Teensy device by its serial number"""
    print(f"🔍 Searching for Teensy with serial number: {target_serial}")
    
    ports = serial.tools.list_ports.comports()
    
    for port in ports:
        print(f"   Checking {port.device}: {port.description}")
        if port.serial_number == target_serial:
            print(f"   ✅ Found Teensy A at {port.device}")
            return port.device
        if port.serial_number:
            print(f"      Serial: {port.serial_number}")
    
    return None

def test_teensy_a_buttons():
    """Test up to 16 buttons on Teensy A with detailed tracking"""
    
    # Find Teensy A by serial number
    teensy_port = find_teensy_by_serial(TEENSY_A_SERIAL)
    if not teensy_port:
        print(f"❌ Could not find Teensy A with serial number {TEENSY_A_SERIAL}")
        print("Available devices:")
        ports = serial.tools.list_ports.comports()
        for port in ports:
            print(f"  {port.device}: {port.description} (Serial: {port.serial_number})")
        return
    
    try:
        # Connect to Teensy A
        teensy_a = serial.Serial(teensy_port, 9600, timeout=0.5)
        print(f"✅ Connected to Teensy A on {teensy_port}")
        
        time.sleep(3)  # Wait for startup
        
        # Clear buffer
        teensy_a.flushInput()
        teensy_a.flushOutput()
        
        # Clear any startup messages
        while teensy_a.in_waiting > 0:
            teensy_a.readline()
        
        print("\n" + "="*60)
        print("🔘 TEENSY A - 16 BUTTON TEST")
        print("="*60)
        print("📋 Press buttons 1-16 on Teensy A")
        print("⚠️  NOTE: Current firmware only supports 6 buttons (pins 0-5)")
        print("   To test all 16 buttons, update the firmware configuration")
        print("-" * 60)
        
        # Monitor for button presses with detailed debugging
        for i in range(100):  # Test for ~50 seconds
            try:
                if teensy_a.in_waiting > 0:
                    # Read one line at a time
                    raw_line = teensy_a.readline()
                    
                    try:
                        line = raw_line.decode('utf-8').strip()
                        if line:
                            current_time = time.strftime("%H:%M:%S")
                            print(f"[{current_time}] RAW: '{line}'")
                            
                            # Check for button press
                            if "BUTTON_PRESS:" in line:
                                try:
                                    button_id = int(line.split(":")[1])
                                    print(f"   ✅ CLEAN BUTTON PRESS: Button {button_id}")
                                except:
                                    print(f"   ⚠️  CORRUPTED BUTTON PRESS: {line}")
                            else:
                                print(f"   ℹ️  Other message: {line}")
                                
                    except UnicodeDecodeError as e:
                        print(f"   ❌ DECODE ERROR: {raw_line}")
                        
                time.sleep(0.5)
                
            except Exception as e:
                print(f"❌ Error during monitoring: {e}")
                break
        
    except Exception as e:
        print(f"❌ Overall error: {e}")
    finally:
        if teensy_a:
            teensy_a.close()
        print("✅ Test complete!")

if __name__ == "__main__":
    test_teensy_a_buttons() 