# -*- coding: utf-8 -*-
#!/usr/bin/env python3
"""
Individual LED strip test for troubleshooting hardware wiring
Tests each strip one by one with clear feedback
"""

import serial
import time

# Import centralized configuration and utilities
from config import TEENSY_B_SERIAL, CMD_LED_PULSE, LED_STRIP_PIN_MAPPING
from device_utils import find_teensy_by_serial, print_available_ports
from protocol import create_led_pulse_packet

# Pin mapping for reference (now imported from config)
PIN_MAPPING = LED_STRIP_PIN_MAPPING

# Removed duplicated CommandPacket and find_teensy_by_serial - now using centralized utilities

def test_individual_strips():
    """Test each LED strip individually for wiring diagnosis"""
    
    # Connect to Teensy B
    try:
        # Find Teensy B by serial number using centralized utility
        teensy_port = find_teensy_by_serial(TEENSY_B_SERIAL, verbose=True)
        if not teensy_port:
            print(f"❌ Could not find Teensy B with serial number {TEENSY_B_SERIAL}")
            print("Available devices:")
            print_available_ports()
            return
        
        teensy_b = serial.Serial(teensy_port, 9600, timeout=0.1)
        print(f"✅ Connected to Teensy B on {teensy_port}")
        
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
            
            # Create LED pulse command using centralized protocol
            packet = create_led_pulse_packet(strip_id)
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
            
            # print(f"   ⏳ Waiting 2 seconds before next test...")
            # time.sleep(2)
        
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