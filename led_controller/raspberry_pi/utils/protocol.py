#!/usr/bin/env python3
"""
Teensy communication protocol definitions

Contains the shared CommandPacket class and protocol constants used for
communication between Raspberry Pi and Teensy devices.
"""

import struct
from .config import (
    CMD_LED_PULSE,
    CMD_LED_EFFECT,
    CMD_BUTTON_PRESS,
    CMD_BUTTON_LED,
    CMD_SENSOR_DATA,
    CMD_HEARTBEAT,
    NUM_STRIPS_PER_TEENSY,
)


class CommandPacket:
    """
    Binary command packet for Teensy communication
    
    This packet format is used for all communication with Teensy devices.
    The packet consists of:
    - command (1 byte): Command type
    - data_length (1 byte): Number of data bytes
    - data (32 bytes): Command data (padded with zeros)
    - checksum (1 byte): XOR checksum
    """
    
    def __init__(self, command=0, data_length=0, data=None, checksum=0):
        self.command = command
        self.data_length = data_length
        self.data = data if data is not None else [0] * 32
        self.checksum = checksum
    
    def calculate_checksum(self):
        """Calculate XOR checksum for the packet"""
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
    
    @classmethod
    def from_bytes(cls, packet_bytes):
        """Create CommandPacket from received bytes"""
        if len(packet_bytes) != 35:  # 1 + 1 + 32 + 1
            raise ValueError(f"Invalid packet length: {len(packet_bytes)}")
        
        command, data_length, data_bytes, checksum = struct.unpack('BB32sB', packet_bytes)
        data = list(data_bytes[:data_length])
        
        packet = cls(command, data_length, data, checksum)
        
        # Verify checksum
        expected_checksum = packet.calculate_checksum()
        if checksum != expected_checksum:
            raise ValueError(f"Checksum mismatch: expected {expected_checksum}, got {checksum}")
        
        return packet
    
    def __str__(self):
        """String representation for debugging"""
        return f"CommandPacket(cmd=0x{self.command:02x}, len={self.data_length}, data={self.data[:self.data_length]})"

# Safety function to make sure we are always sending strip indices with values [0-7].
# Committing to this standard this on the raspberry pi allows us to avoid doing post-unpack checks on the Teensy side.
def get_validated_strip_id(strip_id):
    return strip_id % NUM_STRIPS_PER_TEENSY

# Convenience functions for creating common packets
def create_led_pulse_packet(strip_id):
    """Create a packet to pulse a specific LED strip"""
    return CommandPacket(CMD_LED_PULSE, 1, [get_validated_strip_id(strip_id)])

def create_led_effect_packet(strip_id, effect_type, *params):
    """Create a packet for LED effects"""
    data = [get_validated_strip_id(strip_id), effect_type] + list(params)
    return CommandPacket(CMD_LED_EFFECT, len(data), data)

# TODO: Is this needed?
def create_button_led_packet(button_id, r, g, b):
    """Create a packet to set button LED color"""
    return CommandPacket(CMD_BUTTON_LED, 4, [button_id, r, g, b])
