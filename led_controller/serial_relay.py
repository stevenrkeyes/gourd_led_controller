#!/usr/bin/env python3
"""
Serial relay script for Teensy LED controller.
Finds Teensy serial port and sends 'a' over serial when keyboard 'a' is pressed.
"""

import serial
import serial.tools.list_ports
import time
import sys
import threading
from pynput import keyboard

def find_teensy_port():
    """Find Teensy serial port on both Windows and Unix."""
    ports = serial.tools.list_ports.comports()
    
    for port in ports:
        # Look for Teensy in port description or manufacturer
        desc = port.description.lower()
        manufacturer = getattr(port, 'manufacturer', '').lower()
        
        # Common Teensy identifiers
        teensy_indicators = ['teensy', 'pjrc', 'usb serial']
        
        for indicator in teensy_indicators:
            if indicator in desc or indicator in manufacturer:
                return port.device
    
    return None

def on_key_press(key, ser):
    """Handle key press events."""
    try:
        if key.char == 'a':
            print("Keyboard 'a' pressed, sending 'a' over serial")
            ser.write(b'a')
            ser.flush()
    except AttributeError:
        # Special keys don't have char attribute
        pass

def main():
    print("Looking for Teensy serial port...")
    
    # Find Teensy port
    port_name = find_teensy_port()
    if not port_name:
        print("Error: Teensy not found!")
        print("Available ports:")
        for port in serial.tools.list_ports.comports():
            print(f"  {port.device}: {port.description}")
        sys.exit(1)
    
    print(f"Found Teensy on {port_name}")
    
    # Connect to serial port
    try:
        ser = serial.Serial(port_name, 9600, timeout=1)
        print("Connected to Teensy")
        print("Press 'a' on keyboard to send 'a' over serial (Ctrl+C to exit)")
        
        # Set up keyboard listener
        def on_press(key):
            on_key_press(key, ser)
        
        with keyboard.Listener(on_press=on_press) as listener:
            listener.join()
            
    except Exception as e:
        print(f"Serial error: {e}")
        sys.exit(1)
    except KeyboardInterrupt:
        print("\nExiting...")
    finally:
        if 'ser' in locals():
            ser.close()

if __name__ == "__main__":
    main() 