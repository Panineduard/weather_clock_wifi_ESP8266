#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include "ConfigManager.h"
#include "TimeClient.h"
#include "WeatherService.h"
#include "WeatherIcons.h"
#include "HTU21DService.h"

WiFiClientSecure client;
ESP8266WebServer server(80);

// ==== DEBUG FLAG ====
#define DEBUG_NO_DISPLAY false  // Set to true to run without display (debug mode)

enum DisplayMode { TIME,
                   WEATHER,
                   SENSOR };
DisplayMode currentMode = TIME;
const unsigned long interval = 1000;       // OLED refresh interval
const unsigned long modeInterval = 10000;  // Mode switch interval
unsigned long lastModeSwitch = 0;


// ==== CONFIGURATION ====
ConfigManager* configManager;
Config config;

//  ===== Weather  =====
WeatherService* weather;
WeatherIcons weatherIcon;
float currentTemp;
float internalTemp;
unsigned long lastWeatherUpdate = 0;
const unsigned long weatherInterval = 1UL * 60UL * 60UL * 1000UL;  // 1 hour in ms

//  ===== Local Sensor (HTU21D)  =====
HTU21DService localSensor;
bool usLocalSensor = false;  // true if HTU21D is available
unsigned long lastSensorUpdate = 0;
const unsigned long sensorInterval = 5000;  // Update every 5 seconds


// ==== LOOP TIMING ====
TimeClient* timeClient;
unsigned long lastUpdate = 0;  // time of last OLED update
String lastTime = "";          // store last displayed time

// ==== OLED DISPLAY ====
#define OLED_RESET 1
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  Wire.begin();
  WiFiManager wifiManager;

  // Init Display
#if !DEBUG_NO_DISPLAY
  Wire.begin(0, 2);  // SDA=GPIO0, SCL=GPIO2
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.setRotation(2);
  showServiceMessage("Connecting WiFi...");
#endif

  // Initialize Configuration Manager
  configManager = new ConfigManager(&server);
  configManager->setupRoutes();
  config = configManager->getConfig();

// DON'T initialize display yet - wait until after weather fetch
#if !DEBUG_NO_DISPLAY
  Serial.println("Display initialization postponed until after weather fetch");
#endif
  // Connect to WiFi using WiFiManager (fallback to AP mode if needed)
  
  Serial.println("Start to connect");
  if (!wifiManager.autoConnect("Smart_Clock_Setup", "12345678")) {
    showServiceMessage("Failed to connect. Restarting...");
    delay(3000);
    ESP.restart();
  }

  // Start web server
  server.begin();
  showServiceMessage("WiFi connected!");
  String ipMsg = "IP: " + WiFi.localIP().toString();
  showServiceMessage(ipMsg.c_str());
  delay(2000);

  // Initialize Time Client with config
  showServiceMessage("Time Client Init...");
  timeClient = new TimeClient();
  timeClient->UTC_OFFSET = config.utc_offset;
  timeClient->init();

  // Initialize Weather Service FIRST (needs most memory)
  showServiceMessage("Weather Init...");
  Serial.print("Free heap before weather: ");
  Serial.println(ESP.getFreeHeap());

  // Force cleanup to free memory
  delay(100);

  weather = new WeatherService(
    String(config.weather_api_key),
    String(config.weather_place_id));

  if (!weather->update()) {
    showServiceMessage("Weather update failed - retrying...");
    delay(5000);

    // Retry once
    Serial.print("Free heap before retry: ");
    Serial.println(ESP.getFreeHeap());

    if (!weather->update()) {
      showServiceMessage("Weather update failed again!");
    }
  }

  Serial.print("Free heap after weather: ");
  Serial.println(ESP.getFreeHeap());

  setCurrentTemp();
  Serial.print("Free heap after display init: ");
  Serial.println(ESP.getFreeHeap());
  showServiceMessage("Weather OK!");

  // Initialize HTU21D Sensor (optional)
  showServiceMessage("Sensor Init...");
  usLocalSensor = localSensor.begin();
  if (usLocalSensor) {
    showServiceMessage("HTU21D OK!");
    localSensor.printReadings();
  } else {
    showServiceMessage("HTU21D N/A");
    Serial.println("HTU21D not found - continuing without local sensor...");
  }
  showServiceMessage("Ready!");
}

