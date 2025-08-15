#!/usr/bin/env python3
"""
Simple script runner that handles imports correctly
Usage: python run.py test/test_both_teensys.py
"""
import sys
import runpy

if len(sys.argv) != 2:
    print("Usage: python run.py <script_path>")
    print("Example: python run.py test/test_both_teensys.py")
    sys.exit(1)

script_path = sys.argv[1]
# Remove .py extension if present and convert / to .
module_name = script_path.replace('.py', '').replace('/', '.')

try:
    runpy.run_module(module_name, run_name='__main__')
except ModuleNotFoundError as e:
    print(f"Could not run {script_path}: {e}")
    print("Make sure the script exists and uses proper imports")
