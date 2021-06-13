#include <Arduino.h>
#include <ESPAsync_WiFiManager.h> 
//#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager
#include "WifiModule.h"
#include "main.h"

#define HTTP_PORT           80

// Define the Button trigger flags
extern uint8_t gFlagTrig1;
extern uint8_t gFlagTrig2;

// SSID and PW for Config Portal
extern String ssid;
extern const char* password;

extern String Router_SSID;
extern String Router_Pass;

extern IPAddress stationIP;
extern IPAddress gatewayIP;
extern IPAddress netMask;
extern IPAddress dns1IP;
extern IPAddress dns2IP;

extern String g_httpsResponse[10];

// Indicates whether ESP has WiFi credentials saved from previous session
bool initialConfig = false;

/* **************setupWiFiConfig******************************************************
Setup the WiFi state
*/
void setupWiFiConfig()
{
    SerialMon.println("\nConfiguration portal requested.");
    digitalWrite(PIN_LED, LED_ON); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.

    AsyncWebServer webServer(HTTP_PORT);

    DNSServer dnsServer;

    ESPAsync_WiFiManager ESP_wifiManager(&webServer, &dnsServer, "AutoConnectAP");;

    //Check if there is stored WiFi router/password credentials.
    //If not found, device will remain in configuration mode until switched off via webserver.
    SerialMon.print("Opening configuration portal. ");
    Router_SSID = ESP_wifiManager.WiFi_SSID();
    if (Router_SSID != "")
    {
      ESP_wifiManager.setConfigPortalTimeout(60); //If no access point name has been previously entered disable timeout.
      SerialMon.println("Got stored Credentials. Timeout 60s");
    }
    else
      SerialMon.println("No stored Credentials. No timeout");

    //it starts an access point
    //and goes into a blocking loop awaiting configuration
    if (!ESP_wifiManager.startConfigPortal((const char *) ssid.c_str(), password))
    {
      SerialMon.println("Not connected to WiFi but continuing anyway.");
    }
    else
    {
      //if you get here you have connected to the WiFi
      SerialMon.println("connected...yeey :)");
    }

    gFlagTrig1 = 0;
    gFlagTrig2 = 0;
    digitalWrite(PIN_LED, LED_OFF); // Turn led off as we are not in configuration mode.
}

/* **************requestConfigPortal******************************************************
Configure the Wifi Config portal on request
*/
void requestConfigPortal()
{
  if ((gFlagTrig1 == HIGH) || (gFlagTrig2 == HIGH))
  {
    SerialMon.println("\nConfiguration portal requested.");
    digitalWrite(PIN_LED, LED_ON); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.

    AsyncWebServer webServer(HTTP_PORT);

    DNSServer dnsServer;

    //Local intialization. Once its business is done, there is no need to keep it around
    ESPAsync_WiFiManager ESP_wifiManager(&webServer, &dnsServer, "AutoConnectAP");;

    //Check if there is stored WiFi router/password credentials.
    //If not found, device will remain in configuration mode until switched off via webserver.
    SerialMon.print("Opening configuration portal. ");
    Router_SSID = ESP_wifiManager.WiFi_SSID();
    if (Router_SSID != "")
    {
      ESP_wifiManager.setConfigPortalTimeout(30); //If no access point name has been previously entered disable timeout.
      SerialMon.println("Got stored Credentials. Timeout 30s");
    }
    else
      SerialMon.println("No stored Credentials. No timeout");

    //it starts an access point
    //and goes into a blocking loop awaiting configuration
    if (!ESP_wifiManager.startConfigPortal((const char *) ssid.c_str(), password))
    {
      SerialMon.println("Not connected to WiFi but continuing anyway.");
    }
    else
    {
      //if you get here you have connected to the WiFi
      SerialMon.println("connected...yeey :)");
    }

    gFlagTrig1 = 0;
    gFlagTrig2 = 0;
    digitalWrite(PIN_LED, LED_OFF); // Turn led off as we are not in configuration mode.
  }
}

/* **************weatherHttpGET******************************************************
Fetch Weather Data periodically 
*/
void weatherHttpGET(String p_endpoint, String p_key, weather *payloadWeather)
{
  //weather payloadWeather;
  unsigned long currentMillis = millis();
  uint16_t interval = 30000; //30 seconds
  static unsigned long previousMillis = currentMillis;

  #if DEBUG_TIME
  SerialMon.print("Current Time: ");
  SerialMon.println(currentMillis);
  SerialMon.print("Saved Time: ");
  SerialMon.println(previousMillis);
  SerialMon.print("Diff Time: ");
  SerialMon.println(currentMillis - previousMillis);
  #endif

  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    if(WiFi.status()==WL_CONNECTED)
    {
      HTTPClient http;

      http.begin(p_endpoint + p_key);
    
      int httpCode = http.GET();
      #if DEBUG
      SerialMon.println(p_endpoint+p_key);
      SerialMon.println(p_endpoint);
      SerialMon.print("HTTP GET return code: ");
      SerialMon.println(httpCode);
      #endif
    
      if (httpCode > 0) 
      { //Check for the returning code
        String payload = http.getString();
        
        parseWeatherData(payload, payloadWeather);
        #if DEBUG
        SerialMon.println(payload);
        SerialMon.println();
        SerialMon.print(F("\rWeather longitude: "));
        SerialMon.println(payloadWeather->longitude);
        SerialMon.print(F("\rWeather latitude: "));
        SerialMon.println(payloadWeather->latitude);
        SerialMon.print(F("\rWeather location: "));
        SerialMon.println(payloadWeather->location);
        SerialMon.print(F("\rWeather Main Description: "));
        SerialMon.println(payloadWeather->mainW);
        SerialMon.print(F("\rWeather Aux Description: "));
        SerialMon.println(payloadWeather->descW);
        SerialMon.print(F("\rWeather Temperature: "));
        SerialMon.println(payloadWeather->tempMain);
        SerialMon.print(F("\rWeather Temperature FeelsLike: "));
        SerialMon.println(payloadWeather->feelsLike);
        SerialMon.print(F("\rWeather Temperature Min: "));
        SerialMon.println(payloadWeather->tempMin);
        SerialMon.print(F("\rWeather Temperature Max: "));
        SerialMon.println(payloadWeather->tempMax);
        #endif
      }
      else 
      {
        SerialMon.println("Error on HTTP request");
      }
    
      http.end();
    }

  }  
  //return payloadWeather;
}

