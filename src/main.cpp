/*******************************************************************************
* Title                 :   ESP32 WiFi Neopixel Controller 
* Filename              :   main.c
* Author                :   Turyn Lim Banda
* Origin Date           :   26/07/2020
* Version               :   1.0.0
* Compiler              :   PlatformIO
* Target                :   ESP32
* Notes                 :   None
*
* THIS SOFTWARE IS PROVIDED BY EMBEDDEDGE "AS IS" AND ANY EXPRESSED
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL EMBEDDEDGE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/
/*************** MODULE REVISION LOG ***************************************************************
*
*    Date    Version  Author           Description 
*  26/07/20   1.0.0   Turyn Lim Banda  Initial Coding.
*  31/07/20   1.0.1   Turyn Lim Banda  Control Flow and Feature Establish.
*
****************************************************************************************************/

/** @file main.c
 *  @brief The implementation for the dio.
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "main.h"
//#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager
FASTLED_USING_NAMESPACE
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint8_t gFlagTrig1 = 0;
uint8_t gFlagTrig2 = 0;

// SSID and PW for Config Portal
String ssid = "ESP_" + String(ESP_getChipId(), HEX);
const char* password = "your_password";

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Cape%20Town,za&APPID=";
const String key = "b2db60f3906cb485bba1d93fe2a17395";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

IPAddress stationIP   = IPAddress(192, 168, 2, 224);
IPAddress gatewayIP   = IPAddress(192, 168, 2, 1);
IPAddress netMask     = IPAddress(255, 255, 255, 0);
IPAddress dns1IP      = gatewayIP;
IPAddress dns2IP      = IPAddress(8, 8, 8, 8);

String g_httpsResponse[10] = {};

// Define the array of leds
CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> ledsTF;

// Background color for 'unlit' pixels
// Can be set to CRGB::Black if desired.
CRGB gBackgroundColor = CRGB::Black; 
// Example of dim incandescent fairy light background color
// CRGB gBackgroundColor = CRGB(CRGB::FairyLight).nscale8_video(16);

CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void setup()
{
  // put your setup code here, to run once:
  // initialize the LED digital pin as an output.
  delay(3000); //safety startup delay
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  gCurrentPalette = HeatColors_p;
  currentBlending = NOBLEND;

  //chooseNextColorPalette(gTargetPalette);

  //setupWiFiConfig();
}

void loop()
{
  weather weatherForRGB;
  //Is configuration portal requested?
  requestConfigPortal();

  // put your main code here, to run repeatedly
  check_status();
  ReadSerialMon();
  
  weatherForRGB = weatherHttpGET(endpoint, key);
  //delay(10000);

  ChangePalettePeriodically();
  //ChangePaletteToWeather(weatherForRGB);
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; // motion speed 

  FillLEDsFromPaletteColors(startIndex);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  
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
