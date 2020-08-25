#include <Arduino.h>
#include "RgbModule.h"
#include "main.h"

// Define the array of leds
extern CRGB leds[NUM_LEDS];
extern CRGBArray<NUM_LEDS> ledsTF;
extern CRGB gBackgroundColor;

extern CRGBPalette16 gCurrentPalette;
extern CRGBPalette16 gTargetPalette;
extern TBlendType    currentBlending;

extern uint8_t gCurrentPatternNumber; 
extern uint8_t gHue; 

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

bool gReverseDirection = false;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
//#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(gCurrentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePaletteToWeather(weather p_weather)
{
    //Change the pallette according to Main Description
    if(p_weather.mainW = "Clouds")
    {
        gCurrentPalette = CloudColors_p;
        currentBlending = LINEARBLEND;
    }
    else
    {
        gCurrentPalette = ForestColors_p;
        currentBlending = NOBLEND;
    }

    //Change the hue according to temperature

    //Change the speed according to pressure?
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) 
    {
        lastSecond = secondHand;
        if( secondHand ==  0)  { gCurrentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { gCurrentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { gCurrentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { gCurrentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { gCurrentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { gCurrentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { gCurrentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        gCurrentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid(gCurrentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    gCurrentPalette[0] = CRGB::White;
    gCurrentPalette[4] = CRGB::White;
    gCurrentPalette[8] = CRGB::White;
    gCurrentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    gCurrentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}



