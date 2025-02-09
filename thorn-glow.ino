/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <FastLED.h>

// When hooking up the Arduino, ensure you're using a power + data cable. The short one is power only!

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

  

  // Serial.println("Glow is valid.");
  return true;
}

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  randomSeed(analogRead(0));
  FastLED.setBrightness(75);
}

void loop() { 

  int colorCount = 7;

  // Blues
  // CHSV chsvColors[colorCount] = {
  //   CHSV(140, 255, 255), // Aqua
  //   CHSV(155, 150, 255), // Sky
  //   CHSV(160, 255, 255), // Blue
  //   CHSV(165, 200, 255), // Faded Blue
  //   CHSV(175, 150, 255), // Amethyst
  //   CHSV(180, 255, 255), // Violet
  // };

  // Purples
  CHSV chsvColors[colorCount] = {
    CHSV(235, 255, 255), // Red Pink
    CHSV(220, 255, 255), // Purple Pink
    CHSV(210, 255, 255), // Mostly Purple
    CHSV(205, 255, 150), // Light Purple
    CHSV(205, 255, 200), // Purple
    CHSV(201, 200, 150), // Amethyst
    CHSV(185, 255, 200), // Violet
  };

  // Pink Purple
  // CHSV chsvColors[colorCount] = {
  //   CHSV(245, 255, 255), // Almost Red
  //   CHSV(240, 255, 255), // Red Pink
  //   CHSV(240, 200, 255), // Light Red Pink
  //   CHSV(230, 255, 255), // Pink
  //   CHSV(230, 200, 255), // Light Pink
  //   CHSV(225, 255, 255), // Purple Pink
  //   CHSV(215, 255, 255), // Light Pink
  //   CHSV(210, 255, 255), // Purple Pink
  // };

    // Pink Orange
  // CHSV chsvColors[colorCount] = {
  //   CHSV(230, 255, 255), // Pink
  //   CHSV(230, 200, 255), // Light Pink
  //   CHSV(240, 255, 255), // Red Pink
  //   CHSV(240, 200, 255), // Light Red Pink
  //   CHSV(20, 255, 255), // Orange
  //   CHSV(10, 225, 255), // Light Orange Red
  // };

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
    if (sizeRand < 20) {
      // Short glow
      glowSize = random(7, 12);
    } else if (sizeRand < 60) {
      // Medium glow
      glowSize = random(12, 16);
    } else {
      // Long glow
      glowSize = random(17, 24);
    }

    int glowStart = random(0, NUM_LEDS - glowSize + 1);

    bool glowValid = true;

    // Serial.println("Potential glow start: ");
    // Serial.print(glowStart);
    // Serial.print(" and size ");
    // Serial.println(glowSize);

    // Check if glow is valid
    for (int idx = 0; idx < MAX_GLOWS && glowValid; idx++) {
      Glow* glow = &activeGlows[idx];

      if (glow->active) {
        int activeGlowStart = glow->startIdx;
        int activeGlowEnd = glow->startIdx + glow->size - 1;

        int testGlowEnd = glowStart + glowSize - 1;

        if (
          // potential glow start overlaps existing glow
          (glowStart >= activeGlowStart && glowStart <= activeGlowEnd) ||
          // potential glow end overlaps existing glow
          (testGlowEnd >= activeGlowStart && testGlowEnd <= activeGlowEnd) ||
          // Glows are equal or potential glow contains active glow
          (glowStart <= activeGlowStart && testGlowEnd >= activeGlowEnd) ||
          // Glows are equal or active glow contains potential glow
          (activeGlowStart <= glowStart && activeGlowEnd >= testGlowEnd) 
          ) {

          glowValid = false;
        } else {
          // Glows do not overlap
          // Serial.print("Non overlapping glow start: ");
          // Serial.print(activeGlowStart);
          // Serial.print(" and end ");
          // Serial.println(activeGlowEnd);
        }
      }
    }
    
    if (glowValid) {
      // Serial.println("Glow is valid let's really make it!");
      // Serial.print("Potential glow was valid add a glow at glow index: ");
      // Serial.println(emptyGlowIdx);
      Glow* updateGlow = &activeGlows[emptyGlowIdx];
      updateGlow->active = true;
      updateGlow->color = chsvColors[random(colorCount)];
      updateGlow->startIdx = glowStart;
      updateGlow->size = glowSize;

      int speed;
      int speedRand = random(100);
      if (speedRand < 20) {
        // Slow glow
        speed = random(1, 3);
      } else if (speedRand < 60) {
        // Medium glow
        speed = random(4, 30);
      } else {
        // Fast glow
        speed = random(31, 175);
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
          glow->currentIntensity++;
          if (glow->currentIntensity >= glow->maxIntensity) {
            glow->increment = false;
          }
        } else if (glow->currentIntensity > 0) {
          // Decrease glow
          // Serial.println("Decrement intensity.");
          glow->currentIntensity--;
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
  delay(4);
}
