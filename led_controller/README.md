# LED Controller

An Arduino project for controlling LED strips and eye LEDs using a Teensy 4.1 board.

## Setup with PlatformIO

### Prerequisites

1. Install PlatformIO Core:
   ```bash
   pip install platformio
   ```

2. Install PlatformIO IDE (optional but recommended):
   - VS Code: Install the "PlatformIO IDE" extension
   - Or use PlatformIO Core CLI

### Project Structure

```
led_controller/
├── platformio.ini          # PlatformIO configuration
├── src/
│   └── main.cpp           # Main Arduino sketch
└── README.md              # This file
```

### Building and Uploading

1. **Build the project:**
   ```bash
   pio run
   ```

2. **Upload to Teensy 4.1:**
   ```bash
   pio run --target upload
   ```

3. **Monitor serial output:**
   ```bash
   pio device monitor
   ```

4. **Clean build files:**
   ```bash
   pio run --target clean
   ```

### Hardware Requirements

- Teensy 4.1 board
- WS2811 LED strips
- Push buttons
- Power supply for LEDs

### Pin Configuration

- LED Strip 1: Pin 2
- Eye LEDs: Pin 22
- Button Column 1: Pin 0
- Button Row 1: Pin 1
- Board LED: Pin 13

### Libraries Used

- OctoWS2811: For controlling WS2811 LED strips
- Bounce2: For button debouncing

### Troubleshooting

If you encounter build issues:
1. Make sure PlatformIO is properly installed
2. Check that the Teensy board is connected
3. Verify all dependencies are installed: `pio lib install`
4. Try cleaning and rebuilding: `pio run --target clean && pio run` 