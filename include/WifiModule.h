#ifndef WifiModuleH
#define WifiModuleH

#define PROMPT  "\r\n> "
#define ACK     "\r\n>> "

// Use from 0 to 4. Higher nimber, more debugging messages and memory usage.
#define _WIFIMGR_LOGLEVEL_    4
//For ESP32, To use ESP32 Dev Module, QIO, Flash 4MB/80MHz, Upload 921600


//Ported to ESP32
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager

#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

#define LED_ON      HIGH
#define LED_OFF     LOW

#define WIFI_CONNECT_TIMEOUT        30000L
#define WHILE_LOOP_DELAY            200L
#define WHILE_LOOP_STEPS            (WIFI_CONNECT_TIMEOUT / ( 3 * WHILE_LOOP_DELAY ))

// SSID and PW for Config Portal
String ssid = "ESP_" + String(ESP_getChipId(), HEX);
const char* password = "your_password";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// Indicates whether ESP has WiFi credentials saved from previous session
bool initialConfig = false;

IPAddress stationIP   = IPAddress(192, 168, 2, 224);
IPAddress gatewayIP   = IPAddress(192, 168, 2, 1);
IPAddress netMask     = IPAddress(255, 255, 255, 0);
IPAddress dns1IP      = gatewayIP;
IPAddress dns2IP      = IPAddress(8, 8, 8, 8);

//RGB Pattern Functions
void requestConfigPortal(void);

#endif