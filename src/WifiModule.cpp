#include <Arduino.h>
#include "main.h"
#include "RgbModule.h"

// Define the Button trigger flags
extern uint8_t gFlagTrig1;
extern uint8_t gFlagTrig2;

/* **************requestConfigPortal******************************************************
Configure the Wifi Config portal on request
*/
void requestConfigPortal()
{
  if ((gFlagTrig1 == HIGH) || (gFlagTrig2 == HIGH))
  {
    Serial.println("\nConfiguration portal requested.");
    digitalWrite(PIN_LED, LED_ON); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.

    //Local intialization. Once its business is done, there is no need to keep it around
    ESP_WiFiManager ESP_wifiManager;

    //Check if there is stored WiFi router/password credentials.
    //If not found, device will remain in configuration mode until switched off via webserver.
    Serial.print("Opening configuration portal. ");
    Router_SSID = ESP_wifiManager.WiFi_SSID();
    if (Router_SSID != "")
    {
      ESP_wifiManager.setConfigPortalTimeout(60); //If no access point name has been previously entered disable timeout.
      Serial.println("Got stored Credentials. Timeout 60s");
    }
    else
      Serial.println("No stored Credentials. No timeout");

    //it starts an access point
    //and goes into a blocking loop awaiting configuration
    if (!ESP_wifiManager.startConfigPortal((const char *) ssid.c_str(), password))
    {
      Serial.println("Not connected to WiFi but continuing anyway.");
    }
    else
    {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }

    gFlagTrig1 = 0;
    gFlagTrig2 = 0;
    digitalWrite(PIN_LED, LED_OFF); // Turn led off as we are not in configuration mode.
  }
}
