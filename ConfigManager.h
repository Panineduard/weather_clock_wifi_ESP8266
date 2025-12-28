#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <ESP8266WebServer.h>
#include <EEPROM.h>

struct Config {
  char wifi_ssid[32];
  char wifi_password[64];
  char weather_api_key[64];
  char weather_place_id[32];
  int utc_offset;  // UTC offset in hours
  bool initialized;
};

class ConfigManager {
public:
  ConfigManager(ESP8266WebServer* server) : m_server(server) {
    EEPROM.begin(512);
    loadConfig();
  }

  void setupRoutes() {
    m_server->on("/", [this]() { handleRoot(); });
    m_server->on("/config", HTTP_GET, [this]() { handleConfigPage(); });
    m_server->on("/config", HTTP_POST, [this]() { handleConfigSave(); });
    m_server->on("/status", [this]() { handleStatus(); });
    m_server->on("/reset", [this]() { handleFactoryReset(); });
  }

  Config getConfig() const { return m_config; }

  bool isConfigured() const { return m_config.initialized; }

  void factoryReset() {
    // Clear EEPROM
    for (int i = 0; i < 512; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
    
    Serial.println("Factory reset complete - all settings erased");
    Serial.println("Device will restart in 3 seconds...");
    delay(3000);
    ESP.restart();
  }

  void setDefaults() {
    strcpy(m_config.wifi_ssid, "WI_FI_LINE");
    strcpy(m_config.wifi_password, "PASSWORD");
    strcpy(m_config.weather_api_key, "WEATHER_KEY");
    strcpy(m_config.weather_place_id, "postal-ua-79007");
    m_config.utc_offset = 2;
    m_config.initialized = true;
    saveConfig();
  }

private:
  ESP8266WebServer* m_server;
  Config m_config;

  void loadConfig() {
    EEPROM.get(0, m_config);
    if (!m_config.initialized) {
      // First time setup - set defaults
      setDefaults();
    }
  }

  void saveConfig() {
    EEPROM.put(0, m_config);
    EEPROM.commit();
  }

  void handleRoot() {
    String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Weather Clock</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 600px;
      margin: 50px auto;
      padding: 20px;
      background: #f0f0f0;
    }
    .container {
      background: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    }
    h1 {
      color: #333;
      border-bottom: 2px solid #4CAF50;
      padding-bottom: 10px;
    }
    .status {
      background: #e8f5e9;
      padding: 15px;
      border-radius: 5px;
      margin: 20px 0;
      border-left: 4px solid #4CAF50;
    }
    .status-item {
      margin: 8px 0;
      font-size: 14px;
    }
    .button {
      display: inline-block;
      padding: 12px 30px;
      background: #4CAF50;
      color: white;
      text-decoration: none;
      border-radius: 5px;
      margin: 10px 5px;
      transition: background 0.3s;
    }
    .button:hover {
      background: #45a049;
    }
    .button-danger {
      background: #f44336;
    }
    .button-danger:hover {
      background: #da190b;
    }
    .info {
      color: #666;
      font-size: 14px;
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🕐 Weather Clock Dashboard</h1>
    
    <div class="status">
      <div class="status-item">📶 <strong>WiFi:</strong> Connected</div>
      <div class="status-item">🌐 <strong>IP Address:</strong> )=====";
    
    html += WiFi.localIP().toString();
    html += R"=====(</div>
      <div class="status-item">📍 <strong>Location:</strong> )=====";
    html += String(m_config.weather_place_id);
    html += R"=====(</div>
    </div>
    
    <div style="text-align: center; margin-top: 30px;">
      <a href="/config" class="button">⚙️ Configure Settings</a>
      <a href="/status" class="button">📊 Full Status</a>
      <a href="/reset" class="button button-danger">🔄 Factory Reset</a>
    </div>
    
