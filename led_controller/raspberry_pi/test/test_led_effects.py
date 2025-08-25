#!/usr/bin/env python3
"""
LED Effects Test Script

Demonstrates how to control the new LED effects from the Raspberry Pi.
This script will cycle through all available LED effects.
"""

import time
import sys
import os

# Add the parent directory to path so we can import from utils
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from utils.dual_teensy import DualTeensyManager
from utils.protocol import (
    create_led_effect_packet,
    create_led_pulse_packet
)

def test_led_effects():
    """Test all LED effects by cycling through them"""
    
    print("🎨 LED Effects Test")
    print("==================")
    
    # Initialize Teensy manager
    manager = DualTeensyManager()
    
    if not manager.connect():
        print("❌ Failed to connect to Teensys")
        return False
    
    try:
        print("🚀 Testing LED effects...")
        
        # Test 1: Red Breathing Effect (like the eye LEDs)
        print("\n1️⃣ Testing Red Breathing Effect")
        packet = create_led_effect_packet(1)  # Effect ID 1 = red breathing
        if manager.send_to_teensy_b(packet):
            print("✅ Red breathing effect started")
        time.sleep(8)  # Let it breathe for a bit
        
        # Test some pulses on top of the breathing
        print("   Adding some pulses...")
        for strip in [0, 2, 4, 6]:
            pulse_packet = create_led_pulse_packet(strip)
            manager.send_to_teensy_b(pulse_packet)
            time.sleep(0.5)
        
        time.sleep(3)
        
        # Test 2: Sine Breathing Effect (multi-color)
        print("\n2️⃣ Testing Sine Breathing Effect")
        packet = create_led_effect_packet(2)  # Effect ID 2 = sine breathing
        if manager.send_to_teensy_b(packet):
            print("✅ Sine breathing effect started")
        time.sleep(6)
        
        # Test 3: Pulses Only Mode
        print("\n3️⃣ Testing Pulses Only Mode")
        packet = create_led_effect_packet(3)  # Effect ID 3 = pulses only
        if manager.send_to_teensy_b(packet):
            print("✅ Pulses only mode activated")
        
        # Send some pulses to see them clearly
        print("   Sending pulse sequence...")
        for i in range(8):
            pulse_packet = create_led_pulse_packet(i)
            manager.send_to_teensy_b(pulse_packet)
            time.sleep(0.3)
        
        time.sleep(2)
        
        # Test 4: Effects Off
        print("\n4️⃣ Testing Effects Off")
        packet = create_led_effect_packet(0)  # Effect ID 0 = off
        if manager.send_to_teensy_b(packet):
            print("✅ All effects turned off")
        time.sleep(3)
        
        # Return to default red breathing
        print("\n🔄 Returning to Red Breathing Effect")
        packet = create_led_effect_packet(1)
        manager.send_to_teensy_b(packet)
        
        print("\n✅ LED Effects test completed successfully!")
        print("\nAvailable effects:")
        print("  - Red Breathing (like eye LEDs)")
        print("  - Sine Breathing (multi-color)")
        print("  - Pulses Only")
        print("  - Effects Off")
        
        return True
        
    except KeyboardInterrupt:
        print("\n⏹️  Test interrupted by user")
        return True
    except Exception as e:
        print(f"\n❌ Error during test: {e}")
        return False
    finally:
        manager.disconnect()

def interactive_effect_control():
    """Interactive mode for manual effect control"""
    
    print("\n🎮 Interactive LED Effects Control")
    print("=================================")
    
    manager = DualTeensyManager()
    
    if not manager.connect():
        print("❌ Failed to connect to Teensys")
        return
    
    try:
        while True:
            print("\nChoose an effect:")
            print("1 - Red Breathing")
            print("2 - Sine Breathing")
            print("3 - Pulses Only")
            print("4 - Effects Off")
            print("5 - Send Pulse (strip 0)")
            print("6 - Send Pulse (strip 3)")
            print("q - Quit")
            
            choice = input("\nEnter choice: ").strip().lower()
            
            if choice == 'q':
                break
            elif choice == '1':
                packet = create_led_effect_packet(1)
                manager.send_to_teensy_b(packet)
                print("✅ Red breathing effect activated")
            elif choice == '2':
                packet = create_led_effect_packet(2)
                manager.send_to_teensy_b(packet)
                print("✅ Sine breathing effect activated")
            elif choice == '3':
                packet = create_led_effect_packet(3)
                manager.send_to_teensy_b(packet)
                print("✅ Pulses only mode activated")
            elif choice == '4':
                packet = create_led_effect_packet(0)
                manager.send_to_teensy_b(packet)
                print("✅ All effects turned off")
            elif choice == '5':
                packet = create_led_pulse_packet(0)
                manager.send_to_teensy_b(packet)
                print("✅ Pulse sent to strip 0")
            elif choice == '6':
                packet = create_led_pulse_packet(3)
                manager.send_to_teensy_b(packet)
                print("✅ Pulse sent to strip 3")
            else:
                print("❌ Invalid choice")
    
    except KeyboardInterrupt:
        print("\n⏹️  Exiting...")
    finally:
        manager.disconnect()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "-i":
        interactive_effect_control()
    else:
        test_led_effects()
