/* MAIN */
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <PubSubClient.h>

extern PubSubClient mqtt;

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up Wifi and Internet...");        setup_wifi();
  Serial.println("Setting up OTA firmware update...");      setup_ota();
  Serial.println("Setting up MQTT Communication...");       setup_mqtt();
  // Serial.println("Setting up RGB Sensor (TCS34725)...");    setup_rgb_sensor();
  Serial.println("Setting up NeoPixel (WS2812B)...");       setup_neopixel();
}

void loop() {
  loop_mqtt(); /* handles (a) arq, (b) neopixel */
  loop_ota();
  // EVERY_N_MILLISECONDS(250) { rgb_read_and_display(); }
}