    <div class="info">
      <p><strong>About:</strong> This device displays time, weather forecast, and inspirational quotes on an OLED screen.</p>
      <p>Use the configuration page to update WiFi credentials, weather API settings, and time zone.</p>
    </div>
  </div>
</body>
</html>
)=====";
    m_server->send(200, "text/html", html);
  }

  void handleConfigPage() {
    String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Configuration</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 600px;
      margin: 50px auto;
      padding: 20px;
      background: #f0f0f0;
    }
    .container {
      background: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    }
    h1 {
      color: #333;
      border-bottom: 2px solid #2196F3;
      padding-bottom: 10px;
    }
    h2 {
      color: #555;
      margin-top: 25px;
      font-size: 18px;
    }
    .form-group {
      margin: 15px 0;
    }
    label {
      display: block;
      margin-bottom: 5px;
      color: #555;
      font-weight: bold;
    }
    input[type="text"],
    input[type="password"],
    input[type="number"] {
      width: 100%;
      padding: 10px;
      border: 1px solid #ddd;
      border-radius: 4px;
      box-sizing: border-box;
      font-size: 14px;
    }
    input:focus {
      outline: none;
      border-color: #2196F3;
    }
    .hint {
      font-size: 12px;
      color: #888;
      margin-top: 3px;
    }
    button {
      background: #2196F3;
      color: white;
      padding: 12px 30px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
      margin-top: 20px;
      width: 100%;
    }
    button:hover {
      background: #0b7dda;
    }
    .back-link {
      display: inline-block;
      margin-top: 20px;
      color: #2196F3;
      text-decoration: none;
    }
    .back-link:hover {
      text-decoration: underline;
    }
    .warning {
      background: #fff3cd;
      padding: 10px;
      border-radius: 4px;
      border-left: 4px solid #ffc107;
      margin: 20px 0;
      font-size: 14px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>⚙️ Configuration</h1>
    
    <form method="POST" action="/config">
      <h2>📶 WiFi Settings</h2>
      <div class="form-group">
        <label for="wifi_ssid">WiFi SSID:</label>
        <input type="text" id="wifi_ssid" name="wifi_ssid" value=")=====";
    html += String(m_config.wifi_ssid);
    html += R"=====(">
        <div class="hint">Network name to connect to</div>
      </div>
      
      <div class="form-group">
        <label for="wifi_password">WiFi Password:</label>
        <input type="password" id="wifi_password" name="wifi_password" value=")=====";
    html += String(m_config.wifi_password);
    html += R"=====(">
        <div class="hint">Leave blank to keep current password</div>
      </div>
      
      <h2>🌤️ Weather API Settings</h2>
      <div class="form-group">
        <label for="weather_api_key">MeteoSource API Key:</label>
        <input type="text" id="weather_api_key" name="weather_api_key" value=")=====";
    html += String(m_config.weather_api_key);
    html += R"=====(">
        <div class="hint">Get your free API key at <a href="https://www.meteosource.com" target="_blank">meteosource.com</a></div>
      </div>
      
      <div class="form-group">
        <label for="weather_place_id">Place ID:</label>
        <input type="text" id="weather_place_id" name="weather_place_id" value=")=====";
    html += String(m_config.weather_place_id);
    html += R"=====(">
        <div class="hint">Example: postal-ua-79007, postal-us-10001, prague</div>
      </div>
      
      <h2>🕐 Time Settings</h2>
      <div class="form-group">
        <label for="utc_offset">UTC Offset (hours):</label>
        <input type="number" id="utc_offset" name="utc_offset" value=")=====";
    html += String(m_config.utc_offset);
    html += R"=====(">
        <div class="hint">Your timezone offset from UTC (e.g., 2 for EET, -5 for EST)</div>
      </div>
      
      <div class="warning">
        ⚠️ <strong>Note:</strong> Changing WiFi settings will restart the device. Make sure the credentials are correct!
      </div>
      
      <button type="submit">💾 Save Configuration</button>
    </form>
    
    <a href="/" class="back-link">← Back to Dashboard</a>
  </div>
</body>
</html>
)=====";
    m_server->send(200, "text/html", html);
  }

  void handleConfigSave() {
    if (m_server->hasArg("wifi_ssid")) {
      String ssid = m_server->arg("wifi_ssid");
      ssid.toCharArray(m_config.wifi_ssid, sizeof(m_config.wifi_ssid));
    }

    if (m_server->hasArg("wifi_password")) {
      String pass = m_server->arg("wifi_password");
      if (pass.length() > 0) {  // Only update if not empty
        pass.toCharArray(m_config.wifi_password, sizeof(m_config.wifi_password));
      }
    }

    if (m_server->hasArg("weather_api_key")) {
      String key = m_server->arg("weather_api_key");
      key.toCharArray(m_config.weather_api_key, sizeof(m_config.weather_api_key));
    }

    if (m_server->hasArg("weather_place_id")) {
      String place = m_server->arg("weather_place_id");
      place.toCharArray(m_config.weather_place_id, sizeof(m_config.weather_place_id));
    }

    if (m_server->hasArg("utc_offset")) {
      m_config.utc_offset = m_server->arg("utc_offset").toInt();
    }

    m_config.initialized = true;
    saveConfig();

    String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Configuration Saved</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 600px;
      margin: 50px auto;
      padding: 20px;
      background: #f0f0f0;
    }
    .container {
      background: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      text-align: center;
    }
    .success {
      color: #4CAF50;
      font-size: 48px;
      margin: 20px 0;
    }
    h1 {
      color: #333;
    }
    .message {
      color: #666;
      margin: 20px 0;
      font-size: 16px;
    }
    .button {
      display: inline-block;
      padding: 12px 30px;
      background: #4CAF50;
      color: white;
      text-decoration: none;
      border-radius: 5px;
      margin: 10px 5px;
    }
  </style>
  <script>
    setTimeout(function() {
      window.location.href = '/';
    }, 5000);
  </script>
