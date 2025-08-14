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

# A brief pause to ensure system resources are freed up.
sleep 1

echo "Starting a new jackd process in the background..."

# Use the dummy driver to avoid ALSA device parsing issues
# This creates a virtual audio device that we can route to ALSA later if needed
JACK_NO_AUDIO_RESERVATION=1 jackd -d dummy -r 44100 -p 1024 -C 0 -P 2 &

# Give jackd a couple of seconds to initialize.
sleep 2

# Final check to confirm the new process has started.
if pgrep -x "jackd" >/dev/null; then
  echo "✅ Success! JACK server is now running with PID: $(pgrep -x "jackd")"
else
  echo "❌ Error: Failed to start jackd. Check your parameters or system logs for more info."
  exit 1
fi

echo "-------------------------------"
echo "Starting Python script with JACK audio backend..."

# Run the Python script using uv to ensure it uses the virtual environment
uv run python main.py
