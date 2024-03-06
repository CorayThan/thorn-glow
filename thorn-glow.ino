/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 48

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3

#define MAX_GLOWS 4

// Define the array of leds
CRGB leds[NUM_LEDS];

struct Glow{
  bool active;
  bool increment;
  CHSV color;
  int startIdx;
  int size;
  int currentIntensity;
  int maxIntensity;
  int speed;
  int interval;
};

Glow activeGlows[MAX_GLOWS] = {
  {
    false,
    true,
    CHSV(165, 200, 255),
    0,
    0,
    0,
    0,
    0,
    0
  },
  {
    false,
    true,
    CHSV(165, 200, 255),
    0,
    0,
    0,
    0,
    0,
    0
  },
  {
    false,
    true,
    CHSV(165, 200, 255),
    0,
    0,
    0,
    0,
    0,
    0
  },
  {
    false,
    true,
    CHSV(165, 200, 255),
    0,
    0,
    0,
    0,
    0,
    0
  },
};

void printGlow(Glow* glow) {
  // Serial.print("******");
  // Serial.print("Glow active: ");
  // if (glow->active) {
  //   Serial.print("Glow active true. ");
  // } else {
  //   Serial.print("Glow active false. ");
  // }
  // Serial.print("Glow start LED is: ");
  // Serial.print(glow->startIdx);
  // Serial.print(" and size ");
  // Serial.print(glow->size);
  // Serial.print(" color: h,s,v");
  // Serial.print(glow->color.hue);
  // Serial.print(",");
  // Serial.print(glow->color.saturation);
  // Serial.print(",");
  // Serial.print(glow->color.value);
  // Serial.print(" intensity: ");
  // Serial.print(glow->currentIntensity);
  // Serial.print(" max intensity: ");
  // Serial.print(glow->maxIntensity);
  // Serial.print(" interval: ");
  // Serial.print(glow->interval);
  // Serial.print(" speed: ");
  // Serial.print(glow->speed);
  // Serial.println("******");
}

bool validGlow(int testGlowStart, int testGlowSize) {

  for (int idx = 0; idx < MAX_GLOWS; idx++) {
    Glow* glow = &activeGlows[idx];
    // Serial.print("Found glow at index ");
    // Serial.println(idx);
    // printGlow(glow);
    if (glow->active) {
      int activeGlowStart = glow->startIdx;
      int activeGlowEnd = glow->startIdx + glow->size - 1;

      int testGlowEnd = testGlowStart + testGlowSize - 1;
      if ((testGlowStart >= activeGlowStart && testGlowStart <= activeGlowEnd) ||
        (testGlowEnd >= activeGlowStart && testGlowEnd <= activeGlowEnd)) {

        // Serial.print("Glow is invalid. Found glow with start ");
        // Serial.print(activeGlowStart);
        // Serial.print(" and end ");
        // Serial.print(activeGlowEnd);
        // Serial.println("");

        return false;
      }
    }
  }

  // Serial.println("Glow is valid.");
  return true;
}

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  randomSeed(analogRead(0));
  FastLED.setBrightness(100);
}

