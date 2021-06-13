#ifndef RgbModuleH
#define RgbModuleH

#include "FastLED.h"
#include "pixeltypes.h"
#include "WifiModule.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// How many leds in your strip?
#define NUM_LEDS 9
#define LED_TYPE   NEOPIXEL
#define COLOR_ORDER   GRB
#define VOLTS          5
#define MAX_MA       500
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN SS
//#define CLOCK_PIN 13

#define BRIGHTNESS         200
#define FRAMES_PER_SECOND  100

const TProgmemPalette16 myClearPalette_p PROGMEM =
{
    CRGB::DarkGreen,
    CRGB::DarkGreen,
    CRGB::DarkOliveGreen,
    CRGB::DarkGreen,

    CRGB::Green,
    CRGB::ForestGreen,
    CRGB::OliveDrab,
    CRGB::Green,

    CRGB::SeaGreen,
    CRGB::DarkGreen,
    CRGB::LimeGreen,
    CRGB::YellowGreen,

    CRGB::LimeGreen,
    CRGB::LawnGreen,
    CRGB::DarkGreen,
    CRGB::ForestGreen
};

const TProgmemPalette16 myCloud_p PROGMEM =
{
    CRGB::DarkSlateGrey,
    CRGB::DarkSlateGrey,
    CRGB::CadetBlue,
    CRGB::CadetBlue,

    CRGB::DarkSlateGrey,
    CRGB::CadetBlue,
    CRGB::CadetBlue,
    CRGB::CadetBlue,

    CRGB::DarkSlateGrey,
    CRGB::CadetBlue,
    CRGB::DarkSlateGrey,
    CRGB::CadetBlue,

    CRGB::DarkSlateGrey,
    CRGB::DarkSlateGrey,
    CRGB::DarkSlateGrey,
    CRGB::CadetBlue
};

//RGB Pattern Functions
void FillLEDsFromPaletteColors(uint8_t);
void ChangePaletteToWeather(weather*);
uint64_t ChangeFadeToWeather(weather*);
void ChangePalettePeriodically(void);
void SetupTotallyRandomPalette(void);
void SetupBlackAndWhiteStripedPalette(void);
void SetupPurpleAndGreenPalette(void);
void no_wifi_rgb(void);
void FadeInOut(uint64_t, uint8_t);
void setAll(byte, byte, byte); 
void setPixel(int, byte, byte, byte); 

#endif