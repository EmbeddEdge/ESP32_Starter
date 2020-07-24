#ifndef MainModuleH
#define MainModuleH

#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
//#include <ESP8266WiFi.h> // Library specific to ESP8266?
//#include <WiFiManager.h> // Library specific to ESP8266?
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>   
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>    
#include <ESP8266WebServer.h>   // Include the WebServer library
//#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_INA219.h>
#include <FS.h>   // Include the SPIFFS library
#include <EEPROM.h>  
#include "adc.h" 
#include "wifi.h"
#include "uart.h"
#include "memory.h"

#define DEBUG             0   // 0 = debugging disabled, 1 = enabled

const int pin_KSrelay = D3;

/* Structures -------------------------------------------------------------*/
struct power_value_config
{
  float f_current;
  float f_voltage;
  float f_power;
  float f_watthour;
  int   i_port_id;
  int offset;
  String  str_watthour;

  float f_output_watthour;
  String  str_input_watthour;
};

struct timing_value_config
{
  uint16_t uint_duration;
  uint16_t uint_durationKS;
  String str_duration;
  String str_date;
  String str_time;
  String str_datetime;
};

struct https_response_value
{
  String line1;
  String line2;
  String line3;
};

struct ads1115_adc_values
{
  uint16_t adc0 = 0;
  uint16_t adc1 = 0;
  uint16_t adc2 = 0;
  uint16_t adc3 = 0;
};

/* ------------------------------------------------------------------------*/


/* Enums------------------------------------------*/
typedef enum
{ // Procedures
  STATE_ACTION_IDLE, 
  STATE_ACTION_SAMPLE_ADC,
  STATE_ACTION_UPDATE_TICKER,
  STATE_ACTION_PUBLISH_INPUT_ENERGY,  // Publish Input Power Values
  STATE_ACTION_PUBLISH_OUTPUT_ENERGY,  // Publish Input Power Values
  STATE_ACTION_PUBLISH_TEST,
  STATE_ACTION_GET_KIOSKID,
  STATE_ACTION_CHECK_KILLSWITCH,
  STATE_ACTION_PRINT_ADC,
  STATE_ACTION_PRINT_SETTINGS,
  STATE_ACTION_RESET       
} EnumActions_t;

/* -----------------------------------------------*/

/* Function Prototypes -------------------------------------*/
void command_actions(void);     //Main Control State Machine

//Date Time Functions and ISRs
void upDateTime(void); 
void setPublishPower(void);

//Output Control - Data Publishing and relay Functions
void send_data_solarinput(int, int, String, String, int16_t);
void send_data_load_energy(float, int, String, String, int, int16_t);
int16_t send_data_uniqueid(String);
bool get_data_killswitch(int);
void send_data_gps(int, String, String);
void Send_Data_Test(void);
void checkKillSwitch(void);
void killSwitchControl(bool);

//Web Server 
void handleRoot();              // function prototypes for HTTP handlers
void handleKillSwitch();
void handleNotFound();
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
void handleFileUpload();                // upload a new file to the SPIFFS

//Misc. Calculations
String getValue(String, char, int);
/* ---------------------------------------------------------*/

#endif