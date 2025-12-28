#ifndef WEATHERSERVICE_H
#define WEATHERSERVICE_H

#include <Arduino.h>
#include <WiFiClientSecure.h>

struct DailyWeather {
  String day;      // e.g., "2025-12-28"
  float tempMin;   // °C
  float tempMax;   // °C
  float temp;      // °C
  int icon;        // MeteoSource icon ID
};

class WeatherService {
public:
  WeatherService(const String& apiKey, const String& placeId = "postal-ua-79007")
    : m_apiKey(apiKey), m_placeId(placeId), m_validData(false) {
    // Initialize with defaults
    for (int i = 0; i < 3; i++) {
      m_daily[i].tempMin = 0;
      m_daily[i].tempMax = 0;
      m_daily[i].temp = 0;
      m_daily[i].icon = 0;
    }
  }

  bool update() {
    if (!fetchAndParse()) {
      m_validData = false;
      return false;
    }
    m_validData = true;
    return true;
  }

  bool isDataValid() const { return m_validData; }

  DailyWeather getDailyWeather(int index) const {
    if (index < 0 || index > 2) return DailyWeather();
    return m_daily[index];
  }

private:
  WiFiClientSecure client;
  String m_apiKey;
  String m_placeId;
  DailyWeather m_daily[3];
  bool m_validData;

  String buildRequest() const {
    String path = "/api/v1/free/point?place_id=" + m_placeId + "&sections=daily&language=en&units=metric";
    String request = "GET " + path + " HTTP/1.1\r\n";
    request += "Host: www.meteosource.com\r\n";
    request += "X-API-Key: " + m_apiKey + "\r\n";
    request += "User-Agent: ESP8266\r\n";
    request += "Accept: application/json\r\n";
    request += "Connection: close\r\n\r\n";
    return request;
  }

  bool fetchAndParse() {
    const char* host = "www.meteosource.com";
    const uint16_t port = 443;
    
    client.setTimeout(15000);
    client.setInsecure();

    Serial.print("Weather: Connecting...");
    if (!client.connect(host, port)) {
      Serial.println("FAIL");
      return false;
    }
    Serial.println("OK");
    
    // Send request
    String request = buildRequest();
    if (client.print(request) != request.length()) {
      Serial.println("Weather: Send error");
      client.stop();
      return false;
    }
    client.flush();

    // Wait for response
    unsigned long responseStart = millis();
    while (client.available() == 0) {
      if (millis() - responseStart > 20000) {
        Serial.println("Weather: Timeout");
        client.stop();
        return false;
      }
      delay(10);
    }

    // Read and validate HTTP status
    int httpCode = 0;
    String line = client.readStringUntil('\n');
    int firstSpace = line.indexOf(' ');
    int secondSpace = line.indexOf(' ', firstSpace + 1);
    if (firstSpace > 0 && secondSpace > firstSpace) {
      httpCode = line.substring(firstSpace + 1, secondSpace).toInt();
    }
    
    if (httpCode != 200) {
      Serial.print("Weather: HTTP ");
      Serial.println(httpCode);
      client.stop();
      return false;
    }

    // Skip remaining headers until we find empty line
    while (client.connected() || client.available()) {
      line = client.readStringUntil('\n');
      if (line == "\r" || line.length() == 0) {
        break;
      }
    }

    Serial.print("Weather: Parsing...");
    
    // Parse JSON on-the-fly
    int dayIndex = -1;
    bool inAllDay = false;
    String buffer = "";
    
    while (client.connected() || client.available()) {
      if (!client.available()) {
        delay(1);
        continue;
      }
      
      char c = client.read();
      buffer += c;
      
      // Keep buffer small - only last 100 chars
      if (buffer.length() > 100) {
        buffer = buffer.substring(buffer.length() - 100);
      }
      
      // Detect new day object
      if (buffer.endsWith("\"day\":\"")) {
        dayIndex++;
        if (dayIndex >= 3) break; // Got all 3 days
        
        // Read date (next 10 chars: YYYY-MM-DD)
        String date = "";
        for (int i = 0; i < 10 && client.available(); i++) {
          date += (char)client.read();
        }
        m_daily[dayIndex].day = date;
        inAllDay = false;
      }
      
      // Detect all_day section
      if (buffer.endsWith("\"all_day\":{")) {
        inAllDay = true;
      }
      
      // Only parse if we're in a valid day and all_day section
      if (dayIndex >= 0 && dayIndex < 3 && inAllDay) {
        
        // Extract icon
        if (buffer.endsWith("\"icon\":")) {
          String num = "";
          while (client.available()) {
            char ch = client.read();
            if (ch >= '0' && ch <= '9') {
              num += ch;
            } else {
              buffer += ch;
              break;
            }
          }
          if (num.length() > 0) {
            m_daily[dayIndex].icon = num.toInt();
          }
        }
        
        // Extract temperature
        if (buffer.endsWith("\"temperature\":")) {
          String num = readNumber();
          if (num.length() > 0) {
            m_daily[dayIndex].temp = num.toFloat();
          }
        }
        
        // Extract temperature_min
        if (buffer.endsWith("\"temperature_min\":")) {
          String num = readNumber();
          if (num.length() > 0) {
            m_daily[dayIndex].tempMin = num.toFloat();
          }
        }
        
        // Extract temperature_max
        if (buffer.endsWith("\"temperature_max\":")) {
          String num = readNumber();
          if (num.length() > 0) {
            m_daily[dayIndex].tempMax = num.toFloat();
          }
        }
        
        // Exit all_day when we see closing brace after we have data
        if (buffer.endsWith("}") && m_daily[dayIndex].icon > 0) {
          inAllDay = false;
        }
      }
    }
    
    client.stop();
    Serial.println("OK");
    
    return (dayIndex >= 2); // Success if we got at least 3 days
  }
  
  String readNumber() {
    String num = "";
    while (client.available() && num.length() < 10) {
      char ch = client.read();
      if ((ch >= '0' && ch <= '9') || ch == '.' || ch == '-') {
        num += ch;
      } else {
        break;
      }
    }
    return num;
  }
};

#endif
