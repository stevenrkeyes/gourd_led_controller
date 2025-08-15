#!/bin/sh

echo "--- JACK Audio Server Restart ---"

# Check if a jackd process is currently running.
# pgrep returns 0 if a process is found, 1 if not.
if pgrep -x "jackd" >/dev/null; then
  echo "Found a running jackd instance. Terminating it now..."
  # killall sends a signal to all processes with the given name.
  # The -w flag makes it wait until the processes have terminated.
  killall -w jackd
  echo "jackd process terminated."
else
  echo "No running jackd process found. Proceeding to start a new one."
fi

# Also check for alsa_out processes
if pgrep -x "alsa_out" >/dev/null; then
  echo "Found running alsa_out processes. Terminating them now..."
  killall -w alsa_out
  echo "alsa_out processes terminated."
fi

# A brief pause to ensure system resources are freed up.
sleep 1

echo "Starting a new jackd process in the background..."

# Use ALSA with the Headphones device by name instead of hardware spec
# This avoids the parsing issues with hw:X,Y format
JACK_NO_AUDIO_RESERVATION=1 jackd -d alsa -d "bcm2835 Headphones" -r 44100 -p 1024 -C 0 -P 2 &

# Give jackd more time to fully initialize and be ready for connections
echo "Waiting for JACK to be ready..."
sleep 5

# Test if JACK is responding
if jack_control status >/dev/null 2>&1; then
  echo "✅ JACK is responding and ready for connections"
else
  echo "❌ JACK is not responding, waiting a bit more..."
  sleep 3
fi

# Final check to confirm the new process has started.
if pgrep -x "jackd" >/dev/null; then
  echo "✅ Success! JACK server is now running with PID: $(pgrep -x "jackd")"
else
  echo "❌ Error: Failed to start jackd. Check your parameters or system logs for more info."
  exit 1
fi

echo "-------------------------------"
echo "Starting Python script with JACK audio backend..."

# Change to the raspberry_pi directory so imports work correctly
cd /home/gourd/gourd/gourd_led_controller/led_controller/raspberry_pi

# Run the Python script using the virtual environment
.venv/bin/python sound/main.py

