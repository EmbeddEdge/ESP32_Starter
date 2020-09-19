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
EnumMode_t     g_enum_action_mode;

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

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint16_t g_frames_per_second = 100;

void setup()
{
  delay(3000); //safety startup delay
  //Configure the Serial Output
  SerialMon.begin(38400);
  // initialize the LED digital pin as an output.
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);
  //Setup the RGB strip
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  gCurrentPalette = HeatColors_p;
  currentBlending = NOBLEND;
  SerialMon.print(F("\tDevice Startup\n\r"));

  //chooseNextColorPalette(gTargetPalette);

  //setupWiFiConfig();
}

void loop()
{
  //Do the initial checks i.e. is WiFi Connected? Is there serial data?
  requestConfigPortal();  //Is configuration portal requested?
  check_status();
  ReadSerialMon();        //Read the serial line if anything is there

  //Check the current mode of operation and set the RGB settings accordingly
  user_mode_set();
  
  //Display the RGB strip based the up-to-date settings
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; // motion speed 
  //SerialMon.println(startIndex);

  FillLEDsFromPaletteColors(startIndex);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/g_frames_per_second); 
  
}

/****************user_mode_set************************************************************             
Set the mode of operation for the Light setting control based on what is decided by 
the user and also the current WiFi Status
Input:  N/A
Output: N/A
*/ 
void user_mode_set(void)
{
  switch(g_enum_action_mode)
  {
    case MODE_DEFAULT:
    {
      //This mode comes on if there is no WiFi connection and no other user modes have been requested
      if(WiFi.status() != WL_CONNECTED)
      {
        //Set up a regular pallette
        //Rapid blinks for no WiFi
        no_wifi_rgb();
      }
      else
      {
        g_enum_action_mode = MODE_WIFI_WEATHER;
      }
    }
    break;
    case MODE_WIFI_WEATHER:
    {
      if(WiFi.status() != WL_CONNECTED)
      {
        g_enum_action_mode = MODE_DEFAULT;
      }
      weather weatherForRGB;
      //Fetch weather data on a timed interval
      weatherForRGB = weatherHttpGET(endpoint, key);

      //Update the RGB settings based on input data
      //ChangePalettePeriodically();
      ChangePaletteToWeather(weatherForRGB);
    }
    break;
    case MODE_MOOD: 
    {
      //Setup Mood Lights
    }
    break; 
    default:
    break;
  }
}
