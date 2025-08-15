"""
Shared utilities for the LED Controller project.

This module provides centralized access to:
- Device detection and communication (device_utils)
- Communication protocols (protocol)
- Configuration settings (config)
- Dual Teensy management (dual_teensy)
"""

# Re-export commonly used items for convenience
from .config import *
from .device_utils import find_teensy, detect_all_teensys, print_available_ports
from .protocol import CommandPacket, create_led_pulse_packet, create_button_led_packet
from .dual_teensy import DualTeensyTester

__all__ = [
    # From config
    'TEENSY_A_SERIAL', 'TEENSY_B_SERIAL', 'TEENSY_MAPPING',
    'CMD_LED_PULSE', 'CMD_LED_EFFECT', 'CMD_BUTTON_PRESS', 
    'CMD_BUTTON_LED', 'CMD_SENSOR_DATA', 'CMD_HEARTBEAT',
    
    # From device_utils
    'find_teensy', 'detect_all_teensys', 'print_available_ports',
    
    # From protocol
    'CommandPacket', 'create_led_pulse_packet', 'create_button_led_packet',
    
    # From dual_teensy
    'DualTeensyTester',
]
