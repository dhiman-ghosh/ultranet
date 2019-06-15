#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 132
#define LED_VOLTS 5
#define FASTLED_HAS_CLOCKLESS

extern const uint8_t ws2812b_data_pin;
extern const uint32_t ws2812b_max_power;
extern const uint8_t ws2812b_brightness;

CRGB leds[NUM_LEDS];
uint8_t data[ NUM_LEDS];

enum Color {
    red = 0,     blue = 1,      green = 2,    aqua = 3,      yellow = 4,    pink = 5,    white = 6,    
    black = 7,   gold = 8,      orange = 9,   magenta = 10,  maroon = 11,   violet = 12,
    india = 13,  pakistan = 14, rainbow = 15
};

void setup_neopixel() {
  FastLED.addLeds<WS2812B, ws2812b_data_pin, EOrder::GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(ws2812b_brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTS, ws2812b_max_power);
  //fill_solid(leds, NUM_LEDS, CRGB::Blue);
  displayLEDPattern(13);
  //FastLED.show();
  Serial.println("WS2812B Setup Completed...");
}

void fill_data_array()
{
  static uint8_t startValue = 0;
  startValue = startValue + 2;
  
  uint8_t value = startValue;  
  for( int i = 0; i < NUM_LEDS; i++) {
    data[i] = triwave8( value); // convert value to an up-and-down wave
    value += 7;
  }
}


CRGBPalette16 gPaletteInd (
    CRGB::Black, 
    CRGB::DarkOrange, CRGB::DarkOrange,  CRGB::DarkOrange,  CRGB::DarkOrange, CRGB::DarkOrange,
    CRGB::White,  CRGB::White, CRGB::White, CRGB::White,  CRGB::White,
    CRGB::Green,  CRGB::Green, CRGB::Green, CRGB::Green,
    CRGB::Black
);

CRGBPalette16 gPalettePak (
    CRGB::Black,
    CRGB::White,  CRGB::White, CRGB::White, CRGB::White,  CRGB::White,
    CRGB::Green,  CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green,
    CRGB::Green,  CRGB::Green, CRGB::Green, CRGB::Green,
    CRGB::Black
);
void render_data_with_palette(char pattern)
{
  switch(pattern) {
    case 'i': {
      for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( gPaletteInd, data[i], 128, LINEARBLEND);
      }
      break;
    }
    case 'p': {
      for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( gPalettePak, data[i], 128, LINEARBLEND);
      }
      break;
    }
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

