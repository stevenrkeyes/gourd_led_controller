#!/usr/bin/env python3
"""
Centralized utility functions for detecting and connecting to Teensy devices
Provides multiple detection methods for different use cases
"""

import serial
import serial.tools.list_ports
from typing import Optional, Dict, List
from .config import (
    TEENSY_A_SERIAL,
    TEENSY_B_SERIAL,
    TEENSY_MAPPING
)


def find_teensy_by_serial(target_serial: str, verbose: bool = True) -> Optional[str]:
    """
    Find a Teensy device by its serial number using pyserial
    
    Args:
        target_serial: The serial number to search for
        verbose: Whether to print search progress
        
    Returns:
        Device path (e.g., '/dev/ttyACM0') or None if not found
    """
    if verbose:
        print(f"🔍 Searching for Teensy with serial number: {target_serial}")
    
    ports = serial.tools.list_ports.comports()
    
    for port in ports:
        if verbose:
            print(f"   Checking {port.device}: {port.description}")
        
        if port.serial_number == target_serial:
            # Find device name using TEENSY_MAPPING
            device_name = "Unknown Teensy"
            for teensy_id, serial_num in TEENSY_MAPPING.items():
                if serial_num == target_serial:
                    device_name = f"Teensy {teensy_id.upper()}"
                    break
            
            if verbose:
                print(f"   ✅ Found {device_name} at {port.device}")
            return port.device
        
        if verbose and port.serial_number:
            print(f"      Serial: {port.serial_number}")
    
    if verbose:
        print(f"   ❌ Teensy with serial {target_serial} not found")
    return None


def find_teensy(teensy_id: str, verbose: bool = True) -> Optional[str]:
    """
    Find a Teensy device by its ID with built-in error handling
    
    Args:
        teensy_id: Teensy identifier ("a", "b", "c", etc.)
        verbose: Whether to print search progress and errors
        
    Returns:
        Device path or None if not found
    """
    teensy_id = teensy_id.lower()
    
    if teensy_id not in TEENSY_MAPPING:
        if verbose:
            print(f"❌ Unknown Teensy ID: '{teensy_id}'")
            print(f"💡 Available Teensy IDs: {list(TEENSY_MAPPING.keys())}")
        return None

    serial_number = TEENSY_MAPPING[teensy_id]
    print(f"🔍 Looking for Teensy {teensy_id.upper()}...")
    port = find_teensy_by_serial(serial_number, verbose)
    
    if not port and verbose:
        teensy_name = f"Teensy {teensy_id.upper()}"
        print(f"❌ {teensy_name} not found!")
        print(f"💡 Make sure {teensy_name} is connected and has the correct serial number")
    
    return port


# Removed find_teensy_a() and find_teensy_b() - use find_teensy("a") and find_teensy("b") instead


def detect_all_teensys(verbose: bool = True) -> Dict[str, str]:
    """
    Detect all connected Teensy devices using pyserial
    
    Args:
        verbose: Whether to print search progress
        
    Returns:
        Dictionary with detected devices: {'teensy_a': '/dev/ttyACM0', 'teensy_b': '/dev/ttyACM1'}
    """
    ports = {}
    
    if verbose:
        print("🔍 Detecting Teensy devices...")
    
    # Dynamically detect all configured Teensys
    for teensy_id in TEENSY_MAPPING.keys():
        port = find_teensy(teensy_id, verbose)
        if port:
            ports[f'teensy_{teensy_id}'] = port
    
    return ports


def get_all_serial_ports() -> List[Dict[str, str]]:
    """
    Get information about all available serial ports
    Useful for debugging connection issues
    
    Returns:
        List of dictionaries with port information
    """
    ports = []
    for port in serial.tools.list_ports.comports():
        port_info = {
            'device': port.device,
            'description': port.description,
            'serial_number': port.serial_number or 'Unknown'
        }
        if hasattr(port, 'hwid'):
            port_info['hwid'] = port.hwid
        ports.append(port_info)
    return ports


def print_available_ports():
    """Print all available serial ports for debugging"""
    print("\n📋 Available Serial Ports:")
    print("=" * 50)
    
    ports = get_all_serial_ports()
    if not ports:
        print("No serial ports found")
        return
    
    for i, port in enumerate(ports, 1):
        print(f"{i}. Device: {port['device']}")
        print(f"   Description: {port['description']}")
        print(f"   Serial Number: {port['serial_number']}")
        if 'hwid' in port:
            print(f"   Hardware ID: {port['hwid']}")
        print()


if __name__ == "__main__":
    """Run device detection when called directly"""
    print("🔍 Teensy Device Detection Utility")
    print("=" * 40)
    
    print_available_ports()
    
    print("\n🔍 Detecting Teensy devices...")
    detected = detect_all_teensys(verbose=True)
    
    print(f"\n📋 Detection Results:")
    if detected:
        for name, port in detected.items():
            print(f"  {name}: {port}")
    else:
        print("  No Teensy devices detected")
