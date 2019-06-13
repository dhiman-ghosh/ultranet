#include <Wire.h>
#include <Adafruit_TCS34725.h>

/*#define COMMON_ANODE*/
#define UPPER_VALUE 511

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();
 
/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);

int redPin= D5;
int greenPin = D6;
int bluePin = D7;

void setup_rgb_sensor() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.println(D2);
  Serial.println(D5);
   
  if (tcs.begin()) {
  Serial.println("Found TCS34725 sensor!");
  } else {
  Serial.println("No TCS34725 found ... check your connections");
  while (1);
  }
}

void setColor(int redValue, int greenValue, int blueValue) {
  int sub = 0;
  int max_val = max(redValue, greenValue);
  max_val = max(max_val, blueValue);

  // correction
  blueValue -= 50;
  greenValue -=10;

  if (max_val > UPPER_VALUE) {
      sub = max_val-sub;
      redValue = min(redValue - sub, 0);
      greenValue = min(greenValue - sub, 0);
      blueValue = min(blueValue - sub, 0);
  }
  
  #ifdef COMMON_ANODE
    redValue = UPPER_VALUE - redValue;
    greenValue = UPPER_VALUE - greenValue;
    blueValue = UPPER_VALUE - blueValue;
  #endif

  analogWrite(redPin, map(redValue, 0, UPPER_VALUE, 0, 1023));
  analogWrite(greenPin, map(greenValue, 0, UPPER_VALUE, 0, 1023));
  analogWrite(bluePin, map(blueValue, 0, UPPER_VALUE, 0, 1023));
}

/*void loop() {
  uint16_t r, g, b, c, colorTemp, lux;
 
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
   
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");

  setColor(r, g, b);
  delay(1000);
  
}*/
