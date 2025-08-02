#!/usr/bin/env python3
"""
Simple test script to monitor only Teensy A for button presses
"""

import serial
import time

def test_teensy_a_only(port='/dev/ttyACM0'):
    """Test Teensy A button detection in isolation"""
    print(f"🔍 TESTING TEENSY A ONLY - {port}")
    print("=" * 50)
    print("Expected output:")
    print("• Every 10s: PIN_STATES:1,1,1,1,1,1")
    print("• On button press: BUTTON_PRESS:X")
    print("• Any startup messages")
    print("\n💡 Press buttons to test!")
    print("   Press Ctrl+C to stop")
    print("-" * 50)
    
    try:
        ser = serial.Serial(port, 9600, timeout=0.1)
        print(f"✅ Connected to {port}")
        
        last_output_time = time.time()
        
        while True:
            if ser.in_waiting > 0:
                try:
                    line = ser.readline().decode('utf-8').strip()
                    if line:
                        current_time = time.strftime("%H:%M:%S")
                        print(f"[{current_time}] 📥 {line}")
                        last_output_time = time.time()
                        
                        # Analyze the output
                        if "BUTTON_PRESS:" in line:
                            print(f"  🔘 BUTTON DETECTED! {line}")
                        elif "PIN_STATES:" in line:
                            print(f"  📍 Pin status: {line}")
                        elif "Teensy B" in line:
                            print(f"  ⚠️  This looks like Teensy B output - port may be wrong!")
                        elif "Starting Teensy" in line:
                            print(f"  🚀 Startup message: {line}")
                            
                except Exception as e:
                    print(f"❌ Error reading line: {e}")
            
            # Show "listening" status every 5 seconds if no output
            current_time = time.time()
            if current_time - last_output_time > 5:
                print(f"[{time.strftime('%H:%M:%S')}] 👂 Listening for output... (press buttons)")
                last_output_time = current_time
            
            time.sleep(0.01)
            
    except KeyboardInterrupt:
        print("\n⏹️  Stopping test...")
    except Exception as e:
        print(f"❌ Connection error: {e}")
        print("💡 Try switching the port to /dev/ttyACM1 if this doesn't work")
    finally:
        try:
            ser.close()
        except:
            pass
        print("✅ Test complete!")

if __name__ == "__main__":
    # Test the port you said Teensy A is on
    test_teensy_a_only('/dev/ttyACM0') 