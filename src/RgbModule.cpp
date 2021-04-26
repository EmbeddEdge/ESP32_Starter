#include <Arduino.h>
#include "RgbModule.h"
#include "main.h"

// Gradient palette "wiki_ice_greenland_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/wkp/ice/tn/wiki-ice-greenland.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 48 bytes of program space.

DEFINE_GRADIENT_PALETTE( wiki_ice_greenland_gp ) {
    0, 234,250,252,
   79, 234,250,252,
   79, 222,239,237,
  118, 222,239,237,
  118, 192,223,223,
  158, 192,223,223,
  158, 167,205,203,
  198, 167,205,203,
  198, 148,193,192,
  238, 148,193,192,
  238, 126,182,182,
  255, 126,182,182};

// Gradient palette "white_christmas_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ncdc/tn/white-christmas.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 72 bytes of program space.

DEFINE_GRADIENT_PALETTE( white_christmas_gp ) {
    0, 117, 97, 49,
   12, 117, 97, 49,
   12,  26,173,106,
   25,  26,173,106,
   25,  10, 77,164,
   63,  10, 77,164,
   63,   0, 21, 85,
  102,   0, 21, 85,
  102,  53, 86,135,
  127,  53, 86,135,
  127,  26, 31, 65,
  153,  26, 31, 65,
  153,  61, 31, 96,
  191,  61, 31, 96,
  191,  82, 97, 85,
  229,  82, 97, 85,
  229, 255,255,255,
  255, 255,255,255};

  // Gradient palette "solar_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/cmocean/tn/solar.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 2048 bytes of program space.

DEFINE_GRADIENT_PALETTE( solar_gp ) {
    0,   3,  1,  1,
    0,   3,  1,  1,
    0,   4,  1,  1,
    1,   4,  1,  1,
    1,   4,  1,  1,
    2,   4,  1,  1,
    2,   4,  1,  1,
    3,   4,  1,  1,
    3,   4,  1,  1,
    4,   4,  1,  1,
    4,   5,  1,  1,
    5,   5,  1,  1,
    5,   5,  1,  1,
    6,   5,  1,  1,
    6,   5,  1,  1,
    7,   5,  1,  1,
    7,   6,  1,  1,
    8,   6,  1,  1,
    8,   6,  1,  1,
    9,   6,  1,  1,
    9,   7,  1,  1,
   10,   7,  1,  1,
   10,   7,  1,  1,
   11,   7,  1,  1,
   11,   7,  1,  1,
   12,   7,  1,  1,
   12,   7,  1,  1,
   13,   7,  1,  1,
   13,   8,  1,  1,
   14,   8,  1,  1,
   14,   8,  1,  1,
   15,   8,  1,  1,
   15,   9,  1,  1,
   16,   9,  1,  1,
   16,   9,  1,  1,
   17,   9,  1,  1,
   17,  10,  1,  1,
   30, 177,186,  6,
  231, 177,186,  6,
  231, 177,187,  6,
  232, 177,187,  6,
  232, 177,189,  6,
  233, 177,189,  6,
  233, 179,193,  7,
  234, 179,193,  7,
  234, 179,195,  7,
  235, 179,195,  7,
  235, 179,197,  7,
  236, 179,197,  7,
  236, 179,201,  7,
  237, 179,201,  7,
  237, 179,203,  7,
  238, 179,203,  7,
  238, 179,205,  8,
  239, 179,205,  8,
  239, 179,209,  8,
  240, 179,209,  8,
  240, 182,211,  8,
  241, 182,211,  8,
  241, 182,213,  8,
  242, 182,213,  8,
  242, 182,217,  9,
  243, 182,217,  9,
  243, 182,219,  9,
  244, 182,219,  9,
  244, 182,223,  9,
  245, 182,223,  9,
  245, 182,225,  9,
  246, 182,225,  9,
  246, 182,227,  9,
  247, 182,227,  9,
  247, 182,231, 10,
  248, 182,231, 10,
  248, 182,233, 10,
  249, 182,233, 10,
  249, 182,235, 10,
  250, 182,235, 10,
  250, 182,239, 10,
  251, 182,239, 10,
  251, 182,241, 11,
  252, 182,241, 11,
  252, 182,246, 11,
  253, 182,246, 11,
  253, 184,248, 11,
  254, 184,248, 11,
  254, 184,250, 11,
  255, 184,250, 11};