void loop() {
  // Handle web server requests
  server.handleClient();

  unsigned long currentMillis = millis();
  long multiplyer = 1;
  if (currentMode == 0) {
    multiplyer = 2;
  }

  // Calculate number of modes (2 or 3 depending on sensor availability)
  int modeCount = usLocalSensor ? 3 : 2;  // TIME, WEATHER, (optional: SENSOR)

  if (currentMillis - lastModeSwitch >= (modeInterval * multiplyer)) {
    lastModeSwitch = currentMillis;
    currentMode = static_cast<DisplayMode>((currentMode + 1) % modeCount);  // cycle through modes
  }

  updateSensor(currentMillis);
  updateWeather(currentMillis);
  updateDisplay(currentMillis);
}

void updateSensor(unsigned long currentMillis) {
  if (!usLocalSensor) return;

  if (currentMillis - lastSensorUpdate >= sensorInterval) {
    lastSensorUpdate = currentMillis;
    localSensor.update();

    // Use local sensor temperature for display
    internalTemp = localSensor.getTemperature();
  }
}

void updateWeather(unsigned long currentMillis) {
  if (currentMillis - lastWeatherUpdate >= weatherInterval) {
    lastWeatherUpdate = currentMillis;
    weather->update();
    setCurrentTemp();
  }
}

void updateDisplay(unsigned long currentMillis) {
  if (currentMillis - lastUpdate >= interval) {
    lastUpdate = currentMillis;

#if DEBUG_NO_DISPLAY
    // Debug mode - print to Serial instead of display
    Serial.print("Display mode: ");
    switch (currentMode) {
      case TIME:
        Serial.print("TIME - ");
        Serial.print(timeClient->getFormattedTime());
        Serial.print(" | ");
        Serial.print((int)round(currentTemp));
        Serial.println("C");
        break;
      case WEATHER:
        Serial.println("WEATHER");
        if (weather->isDataValid()) {
          for (int i = 0; i < 3; i++) {
            DailyWeather d = weather->getDailyWeather(i);
            Serial.print("  Day ");
            Serial.print(i);
            Serial.print(": ");
            Serial.print((int)round(d.tempMin));
            Serial.print("C to ");
            Serial.print((int)round(d.tempMax));
            Serial.println("C");
          }
        }
        break;
      case SENSOR:
        if (usLocalSensor) {
          Serial.print("SENSOR - ");
          Serial.print(localSensor.getTemperature(), 1);
          Serial.print("C, ");
          Serial.print(localSensor.getHumidity(), 0);
          Serial.println("%");
        }
        break;
    }
#else
    // Normal mode - update display
    switch (currentMode) {
      case TIME:
        showTime();
        break;
      case WEATHER:
        showWeather();
        break;
      case SENSOR:
        if (usLocalSensor) {
          showSensorData();
        }
        break;
    }
#endif
  }
}

// ==== MODE FUNCTIONS ====
String lastTimeStr = "";
String lastDateStr = "";
float lastTemp = -999.0;

void showTime() {
  String timeStr = timeClient->getFormattedTime();
  String dateStr = timeClient->getDate();
  if (timeStr == lastTimeStr && dateStr == lastDateStr && currentTemp == lastTemp) return;
  lastTimeStr = timeStr;
  lastDateStr = dateStr;
  lastTemp = currentTemp;

  display.clearDisplay();

  // Draw date (small, centered)
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(dateStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 5);
  display.println(dateStr);

  // Draw time (large, centered)
  display.setTextSize(2);
  display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 25);
  display.println(timeStr);

  display.drawLine(20, 50, 108, 50, SSD1306_WHITE);

  display.setTextSize(1);
  String tempStr = String(internalTemp, 1) + "/" + String(currentTemp, 1) + "C";  // 1 decimal place
  display.getTextBounds(tempStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(128 - w - 2, 54);
  display.println(tempStr);
  display.display();
}

