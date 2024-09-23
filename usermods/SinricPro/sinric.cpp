/*
 * Example
 *
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 * - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 * - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

 // Custom devices requires SinricPro ESP8266/ESP32 SDK 2.9.6 or later

// Uncomment the following line to enable serial debug output
//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
  #define DEBUG_ESP_PORT Serial
  #define NODEBUG_WEBSOCKETS
  #define NDEBUG
#endif

#include <Arduino.h>
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ESP32
  #include <WiFi.h>
#endif

#include <SinricPro.h>
#include "Leds.h"

#define APP_KEY    "5b4b312f-3222-40df-972a-57f87b9dee86"
#define APP_SECRET "ecc4d3bb-b99b-4482-9754-fc8dd84830fd-10927b59-4df8-4a64-ba74-c801300cf48a"
#define DEVICE_ID  "66d86b0d54041e4ff629c0e0"

#define SSID       "YOUR_WIFI_SSID"
#define PASS       "YOUR_WIFI_PASS"

#define BAUD_RATE  9600

Leds &leds = SinricPro[DEVICE_ID];

/*************
 * Variables *
 ***********************************************
 * Global variables to store the device states *
 ***********************************************/

// PowerStateController
bool globalPowerState;

// ModeController
std::map<String, String> globalModes;

// ColorController
struct Color {
  byte r;
  byte g;
  byte b;
};

Color color;

/*************
 * Callbacks *
 *************/

// PowerStateController
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("[Device: %s]: Powerstate changed to %s\r\n", deviceId.c_str(), state ? "on" : "off");
  globalPowerState = state;
  return true; // request handled properly
}

// ModeController
bool onSetMode(const String& deviceId, const String& instance, String &mode) {
  Serial.printf("[Device: %s]: Modesetting for \"%s\" set to mode %s\r\n", deviceId.c_str(), instance.c_str(), mode.c_str());
  globalModes[instance] = mode;
  return true;
}

// ColorController
bool onColor(const String &deviceId, byte &r, byte &g, byte &b) {
  Serial.printf("[Device: %s]: Color set to red=%d, green=%d, blue=%d\r\n", deviceId.c_str(), r, g, b);
  color.r = r;
  color.g = g;
  color.b = b;
  return true; // request handled properly
}

/**********
 * Events *
 *************************************************
 * Examples how to update the server status when *
 * you physically interact with your device or a *
 * sensor reading changes.                       *
 *************************************************/

// PowerStateController
void updatePowerState(bool state) {
  leds.sendPowerStateEvent(state);
}

// ModeController
void updateMode(String instance, String mode) {
  leds.sendModeEvent(instance, mode, "PHYSICAL_INTERACTION");
}

// ColorController
void updateColor(byte r, byte g, byte b) {
  leds.sendColorEvent(r, g, b);
}

/********* 
 * Setup *
 *********/

void setupSinricPro() {

  // PowerStateController
  leds.onPowerState(onPowerState);

  // ModeController
  leds.onSetMode("modeInstance1", onSetMode);


  // ColorController
  leds.onColor(onColor);

  SinricPro.onConnected([]{ Serial.printf("[SinricPro]: Connected\r\n"); });
  SinricPro.onDisconnected([]{ Serial.printf("[SinricPro]: Disconnected\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
};

void setupWiFi() {
  #if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP); 
    WiFi.setAutoReconnect(true);
  #elif defined(ESP32)
    WiFi.setSleep(false); 
    WiFi.setAutoReconnect(true);
  #endif

  WiFi.begin(SSID, PASS);
  Serial.printf("[WiFi]: Connecting to %s", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected\r\n");
}

void setup() {
  Serial.begin(BAUD_RATE);
  setupWiFi();
  setupSinricPro();
}

/********
 * Loop *
 ********/

void loop() {
  SinricPro.handle();
}