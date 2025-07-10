#define LOOP_DELAY_MS 2

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
}

void loop() {
  delay(LOOP_DELAY_MS);
}
