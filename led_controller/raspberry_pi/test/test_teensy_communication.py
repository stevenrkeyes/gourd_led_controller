# -*- coding: utf-8 -*-
#!/usr/bin/env python3
"""
Test script for Raspberry Pi to communicate with Teensy A
This script reads serial data from Teensy A and displays button presses and sensor data
"""

import serial
import time
import sys
from typing import Optional

# Import centralized configuration and utilities
from utils import (
    TEENSY_A_SERIAL,
    TEENSY_B_SERIAL,
    CMD_LED_PULSE,
    CMD_LED_EFFECT,
    CMD_BUTTON_PRESS,
    CMD_BUTTON_LED,
    CMD_SENSOR_DATA,
    CMD_HEARTBEAT,
    find_teensy,
    CommandPacket,
    create_button_led_packet
)

# Removed duplicated CommandPacket class - now using centralized protocol

class TeensyAMonitor:
    def __init__(self, port='/dev/ttyACM0', baudrate=9600):
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        self.running = False
    
    def connect(self):
        """Connect to Teensy A"""
        try:
            self.ser = serial.Serial(self.port, self.baudrate, timeout=1)
            print(f"Connected to Teensy A on {self.port} at {self.baudrate} baud")
            time.sleep(2)  # Give Arduino time to reset
            return True
        except serial.SerialException as e:
            print(f"Failed to connect to {self.port}: {e}")
            return False
    
    def disconnect(self):
        """Disconnect from Teensy A"""
        if self.ser and self.ser.is_open:
            self.ser.close()
            print("Disconnected from Teensy A")
    
    def send_button_led_command(self, button_id, r, g, b, brightness=255):
        """Send command to control button LED"""
        if not self.ser or not self.ser.is_open:
            print("Not connected to Teensy A")
            return
        
        # Use centralized protocol function
        packet = create_button_led_packet(button_id, r, g, b)
        packet_bytes = packet.to_bytes()
        
        self.ser.write(packet_bytes)
        print(f"Sent button LED command: Button {button_id} -> RGB({r},{g},{b})")
    
    def read_packet(self) -> Optional[CommandPacket]:
        """Read a packet from Teensy A"""
        if not self.ser or not self.ser.is_open:
            return None
        
        if self.ser.in_waiting >= 35:  # Full packet size
            try:
                data = self.ser.read(35)
                return CommandPacket.from_bytes(data)
            except Exception as e:
                print(f"Error reading packet: {e}")
                return None
        
        return None
    
    def monitor(self):
        """Main monitoring loop"""
        print("Starting Teensy A monitor...")
        print("Press Ctrl+C to stop")
        print("Commands:")
        print("  1-4: Set button LED colors")
        print("  q: Quit")
        print("-" * 50)
        
        self.running = True
        
        try:
            while self.running:
                # Read text output from Teensy
                if self.ser.in_waiting > 0:
                    try:
                        line = self.ser.readline().decode('utf-8').strip()
                        if line:
                            self.handle_text_command(line)
                    except Exception as e:
                        print(f"Error reading line: {e}")
                
                time.sleep(0.1)
                
        except KeyboardInterrupt:
            print("\nShutting down...")
        finally:
            self.running = False
    
    def handle_text_command(self, line):
        """Handle text commands from Teensy A"""
        current_time = time.strftime("%H:%M:%S")
        
        if line.startswith("BUTTON_PRESS:"):
            # Parse button press: "BUTTON_PRESS:1"
            try:
                button_id = int(line.split(":")[1])
                print(f"[{current_time}] 🔘 BUTTON PRESSED: Button {button_id}")
                
                # Send LED feedback - light up the pressed button
                colors = [(255,0,0), (0,255,0), (0,0,255), (255,255,0), (255,0,255), (0,255,255)]
                if button_id >= 1 and button_id <= len(colors):
                    r, g, b = colors[button_id - 1]  # Array is 0-based
                    print(f"Would set button LED {button_id} to RGB({r},{g},{b})")
                    # Note: LED command sending is disabled for now since we're using text mode
            except (ValueError, IndexError) as e:
                print(f"Error parsing button press: {line} - {e}")
        
        elif line.startswith("PIN_STATES:"):
            # Debug info: "PIN_STATES:1,1,1,1,1,1"
            print(f"[{current_time}] 📍 {line}")
        
        else:
            # Any other debug output
            print(f"[{current_time}] 🔧 Teensy Debug: {line}")

def find_teensy_port():
    """Try to find a Teensy port, preferring Teensy A"""
    # Try Teensy A first (most likely for communication tests)
    port = find_teensy("a", verbose=False)
    if port:
        print(f"Found Teensy A at {port}")
        return port
    
    # Try Teensy B as fallback
    port = find_teensy("b", verbose=False)
    if port:
        print(f"Found Teensy B at {port}")
        return port
    
    # Last resort: default fallback
    print("No known Teensy found, using default port")
    return '/dev/ttyACM0'

def main():
    print("Teensy A Communication Test")
    print("=" * 40)
    
    # Try to find Teensy port
    port = find_teensy_port()
    
    # Allow user to specify different port
    if len(sys.argv) > 1:
        port = sys.argv[1]
        print(f"Using specified port: {port}")
    
    monitor = TeensyAMonitor(port)
    
    if monitor.connect():
        try:
            monitor.monitor()
        finally:
            monitor.disconnect()
    else:
        print("Failed to connect to Teensy A")
        print("Make sure:")
        print("1. Teensy A is connected via USB")
        print("2. The correct port is being used")
        print(f"3. Try: python3 {sys.argv[0]} /dev/ttyUSB0")

if __name__ == "__main__":
    main() 