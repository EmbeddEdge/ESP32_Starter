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
EnumMode_t      g_enum_action_mode;
EnumEndpoint_t  g_enum_endpoint = EP_CT;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint8_t gFlagTrig1 = 0;
uint8_t gFlagTrig2 = 0;

// SSID and PW for Config Portal
String ssid = "ESP_" + String(ESP_getChipId(), HEX);
const char* password = "your_password";

String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Cape%20Town,za&APPID=";
const String key = "012345678901234567890123456789";


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

//Starting to add an array for colour selection. Not yet implemented.
uint64_t colours[] = {0xF0F8FF, 0x9966CC, 0xFAEBD7, 0x00FFFF, 0x7FFFD4,
                      0xF0FFFF, 0xF5F5DC, 0xFFE4C4, 0x000000, 0xFFEBCD,
                      0x0000FF, 0x8A2BE2, 0xA52A2A, 0xDEB887, 0x5F9EA0,
                      0x7FFF00, 0xD2691E, 0xFF7F50, 0x6495ED, 0xFFF8DC,
                      };

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
uint16_t g_frames_per_second = 50;

weather weatherForRGB;

void setup()
{
  delay(3000); //safety startup delay
  //Configure the Serial Output
  SerialMon.begin(115200);
  // initialize the LED digital pin as an output.
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  //Setup the RGB strip
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  SerialMon.print(F("\t Device Startup\n\r"));

  setupWiFiConfig();
}

void loop()
{
  //Do the initial checks i.e. is WiFi Connected? Is there serial data?
  requestConfigPortal();  //Is configuration portal requested?
  checkStatusWifi();         
  ReadSerialMon();        //Read the serial line if anything is there

  //Check the current mode of operation and set the RGB settings accordingly
  user_mode_set();
  
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
        static uint8_t fadeIndex;

        fadeIndex = upDownCounterInt();

        g_frames_per_second = 1000;

        FadeInOut(CRGB::Crimson, fadeIndex);

        //static uint8_t countIndex;
        //countIndex = countIndex + 1; // motion speed

        //Rapid blinks for no WiFi
        //no_wifi_rgb();
        //Display the RGB strip based the up-to-date settings
        //FillLEDsFromPaletteColors(countIndex);
        
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
      
      endpoint_mode_set();                            //Upadte Location according to user
      weatherHttpGET(endpoint, key, &weatherForRGB);  //Fetch weather data on a timed interval  
      ChangePaletteToWeather(&weatherForRGB);         //Update the RGB settings based on input data

      static uint8_t countIndex;
      countIndex = countIndex + 1; // motion speed

      FillLEDsFromPaletteColors(countIndex); //Display the RGB strip based the up-to-date settings
    }
    break;
    case MODE_MOOD: 
    {
      //Setup Mood Lights
      static uint8_t fadeIndex;

      fadeIndex = upDownCounterInt();
      
      FadeInOut(CRGB::Plum, fadeIndex);
    }
    break; 
    default:
    break;
  }
}

uint8_t upCounterInt()
{
  static uint8_t countIndex;
  countIndex = countIndex + 1; // motion speed
    
  return countIndex; 
}

uint8_t upDownCounterInt()
{
  static uint8_t countIndex;
  static uint8_t upDown = 0;

  if(upDown==0)
  {
    countIndex = countIndex + 1; // motion speed
    if(countIndex==255)
    {
      upDown = 1;
    } 
  }
  else
  {
    countIndex = countIndex - 1; // motion speed
    if(countIndex==0)
    {
      upDown = 0;
    } 
  }
  return countIndex; 
}

/****************endpoint_mode_set************************************************************             
Set which city data is pulled from
Input:  N/A
Output: N/A
*/ 
void endpoint_mode_set(void)
{
  switch(g_enum_endpoint)
  {
    case EP_CT:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Cape%20Town,za&APPID=";
    }
    break;
    case EP_PE:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Port%20Elizabeth,za&APPID=";
    }
    break;
    case EP_DB:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Durban,za&APPID=";
    }
    break;
    case EP_JB:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Johannesburg,za&APPID=";
    }
    break;
    case EP_PA:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Paris,fr&APPID=";
    }
    break;
    case EP_DP:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Deputatsky,ru&APPID=";
    }
    break;
    case EP_KS:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Kinshasa,cd&APPID=";
    }
    break;
    case EP_VA:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Vancouver,ca&APPID=";
    }
    break;
    case EP_PH:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Perth,au&APPID=";
    }
    break;
    case EP_LM:
    {
      endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Lima,pe&APPID=";
    }
    break;
    default:
    break;
  }
}

/****************fadeControl************************************************************             
Change the colour that shows for the fade effect
Input:  N/A
Output: N/A
*/ 
/*
void fadeControl(void)
{
  switch(g_enum_endpoint)
  {
    case EP_CT:
    {
      FadeInOut(CRGB::Yellow, fadeIndex);
    }
    break;
    case EP_PE:
    {
      FadeInOut(CRGB::DarkOliveGreen, fadeIndex);
    }
    break;
    case EP_DB:
    {
      FadeInOut(CRGB::LightCoral, fadeIndex);
    }
    break;
    case EP_JB:
    {
      FadeInOut(CRGB::Yellow, fadeIndex);
    }
    break;
    default:
    break;
  }
}
*/