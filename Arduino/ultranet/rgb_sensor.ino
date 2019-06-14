#include <Wire.h>
#include <Adafruit_TCS34725.h>

#define ANALOG_MAX_RESOLUTION_BITS  10
/*#define COMMON_ANODE
#define RAW_UPPER_VALUE 511*/

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);

// our RGB -> eye-recognized gamma color
byte gammatable[256];
int analog_max_level = pow(2, ANALOG_MAX_RESOLUTION_BITS) - 1;

extern const uint8_t tcs34725_red_pin;
extern const uint8_t tcs34725_green_pin;
extern const uint8_t tcs34725_blue_pin;

void setup_rgb_sensor() {
  pinMode(tcs34725_red_pin, OUTPUT);
  pinMode(tcs34725_green_pin, OUTPUT);
  pinMode(tcs34725_blue_pin, OUTPUT);
   
  if (tcs.begin()) {
  Serial.println("Found TCS34725 sensor!");
  } else {
  Serial.println("No TCS34725 found ... check your connections");
  while (1);
  }

  tcs.setInterrupt(true);  // turn off LED

  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

#ifdef COMMON_ANODE
    gammatable[i] = 255 - x;
#else
    gammatable[i] = x;
#endif
  }
}

void rgb_tune_colors(int range, float &red, float &green, float &blue) {
  if (range > 256) {
    blue -= 50;
    green -= 10;
  }
}

void rgb_set_color(float redValue, float greenValue, float blueValue) {
  int sub = 0, range = 255;
  float max_val = max(redValue, greenValue);
  max_val = max(max_val, blueValue);

  /* correction */
#ifdef RAW_UPPER_VALUE
  range = RAW_UPPER_VALUE;
  rgb_tune_colors(&redValue, &greenValue, &blueValue);
  
  if (max_val > RAW_UPPER_VALUE) {
    sub = max_val-sub;
    redValue = min(redValue - sub, 0);
    greenValue = min(greenValue - sub, 0);
    blueValue = min(blueValue - sub, 0);
  }

  #ifdef COMMON_ANODE
    redValue = RAW_UPPER_VALUE - redValue;
    greenValue = RAW_UPPER_VALUE - greenValue;
    blueValue = RAW_UPPER_VALUE - blueValue;
  #endif
#else
    redValue = gammatable[(int)redValue];
    blueValue = gammatable[(int)blueValue];
    greenValue = gammatable[(int)greenValue];
#endif

  analogWrite(tcs34725_red_pin, map(redValue, 0, range, 0, analog_max_level));
  analogWrite(tcs34725_green_pin, map(greenValue, 0, range, 0, analog_max_level));
  analogWrite(tcs34725_blue_pin, map(blueValue, 0, range, 0, analog_max_level));
}

void rgb_read_and_display() {
  float red, green, blue;
  /* atleast 60ms interval required between reads */
  /* If using tcs.getRawData(), use along with RAW_UPPER_VALUE */
  tcs.getRGB(&red, &green, &blue);
  rgb_set_color(red, green, blue);
}
