#ifndef CONFIG_H
#define CONFIG_H

// Common configuration for all Teensys
#define LED_STRIP_NUM_LEDS 50
#define SERIAL_BAUD_RATE 9600
#define LOOP_DELAY_MS 2

// Communication protocol
#define CMD_LED_PULSE 0x01
#define CMD_LED_EFFECT 0x02
#define CMD_BUTTON_PRESS 0x10
#define CMD_BUTTON_LED 0x11
#define CMD_SENSOR_DATA 0x20
#define CMD_RING_LED_TEST 0x30  // Add this new command
#define CMD_HEARTBEAT 0xFF

// LED configuration
#define LED_CHIPSET WS2811
#define LED_COLOR_ORDER WGRB

// Teensy-specific configurations
#ifdef TEENSY_A
  #define HAS_SENSORS
#endif

#ifdef TEENSY_B
  #define NUM_LED_STRIPS 8
#endif

#ifdef TEENSY_C
  #define NUM_LED_STRIPS 8
#endif

#endif // CONFIG_H 