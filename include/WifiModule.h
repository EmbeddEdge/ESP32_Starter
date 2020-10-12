#ifndef WifiModuleH
#define WifiModuleH

#define PROMPT  "\r\n> "
#define ACK     "\r\n>> "

/* Structures -------------------------------------------------------------*/
typedef struct weathers{
  float longitude;
  float latitude;
  String location;
  String mainW;
  String descW;
  float tempMain;
  float feelsLike;
  float tempMin;
  float tempMax;
} weather;

/* ------------------------------------------------------------------------*/

// Use from 0 to 4. Higher nimber, more debugging messages and memory usage.
#define _WIFIMGR_LOGLEVEL_    4
//For ESP32, To use ESP32 Dev Module, QIO, Flash 4MB/80MHz, Upload 921600

#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

#define LED_ON      HIGH
#define LED_OFF     LOW

#define WIFI_CONNECT_TIMEOUT        30000L
#define WHILE_LOOP_DELAY            200L
#define WHILE_LOOP_STEPS            (WIFI_CONNECT_TIMEOUT / ( 3 * WHILE_LOOP_DELAY ))

//Debug Functions
void heartBeatPrint(void);
void checkStatusWifi(void);

//RGB Pattern Functions
void setupWiFiConfig(void);
void requestConfigPortal(void);
void weatherHttpGET(String, String, weather*);
void parseWeatherData(String, weather*);

#endif