void loop() { 

  CHSV chsvColors[6] = {
    CHSV(140, 255, 255), // Aqua
    CHSV(155, 150, 255), // Sky
    CHSV(160, 255, 255), // Blue
    CHSV(165, 200, 255), // Faded Blue
    CHSV(175, 150, 255), // Amethyst
    CHSV(180, 255, 255), // Violet
  };

  int currentActiveGlows = 0;
  int emptyGlowIdx = -1;

  // Check glow counts
  for (int idx = 0; idx < MAX_GLOWS; idx++) {
    Glow glow = activeGlows[idx];
    if (glow.active) {
      currentActiveGlows++;
    } else {
      emptyGlowIdx = idx;
    }
  }

  if (currentActiveGlows < MAX_GLOWS) {
    
    // Attempt making a new glow
    // Serial.println("Make a new glow?");
    int glowSize;

    int sizeRand = random(100);
    if (sizeRand < 10) {
      // Short glow
      glowSize = random(7, 12);
    } else if (sizeRand < 40) {
      // Medium glow
      glowSize = random(12, 16);
    } else {
      // Long glow
      glowSize = random(17, 24);
    }

    int glowStart = random(0, NUM_LEDS - glowSize);

    int glowChance = random(100);

    if (glowChance < 20 && validGlow(glowStart, glowSize)) {
      // Serial.println("Glow is valid let's really make it!");
      // Serial.print("Potential glow was valid add a glow at glow index: ");
      // Serial.println(emptyGlowIdx);
      Glow* updateGlow = &activeGlows[emptyGlowIdx];
      updateGlow->active = true;
      updateGlow->color = chsvColors[random(6)];
      updateGlow->startIdx = glowStart;
      updateGlow->size = glowSize;

      int speed;
      int speedRand = random(100);
      if (speedRand < 15) {
        // Medium glow
        speed = random(10, 15);
      } else {
        // Long glow
        speed = random(16, 60);
      }

      updateGlow->speed = speed;
      updateGlow->currentIntensity = 0;
      updateGlow->maxIntensity = random(150, 255);
      updateGlow->increment = true;

      // Serial.print("Updated glow to be: ");
      // printGlow(updateGlow);
    }
  }

  // Process active glows
  for (int idx = 0; idx < MAX_GLOWS; idx++) {
    Glow* glow = &activeGlows[idx];
   
    // Serial.print("Processing active glows glow initial values: ");
    // printGlow(glow);
   
    if (glow->active) {
      // Serial.print("Glow #");
      // Serial.print(idx);
      // Serial.println(" is active.");
      if (glow->interval <= glow->speed) {
        // Serial.println("Increment interval");
        glow->interval++;
        if (glow->increment) {
          // Increase glow
          // Serial.println("Increment intensity.");
          glow->currentIntensity += 1;
          if (glow->currentIntensity >= glow->maxIntensity) {
            glow->increment = false;
          }
        } else if (glow->currentIntensity > 0) {
          // Decrease glow
          // Serial.println("Decrement intensity.");
          glow->currentIntensity -= 1;
        } else {
          // Glow now inactive
          // Serial.println("Glow inactive.");
          glow->active = false;
        }
      } else {
        // Serial.println("Reset interval.");
        glow->interval = 1;
      }

      // Serial.print("Processing active glows changed active glow to: ");
      printGlow(glow);

      // Turn on nodes as appropriate
      for (int ledNum = glow->startIdx; ledNum < glow->startIdx + glow->size; ledNum++) {
        CHSV* glowColor = &glow->color;
        glowColor->value = glow->currentIntensity;
        int brightness = glowColor->value;
        if (ledNum == glow->startIdx || ledNum == glow->startIdx + glow->size - 1) {
          // first or last
          brightness = int (brightness / 4);
        } else if (ledNum == glow->startIdx + 1 || ledNum == glow->startIdx + glow->size - 2) {
          // second or second to last
          brightness = int (brightness / 2.5);
        } else if (ledNum == glow->startIdx + 2 || ledNum == glow->startIdx + glow->size - 3) {
          // third or third to last
          brightness = int (brightness / 1.5);
        }
        leds[ledNum] = CHSV(glowColor->hue, glowColor->saturation, brightness);
        // leds[ledNum] = CHSV(140, 255, random(0, 255));
        // Serial.print("Setting LED #");
        // Serial.print(ledNum);
        // Serial.print(" to be hue,saturation,brightness: ");
        // Serial.print(glowColor->hue);
        // Serial.print(",");
        // Serial.print(glowColor->saturation);
        // Serial.print(",");
        // Serial.print(glowColor->value);
        // Serial.println("");
      }
    }
  }

  FastLED.show();
  delay(5);
}