</head>
<body>
  <div class="container">
    <div class="success">✓</div>
    <h1>Configuration Saved!</h1>
    <p class="message">Your settings have been saved successfully.</p>
    <p class="message">Redirecting to dashboard in 5 seconds...</p>
    <a href="/" class="button">Go to Dashboard Now</a>
  </div>
</body>
</html>
)=====";
    m_server->send(200, "text/html", html);
  }

  void handleStatus() {
    String json = "{";
    json += "\"wifi_ssid\":\"" + String(m_config.wifi_ssid) + "\",";
    json += "\"wifi_connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
    json += "\"ip_address\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
    json += "\"weather_place_id\":\"" + String(m_config.weather_place_id) + "\",";
    json += "\"utc_offset\":" + String(m_config.utc_offset) + ",";
    json += "\"free_heap\":" + String(ESP.getFreeHeap());
    json += "}";
    m_server->send(200, "application/json", json);
  }

  void handleFactoryReset() {
    String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Factory Reset - Smart Weather Clock</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 600px;
      margin: 50px auto;
      padding: 20px;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
    }
    .container {
      background: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 4px 6px rgba(0,0,0,0.1);
    }
    h1 {
      color: #667eea;
      text-align: center;
      margin-bottom: 30px;
    }
    .warning {
      background: #fff3cd;
      border: 2px solid #ffc107;
      padding: 15px;
      border-radius: 5px;
      margin: 20px 0;
      color: #856404;
    }
    .button {
      display: inline-block;
      padding: 12px 30px;
      margin: 10px 5px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
      text-decoration: none;
      text-align: center;
    }
    .button-danger {
      background: #dc3545;
      color: white;
    }
    .button-danger:hover {
      background: #c82333;
    }
    .button-secondary {
      background: #6c757d;
      color: white;
    }
    .button-secondary:hover {
      background: #5a6268;
    }
    .button-container {
      text-align: center;
      margin-top: 30px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>⚠️ Factory Reset</h1>
    <div class="warning">
      <strong>WARNING!</strong> This will erase all settings including:
      <ul>
        <li>WiFi credentials</li>
        <li>Weather API key</li>
        <li>Location settings</li>
        <li>Time zone configuration</li>
      </ul>
      <p>The device will restart and you'll need to reconfigure everything.</p>
    </div>
    <div class="button-container">
      <a href="/" class="button button-secondary">Cancel</a>
      <a href="/reset?confirm=yes" class="button button-danger" 
         onclick="return confirm('Are you absolutely sure? This cannot be undone!')">
        Reset All Settings
      </a>
    </div>
  </div>
</body>
</html>
)=====";

    // If confirmed, do the reset
    if (m_server->hasArg("confirm") && m_server->arg("confirm") == "yes") {
      m_server->send(200, "text/html", R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Resetting...</title>
  <meta http-equiv="refresh" content="5;url=http://192.168.4.1">
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      padding: 50px;
      background: #667eea;
      color: white;
    }
  </style>
</head>
<body>
  <h1>Factory Reset Complete</h1>
  <p>All settings have been erased.</p>
  <p>Device is restarting...</p>
  <p>Connect to WiFi network "Smart_Clock_Setup" (password: 12345678)</p>
</body>
</html>
)=====");
      delay(1000);
      factoryReset();
    } else {
      m_server->send(200, "text/html", html);
    }
  }
};

#endif
