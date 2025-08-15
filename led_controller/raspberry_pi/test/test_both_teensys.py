#!/usr/bin/env python3
"""
Test script for complete Teensy A → Pi → Teensy B communication flow
Uses centralized DualTeensyTester for simplified and consistent testing
"""

import time

# Import centralized configuration and utilities
from config import (
    TEENSY_A_SERIAL,
    TEENSY_B_SERIAL,
    CMD_LED_PULSE,
    CMD_LED_EFFECT,
    CMD_BUTTON_PRESS,
    CMD_BUTTON_LED,
    CMD_SENSOR_DATA,
    CMD_HEARTBEAT
)
from dual_teensy import DualTeensyTester

def test_communication():
    """Test the complete communication flow using centralized DualTeensyTester"""
    print("\n" + "="*60)
    print("🚀 DUAL TEENSY COMMUNICATION TEST")
    print("="*60)
    print("📋 Test Flow:")
    print("   1. Press buttons on Teensy A")
    print("   2. Pi receives button press")
    print("   3. Pi sends LED command to Teensy B")
    print("   4. Teensy B shows debug output")
    print("\n💡 Press buttons to test communication!")
    print("   Press Ctrl+C to stop")
    print("-" * 60)
    
    try:
        # Use centralized DualTeensyTester with context manager
        with DualTeensyTester() as tester:
            if tester.start_monitoring():
                print("🧪 Sending test command to Teensy B...")
                tester.send_led_command_to_teensy_b(0)
                
                # Keep main thread alive
                while True:
                    time.sleep(0.1)
            else:
                print("❌ Failed to start monitoring")
                
    except KeyboardInterrupt:
        print("\n⏹️  Stopping test...")
    except Exception as e:
        print(f"❌ Error during test: {e}")
    finally:
        print("✅ Test complete!")

def main():
    test_communication()

if __name__ == "__main__":
    main()