void colorWave (int SpeedDelay, char pattern) {
  byte *c;
  uint16_t i,j = 0;
  int right_dir = 1;
  for(j=0; j<256; j++) {
    for(i=0; i<= NUM_LEDS; i++) {
      c=Wheel((((i * 256 / NUM_LEDS) + j) & 255), right_dir, pattern);
      if (right_dir) {
        setPixel(i, *c, *(c+1), *(c+2));
      } else {
        setPixel(NUM_LEDS - i, *c, *(c+1), *(c+2));
      }
    }
    FastLED.show();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos, int dir, char pattern) {
  static byte r[3];
  if (dir == 1) {
    if(WheelPos < 85) {
      switch (pattern) {
        case 'i': 
        case 'p': {
          static byte c[3] = {0x00, 0x80, 0x00};
          return c;
        }
        case 'r': {
          r[0]=WheelPos * 3;
          r[1]=255 - WheelPos * 3;
          r[2]=0;
          return r;
        }
      }
    } else if(WheelPos < 170) {
     WheelPos -= 85;
     switch (pattern) {
      case 'i': {
       static byte c[3] = {0xff, 0xff, 0xff};
       return c;
      }
      case 'p': {
        static byte c[3] = {0x00, 0x80, 0x00};
        return c;
      }
      case 'r': {
        r[0]=255 - WheelPos * 3;
        r[1]=0;
        r[2]=WheelPos * 3;
        return r;
      }
     }
    } else {
     WheelPos -= 170;
      switch (pattern) {
       case 'i': {
        static byte c[3] = {0xff, 0x8c, 0x00};
        return c;
       }
       case 'p': {
        static byte c[3] = {0xff, 0xff, 0xff};
        return c;
      }
      case 'r': {
        r[0]=0;
        r[1]=WheelPos * 3;
        r[2]=255 - WheelPos * 3;
        return r;
      }
     }
    }
  } else {
    if(WheelPos < 85) {
     switch (pattern) {
       case 'i': {
        static byte c[3] = {0xff, 0x8c, 0x00};
        return c;
       }
       case 'p': {
        static byte c[3] = {0xff, 0xff, 0xff};
        return c;
      }
      case 'r': {
        r[0]=0;
        r[1]=WheelPos * 3;
        r[2]=255 - WheelPos * 3;
        return r;
      }
     }
    } else if(WheelPos < 170) {
     WheelPos -= 85;
     switch (pattern) {
      case 'i': {
       static byte c[3] = {0xff, 0xff, 0xff};
       return c;
      }
      case 'p': {
        static byte c[3] = {0x00, 0x80, 0x00};
        return c;
      }
      case 'r': {
        r[0]=255 - WheelPos * 3;
        r[1]=0;
        r[2]=WheelPos * 3;
        return r;
      }
     }
    } else {
     WheelPos -= 170;
     switch (pattern) {
        case 'i': 
        case 'p': {
          static byte c[3] = {0x00, 0x80, 0x00};
          return c;
        }
        case 'r': {
          r[0]=WheelPos * 3;
          r[1]=255 - WheelPos * 3;
          r[2]=0;
          return r;
        }
      }
    }
  }
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      FastLED.show();
      delay(SpeedDelay);
  }
}

void displayLEDPattern(int pattern) {
  switch (pattern) {
    case 0: {
      // CRGB::Red
      colorWipe(0x00,0x00,0xff, 10);
      break;
    }
    case 1: {
      // CRGB::Blue
      colorWipe(0x00,0x00,0xff, 10);
      break;
    }
    case 2: {
      // CRGB::Green
      colorWipe(0x00,0x80,0x00, 10);
      break;
    }
    case 3: {
      // CRGB::Aqua
      colorWipe(0x00,0xff,0xff, 10);
      break;
    }
    case 4: {
      // CRGB::Yellow
      colorWipe(0xff,0xff,0x00, 10);
      break;
    }
    case 5: {
      // CRGB::DeepPink
      colorWipe(0xff,0x14,0x93, 10);
      //leds[i] = CRGB::Pink;
      break;
    }
    case 6: {
      // CRGB::White
      colorWipe(0xff,0xff,0xff, 10);
      break;
    }
    case 7: {
      // CRGB::LIME
      colorWipe(0x00,0xff,0x00, 10);
      break;
    }
    case 8: {
      // CRGB::Gold
      colorWipe(0xff,0xd7,0x00, 10);
      break;
    }
    case 9: {
      // CRGB::DeepOrange
      colorWipe(0xff,0x8c,0x00, 10);
      break;
    }
    case 10: {
      // CRGB::Magenta
      colorWipe(0xff,0x00,0xff, 10);
      break;
    }
    case 11: {
      // CRGB::Maroon
      colorWipe(0x80,0x00,0x00, 10);
      break;
    }
    case 12: {
      // CRGB::DarkViolet
      colorWipe(0x94,0x00,0xD3, 10);
      break;
    }
    case 13: {
      fill_data_array();
      render_data_with_palette('i');   
      // colorWave(10, 'i');
      break;
    }
    case 14: {
      fill_data_array();
      render_data_with_palette('p');   
      // colorWave(10, 'p');
      break;
    }
    case 15: {
       Serial.println("Setting it to rainbow...");
       colorWave(10, 'r');
      break;
    }
    default:
    Serial.println("Setting it to black...");
      colorWipe(0x00,0x00,0x00, 10);
      //fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
  }
  FastLED.show();
}
