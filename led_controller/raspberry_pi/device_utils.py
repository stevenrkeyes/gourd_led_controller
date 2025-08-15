#!/usr/bin/env python3
"""
Centralized utility functions for detecting and connecting to Teensy devices
Provides multiple detection methods for different use cases
"""

import serial
import serial.tools.list_ports
from typing import Optional, Dict, List
from config import (
    TEENSY_A_SERIAL,
    TEENSY_B_SERIAL,
    TEENSY_USB_VID_PID
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
            device_name = "Teensy A" if target_serial == TEENSY_A_SERIAL else "Teensy B"
            if verbose:
                print(f"   ✅ Found {device_name} at {port.device}")
            return port.device
        
        if verbose and port.serial_number:
            print(f"      Serial: {port.serial_number}")
    
    if verbose:
        print(f"   ❌ Teensy with serial {target_serial} not found")
    return None


def find_teensy_a(verbose: bool = True) -> Optional[str]:
    """
    Find Teensy A by its configured serial number
    
    Args:
        verbose: Whether to print search progress
        
    Returns:
        Device path or None if not found
    """
    return find_teensy_by_serial(TEENSY_A_SERIAL, verbose)


def find_teensy_b(verbose: bool = True) -> Optional[str]:
    """
    Find Teensy B by its configured serial number
    
    Args:
        verbose: Whether to print search progress
        
    Returns:
        Device path or None if not found
    """
    return find_teensy_by_serial(TEENSY_B_SERIAL, verbose)


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
    
    teensy_a = find_teensy_a(verbose)
    if teensy_a:
        ports['teensy_a'] = teensy_a
    
    teensy_b = find_teensy_b(verbose)
    if teensy_b:
        ports['teensy_b'] = teensy_b
    
    return ports





def find_any_teensy_port(verbose: bool = True) -> Optional[str]:
    """
    Find any Teensy-like device (fallback for scripts that don't care about specific serial numbers)
    
    Args:
        verbose: Whether to print search progress
        
    Returns:
        Device path or None if not found
    """
    if verbose:
        print("🔍 Looking for any Teensy-like device...")
    
    ports = serial.tools.list_ports.comports()
    for port in ports:
        # Look for common Teensy/USB indicators
        if ('USB' in port.description or 
            'ACM' in port.device or 
            'tty' in port.device or
            TEENSY_USB_VID_PID in str(port.hwid) if hasattr(port, 'hwid') else False):
            
            if verbose:
                print(f"Found potential Teensy port: {port.device} - {port.description}")
            return port.device
    
    if verbose:
        print("❌ No Teensy-like devices found")
    return None


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
