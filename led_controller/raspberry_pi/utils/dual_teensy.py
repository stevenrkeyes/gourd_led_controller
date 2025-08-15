#!/usr/bin/env python3
"""
High-level Teensy communication manager

Provides the DualTeensyTester class for managing connections to both
Teensy A (button controller) and Teensy B (LED controller) simultaneously.
"""

import serial
import time
import threading
from typing import Optional, Callable

from .config import (
    TEENSY_A_SERIAL,
    TEENSY_B_SERIAL,
    TEENSY_C_SERIAL,
    DEFAULT_BAUDRATE
)
from .device_utils import detect_all_teensys
from .protocol import CommandPacket, create_led_pulse_packet


class DualTeensyTester:
    """
    Manages communication with Teensy A sending to Teensy B and C
    
    This class handles:
    - Auto-detection and connection to both Teensy devices
    - Monitoring Teensy A for button presses
    - Sending LED commands to Teensy B and C
    - Coordinating communication between devices
    """
    
    def __init__(self, baudrate=DEFAULT_BAUDRATE, sound_callback=None):
        self.baudrate = baudrate
        self.teensy_a_port = None
        self.teensy_b_port = None
        self.teensy_c_port = None
        self.teensy_a = None
        self.teensy_b = None
        self.teensy_c = None
        self.running = False
        self.sound_callback = sound_callback
        
    def connect(self):
        """Connect to both Teensys using auto-detection"""
        print("🔍 Auto-detecting Teensy devices...")
        ports = detect_all_teensys()
        
        if 'teensy_a' not in ports:
            print(f"❌ Teensy A (SER={TEENSY_A_SERIAL}) not found!")
            return False
        if 'teensy_b' not in ports:
            print(f"❌ Teensy B (SER={TEENSY_B_SERIAL}) not found!")
            return False
        if 'teensy_c' not in ports:
            print(f"❌ Teensy C (SER={TEENSY_C_SERIAL}) not found!")
            return False
            
        self.teensy_a_port = ports['teensy_a']
        self.teensy_b_port = ports['teensy_b']
        self.teensy_c_port = ports['teensy_c']
        
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

        try:
            self.teensy_c = serial.Serial(self.teensy_c_port, self.baudrate, timeout=0.1)
            print(f"✅ Connected to Teensy C on {self.teensy_c_port}")
        except Exception as e:
            print(f"❌ Failed to connect to Teensy C: {e}")
            if self.teensy_a:
                self.teensy_a.close()
            if self.teensy_b:
                self.teensy_b.close()
            return False
            
        return True
    
    def send_led_command_to_receiver_teensies(self, strip_id=0):
        """Send LED pulse command to Teensy B and C"""
        if not self.teensy_b or not self.teensy_c:
            if not self.teensy_b:
                print("❌ Teensy B not connected")
            if not self.teensy_c:
                print("❌ Teensy C not connected")
            return

        print("Teensy B and C apparently connected...")
            
        packet = create_led_pulse_packet(strip_id)
        packet_bytes = packet.to_bytes()
        
        try:
            # TODO: We should only write to teensy c if strip_id >= 8.
            self.teensy_b.write(packet_bytes)
            self.teensy_c.write(packet_bytes)
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
                            self.send_led_command_to_receiver_teensies(strip_id)
                            
                            # Trigger sound callback if provided
                            if self.sound_callback:
                                self.sound_callback(strip_id)
                        
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
    
    # TODO: Instead make a more general function that takes b/c as arg.
    def monitor_teensy_c(self):
        """Monitor Teensy C for debug output"""
        while self.running:
            if self.teensy_c and self.teensy_c.in_waiting > 0:
                try:
                    line = self.teensy_c.readline().decode('utf-8').strip()
                    if line:
                        current_time = time.strftime("%H:%M:%S")
                        print(f"[{current_time}] 🅱️  Teensy C: {line}")
                except Exception as e:
                    print(f"❌ Error reading Teensy C: {e}")
            
            time.sleep(0.01)
    
    def start_monitoring(self):
        """Start monitoring both Teensys in separate threads"""
        if not self.teensy_a or not self.teensy_b:
            print("❌ Both Teensys must be connected before starting monitoring")
            return False
        
        self.running = True
        
        # Start monitoring threads
        self.thread_a = threading.Thread(target=self.monitor_teensy_a, daemon=True)
        self.thread_b = threading.Thread(target=self.monitor_teensy_b, daemon=True)
        self.thread_c = threading.Thread(target=self.monitor_teensy_c, daemon=True)
        
        self.thread_a.start()
        self.thread_b.start()
        self.thread_c.start()
        
        print("🚀 Started monitoring all Teensys")
        return True
    
    def stop_monitoring(self):
        """Stop monitoring and close connections"""
        print("🛑 Stopping Teensy monitoring...")
        self.running = False
        
        if self.teensy_a:
            self.teensy_a.close()
            self.teensy_a = None
        
        if self.teensy_b:
            self.teensy_b.close()
            self.teensy_b = None

        if self.teensy_c:
            self.teensy_c.close()
            self.teensy_c = None
        
        print("✅ Teensy connections closed")
    
    def __enter__(self):
        """Context manager entry"""
        if self.connect():
            return self
        else:
            raise ConnectionError("Failed to connect to Teensy devices")
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """Context manager exit"""
        self.stop_monitoring()
