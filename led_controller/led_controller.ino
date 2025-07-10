#include <OctoWS2811.h>
#include <Bounce2.h>

#define LOOP_DELAY_MS 2

constexpr int BOARD_LED_PIN = 13;

constexpr int BUTTON_COLUMN_1_PIN = 0;
constexpr int BUTTON_ROW_1_PIN = 1;

constexpr int LED_STRIP_1_PIN = 2;

// todo: fix the color order
#define LED_CHIPSET WS2811
#define LED_COLOR_ORDER GRBW
constexpr int LED_STRIP_NUM_LEDS = 10;

DMAMEM int displayMemory[LED_STRIP_NUM_LEDS * 4 / 4];
int drawingMemory[LED_STRIP_NUM_LEDS * 4 / 4];
const int config = WS2811_RGBW | WS2811_800kHz;

OctoWS2811 leds(LED_STRIP_NUM_LEDS, displayMemory, drawingMemory, config, 0x02);

Bounce button1(BUTTON_COLUMN_1_PIN, 50);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");

  pinMode(BUTTON_COLUMN_1_PIN, INPUT_PULLUP);

  leds.begin();
  leds.show(); // Clear LEDs to off

  // Set all LEDs to white using the white channel
  for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
    leds.setPixel(i, 0x000000AA); // R=0, G=0, B=0, W=255
  }

  leds.show();

  pinMode(BOARD_LED_PIN, OUTPUT);
}

void loop() {
  // read buttons
  button1.update();
  // send buttons to computer
  if (button1.fell()) {
    Serial.println("button pressed");
  }

  delay(LOOP_DELAY_MS);
}