void heartBeatPrint(void)
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
  {
    SerialMon.print("H");        // H means connected to WiFi
  }
  else
  {
    SerialMon.print("F");        // F means not connected to WiFi
  }
  if (num == 80)
  {
    SerialMon.println(); 
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    SerialMon.print(" ");
  }
}

void checkStatusWifi(void)
{
  static ulong checkstatus_timeout = 0;

  #define HEARTBEAT_INTERVAL    10000L
  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

void parseWeatherData(String p_payload, weather *myWeather)
{
  //weather* myWeather;
  //First find and populate the longitude value
  int firstColon = p_payload.indexOf(':');
  int secondColon = p_payload.indexOf(':', firstColon+1);
  int firstComma = p_payload.indexOf(',');
  String sLongitude = p_payload.substring(secondColon+1, firstComma);
  myWeather->longitude = sLongitude.toFloat();

  //Next find and populate the latitude value
  int thirdColon = p_payload.indexOf(':', secondColon+1);
  int firstCloseBrak = p_payload.indexOf('}', thirdColon+1);
  String sLatitude = p_payload.substring(thirdColon+1, firstCloseBrak);
  myWeather->latitude = sLatitude.toFloat();

  //Next find and populate the location/city name
  int lastComma = p_payload.lastIndexOf(',');
  int fourthLastQt = p_payload.lastIndexOf('"', lastComma-2); //-2 because of the " before the comma
  myWeather->location = p_payload.substring(fourthLastQt+1, lastComma-1);

  //Next find and populate the main description of the Weather
  int firstM = p_payload.indexOf('m');
  int thirtnthQt = p_payload.indexOf('"',firstM+5); //+5 because of " at the end of "main"
  int fourtnthQt = p_payload.indexOf('"',thirtnthQt+1); 
  myWeather->mainW = p_payload.substring(thirtnthQt+1, fourtnthQt);

  //Next find and populate the secondary description of the Weather
  int fiftnthQt = p_payload.indexOf('"',fourtnthQt+1); 
  int sixtnthQt = p_payload.indexOf('"',fiftnthQt+1); 
  int sevntnthQt = p_payload.indexOf('"',sixtnthQt+1); 
  int eightnthQt = p_payload.indexOf('"',sevntnthQt+1); 
  myWeather->descW = p_payload.substring(sevntnthQt+1, eightnthQt);

  //Next find and populate the main temperature value
  int destinationColon = 11;
  int colonCount = 0;
  int startSub=0, endSub;
  while(colonCount<destinationColon)
  {
    startSub = p_payload.indexOf(':',startSub+1); 
    endSub = p_payload.indexOf(',',startSub+1); 
    colonCount++;
    //SerialMon.print(F("\rMain Temp Find: "));
    //SerialMon.println(p_payload.substring(startSub+1, endSub));
  }
  String sTempMain = p_payload.substring(startSub+1, endSub);
  myWeather->tempMain = sTempMain.toFloat();

  //Next find and populate the aux temperature value
  int destinationColon2 = 12;
  int colonCount2 = 0;
  int startSub2=0, endSub2;
  while(colonCount2<destinationColon2)
  {
    startSub2 = p_payload.indexOf(':',startSub2+1); 
    endSub2 = p_payload.indexOf(',',startSub2+1); 
    colonCount2++;
  }
  String sTempAux = p_payload.substring(startSub2+1, endSub2);
  myWeather->feelsLike = sTempAux.toFloat();

  //Next find and populate the min temperature value
  int destinationColon3 = 13;
  int colonCount3 = 0;
  int startSub3=0, endSub3;
  while(colonCount3<destinationColon3)
  {
    startSub3 = p_payload.indexOf(':',startSub3+1); 
    endSub3 = p_payload.indexOf(',',startSub3+1); 
    colonCount3++;
  }
  String sTempMin = p_payload.substring(startSub3+1, endSub3);
  myWeather->tempMin = sTempMin.toFloat();

  //Next find and populate the max temperature value
  int destinationColon4 = 14;
  int colonCount4 = 0;
  int startSub4=0, endSub4;
  while(colonCount4<destinationColon4)
  {
    startSub4 = p_payload.indexOf(':',startSub4+1); 
    endSub4 = p_payload.indexOf(',',startSub4+1); 
    colonCount4++;
  }
  String sTempMax = p_payload.substring(startSub4+1, endSub4);
  myWeather->tempMax = sTempMax.toFloat();

}

