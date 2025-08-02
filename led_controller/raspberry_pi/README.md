# Raspberry Pi - Teensy Communication Test

This directory contains test scripts for communicating with the Teensy controllers.

## Setup

1. **Install Python dependencies:**
   ```bash
   pip3 install -r requirements.txt
   ```

2. **Connect Teensy A to Raspberry Pi via USB**

3. **Upload Teensy A firmware:**
   ```bash
   # On your development machine with Teensy A connected:
   pio run -e teensy_a --target upload
   ```

## Testing Button Communication

1. **Run the test script on your Raspberry Pi:**
   ```bash
   python3 test_teensy_communication.py
   ```

   Or specify a specific port:
   ```bash
   python3 test_teensy_communication.py /dev/ttyUSB0
   ```

2. **What you should see:**
   - Connection confirmation
   - Heartbeat messages every second
   - Debug messages from Teensy A

3. **Test button presses:**
   - Press button connected to pin 0 on Teensy A
   - You should see: `🔘 BUTTON PRESSED: Button 1`
   - The script will automatically send LED color commands back to Teensy A

4. **Test sensor data:**
   - The script will display analog sensor readings from pin A0
   - Digital sensor readings from pin 8

## Expected Output

```
Teensy A Communication Test
========================================
Found potential Teensy port: /dev/ttyACM0 - USB Serial Device
Connected to Teensy A on /dev/ttyACM0 at 9600 baud
Starting Teensy A monitor...
Press Ctrl+C to stop
--------------------------------------------------
Teensy Debug: Starting Teensy A - Input Controller with Button LEDs
Teensy Debug: Communication initialized
Teensy Debug: Buttons initialized
Teensy Debug: Button LEDs initialized
Teensy Debug: Initializing Teensy A specific features
Teensy Debug: Setup complete
[14:30:15] 💓 HEARTBEAT: 1523ms
[14:30:16] 💓 HEARTBEAT: 2523ms
[14:30:17] 🔘 BUTTON PRESSED: Button 1
Sent button LED command: Button 1 -> RGB(255,0,0)
[14:30:17] 📊 SENSOR DATA: ID=1, Analog=512, Digital=1
```

## Troubleshooting

1. **Port not found:**
   - Check `ls /dev/tty*` for available ports
   - Try `/dev/ttyUSB0`, `/dev/ttyACM0`, or `/dev/ttyACM1`

2. **Permission denied:**
   ```bash
   sudo usermod -a -G dialout $USER
   # Then logout and login again
   ```

3. **No data received:**
   - Verify Teensy A firmware is uploaded correctly
   - Check that button is connected to pin 0
   - Verify baud rate matches (9600)

4. **Checksum errors:**
   - This indicates communication issues
   - Try a different USB cable
   - Check for interference 