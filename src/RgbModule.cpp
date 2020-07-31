#include <Arduino.h>
#include "main.h"
#include "RgbModule.h"

// Define the array of leds
extern CRGB leds[NUM_LEDS];
extern CRGBArray<NUM_LEDS> ledsTF;

extern CRGBPalette16 gCurrentPalette;
extern CRGBPalette16 gTargetPalette;

extern uint8_t gCurrentPatternNumber; 
extern uint8_t gHue; 
extern SimplePatternList gPatterns;

/* **************nextPattern******************************************************
Updates the pattern to be used
*/
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

/* **************rainbow******************************************************
FastLED's built-in rainbow generator
*/
void rainbow() 
{
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

/* **************rainbowWithGlitter******************************************************
built-in FastLED rainbow, plus some random sparkly glitter
*/
void rainbowWithGlitter() 
{
  rainbow();
  addGlitter(80);
}

/* **************addGlitter******************************************************
Add a glitter effect but do so with a random chance
*/
void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

/* **************confetti******************************************************
Random colored speckles that blink in and fade smoothly
*/
void confetti() 
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

/* **************sinelon******************************************************
A colored dot sweeping back and forth, with fading trails
*/
void sinelon()
{
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

/* **************bpm******************************************************
Colored stripes pulsing at a defined Beats-Per-Minute (BPM)
*/
void bpm()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

/* **************juggle******************************************************
Eight colored dots, weaving in and out of sync with each other
*/
void juggle() {
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