// Define the array of leds
extern CRGB leds[NUM_LEDS];
extern CRGBArray<NUM_LEDS> ledsTF;
extern CRGB gBackgroundColor;

extern CRGBPalette16 gCurrentPalette;
extern CRGBPalette16 gTargetPalette;
extern TBlendType    currentBlending;

extern uint8_t gCurrentPatternNumber; 
extern uint8_t gHue; 
extern uint16_t g_frames_per_second;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
extern const TProgmemPalette16 myOceanPalette_p PROGMEM;
extern const TProgmemPalette16 myClearPalette_p PROGMEM;
extern const TProgmemPalette16 myCloudPalette_p PROGMEM;
CRGBPalette16 myCloudPalette_gp = white_christmas_gp;

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

void ChangePaletteToWeather(weather* p_weather)
{
    //Change the pallette according to Main Description
    if(p_weather->mainW == "Clouds")
    {
        //SerialMon.print(F("Changing to Clouds\n\r"));
        gCurrentPalette = CloudColors_p;
        currentBlending = LINEARBLEND;
    }
    else if(p_weather->mainW == "Sun")
    {
        //SerialMon.print(F("Changing to Sun\n\r"));
        gCurrentPalette = LavaColors_p;
        currentBlending = NOBLEND;
    }
    else if(p_weather->mainW == "Rain")
    {
        //SerialMon.print(F("Changing to Rain\n\r"));
        gCurrentPalette = OceanColors_p;
        currentBlending = NOBLEND;
    }
    else if(p_weather->mainW == "Clear")
    {
        //SerialMon.print(F("Changing to Clear\n\r"));
        gCurrentPalette = myClearPalette_p;
        currentBlending = NOBLEND;
    }
    else if(p_weather->mainW == "Mist")
    {
        //SerialMon.print(F("Changing to Mist\n\r"));
        gCurrentPalette = CloudColors_p;
        currentBlending = NOBLEND;
    }
    else
    {
        gCurrentPalette = PartyColors_p;
        currentBlending = NOBLEND;
    }
    //SerialMon.print(F("Main Weather setting: "));
    //SerialMon.println(p_weather.mainW);
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

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};


const TProgmemPalette16 myOceanPalette_p PROGMEM =
{
    CRGB::Blue,
    CRGB::Blue, // 'white' is too bright compared to red and blue
    CRGB::DeepSkyBlue,
    CRGB::DeepSkyBlue,
    
    CRGB::Blue,
    CRGB::Blue, 
    CRGB::DeepSkyBlue,
    CRGB::DeepSkyBlue,
    CRGB::Gray,
    CRGB::Gray, 

    CRGB::Blue,
    CRGB::Blue,
    CRGB::DeepSkyBlue,
    CRGB::DeepSkyBlue, 
    CRGB::Gray,
    CRGB::Gray,
};

const TProgmemPalette16 myCloudPalette_p PROGMEM =
{
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
};

const TProgmemPalette16 mySunPalette_p PROGMEM =
{
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
};

const TProgmemPalette16 myRainPalette_p PROGMEM =
{
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
};

const TProgmemPalette16 myMistPalette_p PROGMEM =
{
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow, 
    CRGB::Yellow,
    CRGB::Yellow,
};

void no_wifi_rgb(void)
{
    CRGB mycolor = CHSV( HUE_PURPLE, 255, 255);
    CRGB mycolor2 = CHSV( HUE_ORANGE, 255, 255);
    CRGB black  = CRGB::Black;

    gCurrentPalette = CRGBPalette16(
                                   mycolor, mycolor, black,  black,
                                   mycolor2, mycolor2, black,  black,
                                   mycolor, mycolor, black,  black,
                                   mycolor2, mycolor2, black,  black );


    /*  
    gCurrentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
*/
    g_frames_per_second = 200;
    currentBlending = NOBLEND;
}

void FadeInOut(uint64_t Color, uint8_t pIndex)
{
    uint16_t red = (Color >> 16) & 0xff;
    uint16_t green = (Color >> 8) & 0xff; 
    uint16_t blue = Color & 0xff;

    float r = (pIndex/256.0)*red;
    float g = (pIndex/256.0)*green;
    float b = (pIndex/256.0)*blue;
        
    setAll(r,g,b);
}

void setAll(byte red, byte green, byte blue) 
{
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) 
{
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}