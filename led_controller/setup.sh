#!/bin/bash
set -e

echo "🎃 Setting up Gourd LED Controller development environment..."

# Check if running as root
if [ "$EUID" -eq 0 ]; then
    echo "❌ Don't run this script as root/sudo"
    echo "   The script will ask for sudo when needed"
    exit 1
fi

# Install Teensy UDEV rules
echo "📡 Installing Teensy UDEV rules..."
if [ ! -f "00-teensy.rules" ]; then
    echo "❌ 00-teensy.rules not found in project directory"
    exit 1
fi

sudo cp 00-teensy.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger

# Add user to dialout group
echo "👤 Adding user to dialout group..."
sudo usermod -a -G dialout $USER

# Check if PlatformIO is installed
if ! command -v pio &> /dev/null; then
    echo "🔧 PlatformIO not found. Installing..."
    curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
    python3 get-platformio.py
    rm get-platformio.py
    echo "   Note: You may need to restart your shell or run 'source ~/.bashrc'"
fi

echo "✅ Setup complete!"
echo ""
echo "⚠️  IMPORTANT: You need to log out and log back in (or reboot)"
echo "    for the group changes to take effect."
echo ""
echo "🚀 After restarting, you can build and flash with:"
echo "    pio run -e teensy_a -t upload"