String lastFrame = "";
void showWeather() {
  String timeStr = timeClient->getFormattedTime();
  String dateStr = timeClient->getDate();
  String frameKey = dateStr + timeStr + String(currentTemp);

  if (!weather->isDataValid()) {
    Serial.println("Weather data not valid!");
    return;
  }

  if (frameKey == lastFrame) return;
  lastFrame = frameKey;

  // Get forecast data
  DailyWeather day0 = weather->getDailyWeather(0);
  DailyWeather day1 = weather->getDailyWeather(1);
  DailyWeather day2 = weather->getDailyWeather(2);

  // DEBUG: Print weather data
  Serial.println("=== SHOWING WEATHER ===");
  Serial.print("Day 0: ");
  Serial.print(day0.tempMin);
  Serial.print(" to ");
  Serial.print(day0.tempMax);
  Serial.print(", icon=");
  Serial.println(day0.icon);

  Serial.print("Day 1: ");
  Serial.print(day1.tempMin);
  Serial.print(" to ");
  Serial.print(day1.tempMax);
  Serial.print(", icon=");
  Serial.println(day1.icon);

  Serial.print("Day 2: ");
  Serial.print(day2.tempMin);
  Serial.print(" to ");
  Serial.print(day2.tempMax);
  Serial.print(", icon=");
  Serial.println(day2.icon);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // --- Top: Date centered ---
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(dateStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 0);
  display.print(dateStr);

  // --- Column positions (icon centers) ---
  int col1 = 16;  // Column 1 (Today)
  int col2 = 56;  // Column 2 (Tomorrow)
  int col3 = 96;  // Column 3 (Day after)

  // --- Icons row ---
  display.drawBitmap(col1, 12, weatherIcon.getIconById(day0.icon), 16, 16, SSD1306_WHITE);
  display.drawBitmap(col2, 12, weatherIcon.getIconById(day1.icon), 16, 16, SSD1306_WHITE);
  display.drawBitmap(col3, 12, weatherIcon.getIconById(day2.icon), 16, 16, SSD1306_WHITE);

  // --- Min temps row (centered under each icon) ---
  String min0 = String(day0.tempMin, 1);  // With decimal: -2.2
  String min1 = String(day1.tempMin, 1);
  String min2 = String(day2.tempMin, 1);

  // Center min temp 0 under icon
  display.getTextBounds(min0, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(col1 + (16 - w) / 2, 32);
  display.print(min0);

  // Center min temp 1 under icon
  display.getTextBounds(min1, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(col2 + (16 - w) / 2, 32);
  display.print(min1);

  // Center min temp 2 under icon
  display.getTextBounds(min2, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(col3 + (16 - w) / 2, 32);
  display.print(min2);

  // --- Max temps row (centered under each icon) ---
  String max0 = String(day0.tempMax, 1);  // With decimal: 1.0
  String max1 = String(day1.tempMax, 1);
  String max2 = String(day2.tempMax, 1);

  // Center max temp 0 under icon
  display.getTextBounds(max0, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(col1 + (16 - w) / 2, 42);
  display.print(max0);

  // Center max temp 1 under icon
  display.getTextBounds(max1, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(col2 + (16 - w) / 2, 42);
  display.print(max1);

  // Center max temp 2 under icon
  display.getTextBounds(max2, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(col3 + (16 - w) / 2, 42);
  display.print(max2);

  // --- Bottom line with clear separation ---
  display.setCursor(0, 54);
  display.print(timeStr);

  // Add separator
  display.setCursor(60, 54);
  display.print("|");

  // Current temperature with 1 decimal place
  String tempStr = String(internalTemp, 1) + "/" + String(currentTemp, 1) + "C";
  display.setCursor(68, 54);
  display.print(tempStr);

  display.display();
}

void setCurrentTemp() {
  // Prefer local sensor if available
  if (usLocalSensor) {
    internalTemp = localSensor.getTemperature();
  }

  // Fall back to weather API
  if (weather->isDataValid()) {
    DailyWeather d = weather->getDailyWeather(0);
    currentTemp = d.temp;
  } else {
    Serial.println("Weather data invalid!");
  }
}

void showSensorData() {
  if (!usLocalSensor) return;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Home Climate");

  // Temperature - Large
  display.setTextSize(2);
  display.setCursor(10, 15);
  display.print(localSensor.getTemperature(), 1);
  display.println("C");

  // Humidity
  display.setTextSize(1);
  display.setCursor(10, 35);
  display.print("Humidity: ");
  display.print(localSensor.getHumidity(), 0);
  display.println("%");

  // Comfort level
  display.setCursor(10, 45);
  display.print("Feel: ");
  display.println(localSensor.getComfortLevel());

  // Time at bottom
  display.setCursor(10, 55);
  display.println(timeClient->getFormattedTime());

  display.display();
}

void showServiceMessage(String msg) {
#if DEBUG_NO_DISPLAY
  Serial.println(msg);
#else
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);
  int x = (display.width() - w) / 2;
  int y = (display.height() - h) / 2;
  display.setCursor(x, y);
  display.println(msg);
  display.display();
#endif
  delay(1000);
}
