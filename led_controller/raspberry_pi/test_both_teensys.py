#!/usr/bin/env python3
"""
Test script for complete Teensy A → Pi → Teensy B communication flow
Auto-detects Teensys by serial number for reliable port assignment
"""

import serial
import time
import struct
import threading
import subprocess
import re
from typing import Optional, Dict

# Communication protocol constants
CMD_LED_PULSE = 0x01
CMD_LED_EFFECT = 0x02
CMD_BUTTON_PRESS = 0x10
CMD_BUTTON_LED = 0x11
CMD_SENSOR_DATA = 0x20
CMD_HEARTBEAT = 0xFF

# Known Teensy serial numbers
TEENSY_A_SERIAL = "14094100"  # Button controller
TEENSY_B_SERIAL = "17656680"  # LED controller

class CommandPacket:
    """Binary command packet for Teensy B"""
    def __init__(self, command=0, data_length=0, data=None, checksum=0):
        self.command = command
        self.data_length = data_length
        self.data = data if data is not None else [0] * 32
        self.checksum = checksum
    
    def calculate_checksum(self):
        """Calculate XOR checksum"""
        checksum = 0
        checksum ^= self.command
        checksum ^= self.data_length
        for i in range(self.data_length):
            checksum ^= self.data[i]
        return checksum
    
    def to_bytes(self):
        """Convert packet to bytes for transmission"""
        self.checksum = self.calculate_checksum()
        packet_bytes = struct.pack('BB32sB', 
                                 self.command, 
                                 self.data_length, 
                                 bytes(self.data), 
                                 self.checksum)
        return packet_bytes

def detect_teensy_ports() -> Dict[str, str]:
    """
    Detect Teensy ports by serial number
    Returns: {'teensy_a': '/dev/ttyACMX', 'teensy_b': '/dev/ttyACMY'}
    """
    try:
        # Run pio device list and parse output
        result = subprocess.run(['pio', 'device', 'list'], 
                              capture_output=True, text=True, check=True)
        
        ports = {}
        current_port = None
        
        for line in result.stdout.split('\n'):
            # Look for port lines like "/dev/ttyACM0"
            if line.startswith('/dev/ttyACM'):
                current_port = line.strip()
            # Look for serial number lines like "Hardware ID: USB VID:PID=16C0:0487 SER=14094100"
            elif current_port and 'SER=' in line:
                match = re.search(r'SER=(\d+)', line)
                if match:
                    serial_num = match.group(1)
                    if serial_num == TEENSY_A_SERIAL:
                        ports['teensy_a'] = current_port
                        print(f"🔍 Found Teensy A (SER={serial_num}) on {current_port}")
                    elif serial_num == TEENSY_B_SERIAL:
                        ports['teensy_b'] = current_port
                        print(f"🔍 Found Teensy B (SER={serial_num}) on {current_port}")
                current_port = None
        
        return ports
        
    except subprocess.CalledProcessError as e:
        print(f"❌ Error running pio device list: {e}")
        return {}
    except Exception as e:
        print(f"❌ Error detecting Teensy ports: {e}")
        return {}

class DualTeensyTester:
    def __init__(self, baudrate=9600):
        self.baudrate = baudrate
        self.teensy_a_port = None
        self.teensy_b_port = None
        self.teensy_a = None
        self.teensy_b = None
        self.running = False
        
    def connect(self):
        """Connect to both Teensys using auto-detection"""
        print("🔍 Auto-detecting Teensy devices...")
        ports = detect_teensy_ports()
        
        if 'teensy_a' not in ports:
            print(f"❌ Teensy A (SER={TEENSY_A_SERIAL}) not found!")
            return False
        if 'teensy_b' not in ports:
            print(f"❌ Teensy B (SER={TEENSY_B_SERIAL}) not found!")
            return False
            
        self.teensy_a_port = ports['teensy_a']
        self.teensy_b_port = ports['teensy_b']
        
        try:
            self.teensy_a = serial.Serial(self.teensy_a_port, self.baudrate, timeout=0.1)
            print(f"✅ Connected to Teensy A on {self.teensy_a_port}")
        except Exception as e:
            print(f"❌ Failed to connect to Teensy A: {e}")
            return False
            
        try:
            self.teensy_b = serial.Serial(self.teensy_b_port, self.baudrate, timeout=0.1)
            print(f"✅ Connected to Teensy B on {self.teensy_b_port}")
        except Exception as e:
            print(f"❌ Failed to connect to Teensy B: {e}")
            if self.teensy_a:
                self.teensy_a.close()
            return False
            
        return True
    
    def send_led_command_to_teensy_b(self, strip_id=0):
        """Send LED pulse command to Teensy B"""
        if not self.teensy_b:
            print("❌ Teensy B not connected")
            return
            
        packet = CommandPacket(CMD_LED_PULSE, 1, [strip_id])
        packet_bytes = packet.to_bytes()
        
        try:
            self.teensy_b.write(packet_bytes)
            print(f"📤 Sent LED pulse command to Teensy B (strip {strip_id})")
        except Exception as e:
            print(f"❌ Error sending to Teensy B: {e}")
    
    def monitor_teensy_a(self):
        """Monitor Teensy A for button presses"""
        while self.running:
            if self.teensy_a and self.teensy_a.in_waiting > 0:
                try:
                    line = self.teensy_a.readline().decode('utf-8').strip()
                    if line:
                        current_time = time.strftime("%H:%M:%S")
                        print(f"[{current_time}] 🅰️  Teensy A: {line}")
                        
                        # Check for button press
                        if line.startswith("BUTTON_PRESS:"):
                            button_id = int(line.split(":")[1])
                            print(f"[{current_time}] 🔘 Button {button_id} pressed!")
                            
                            # Forward as LED command to Teensy B
                            strip_id = button_id - 1  # Convert to 0-based
                            self.send_led_command_to_teensy_b(strip_id)
                        
                except Exception as e:
                    print(f"❌ Error reading Teensy A: {e}")
            
            time.sleep(0.01)
    
    def monitor_teensy_b(self):
        """Monitor Teensy B for debug output"""
        while self.running:
            if self.teensy_b and self.teensy_b.in_waiting > 0:
                try:
                    line = self.teensy_b.readline().decode('utf-8').strip()
                    if line:
                        current_time = time.strftime("%H:%M:%S")
                        print(f"[{current_time}] 🅱️  Teensy B: {line}")
                        
                except Exception as e:
                    print(f"❌ Error reading Teensy B: {e}")
            
            time.sleep(0.01)
    
    def test_communication(self):
        """Test the complete communication flow"""
        if not self.connect():
            return
        
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
        
        self.running = True
        
        # Start monitoring threads
        thread_a = threading.Thread(target=self.monitor_teensy_a, daemon=True)
        thread_b = threading.Thread(target=self.monitor_teensy_b, daemon=True)
        
        thread_a.start()
        thread_b.start()
        
        try:
            # Send initial test command to verify Teensy B communication
            print("🧪 Sending test command to Teensy B...")
            self.send_led_command_to_teensy_b(0)
            
            # Keep main thread alive
            while self.running:
                time.sleep(0.1)
                
        except KeyboardInterrupt:
            print("\n⏹️  Stopping test...")
        finally:
            self.running = False
            if self.teensy_a:
                self.teensy_a.close()
            if self.teensy_b:
                self.teensy_b.close()
            print("✅ Test complete!")

def main():
    tester = DualTeensyTester()
    tester.test_communication()

if __name__ == "__main__":
    main() 