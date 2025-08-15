#!/usr/bin/env python3
"""
Central configuration for Teensy LED Controller project
Contains hardware configuration, serial numbers, and communication constants
"""

# Hardware Configuration
# ======================

# Teensy Serial Numbers
TEENSY_A_SERIAL = "14094100"  # Button controller
TEENSY_B_SERIAL = "4278530"   # LED controller
TEENSY_C_SERIAL = "17985960"

# Note: Old Teensy B serial number was "17656680" - kept for reference
# TEENSY_B_SERIAL_OLD = "17656680"  # LED controller (old)

# Teensy Mapping for convenient access
TEENSY_MAPPING = {
    "a": TEENSY_A_SERIAL,
    "b": TEENSY_B_SERIAL,
    "c": TEENSY_C_SERIAL,
}

# Communication Protocol Constants
# ===============================

# Command codes for Teensy communication
CMD_LED_PULSE = 0x01
CMD_LED_EFFECT = 0x02
CMD_BUTTON_PRESS = 0x10
CMD_BUTTON_LED = 0x11
CMD_SENSOR_DATA = 0x20
CMD_HEARTBEAT = 0xFF

# Serial Communication Settings
# ============================

DEFAULT_BAUDRATE = 9600
SERIAL_TIMEOUT = 1.0

# LED Strip Configuration
# ======================

# Pin mapping for LED strips on Teensy B
# Maps strip index to physical pin number
LED_STRIP_PIN_MAPPING = {
    0: 2,
    1: 14,
    2: 7,
    3: 8,
    4: 6,
    5: 20,
    6: 21,
    7: 5
}

# Default LED values
DEFAULT_LED_BRIGHTNESS = 255
DEFAULT_LED_COLOR = (255, 255, 255)  # RGB white

# System Paths
# ============

# Path to platformio executable (can be overridden if needed)
PLATFORMIO_PATH = '/home/gourd/gourd/gourd_led_controller/led_controller/raspberry_pi/.venv/bin/platformio'

# Hardware Detection
# =================

# USB device identifiers for Teensy devices
TEENSY_USB_VID_PID = "16C0:0487"  # Teensy USB vendor:product ID

# Timeout for device detection (seconds)
DEVICE_DETECTION_TIMEOUT = 5.0
