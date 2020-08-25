#ifndef RgbModuleH
#define RgbModuleH

#include "FastLED.h"
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

//RGB Pattern Functions
void FillLEDsFromPaletteColors(uint8_t);
void ChangePaletteToWeather(weather);
void ChangePalettePeriodically(void);
void SetupTotallyRandomPalette(void);
void SetupBlackAndWhiteStripedPalette(void);
void SetupPurpleAndGreenPalette(void);

#endif