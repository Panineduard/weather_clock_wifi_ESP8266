# ⏰ ESP8266 Weather Clock

A smart weather clock with OLED display that shows real-time clock, 3-day weather forecast, and optional indoor climate monitoring. Features a web-based configuration interface and memory-optimized streaming weather data parser.

**Hardware compatible with**: [AliExpress ESP8266 Weather Clock Kit](https://www.aliexpress.com/item/1005006706809259.html)  
**Software**: Built from scratch with custom optimizations

![License](https://img.shields.io/badge/license-MIT-blue.svg)  
![Arduino](https://img.shields.io/badge/Arduino-Compatible-green.svg)  
![ESP8266](https://img.shields.io/badge/ESP8266-Compatible-orange.svg)

## 🌟 Key Features

✅ **Custom-Built Software** - Written from scratch for optimal performance  
✅ **HTU21D Indoor Climate Sensor** - Monitor both indoor and outdoor conditions  
✅ **Memory-Optimized Parser** - Streaming JSON parser saves ~2KB RAM  
✅ **Professional Web UI** - Factory reset, status API, responsive design  
✅ **Comprehensive Documentation** - Complete setup and reference guides

## ✨ Features

*   🕐 **Real-time Clock** - NTP-synchronized time and date display
*   🌤️ **3-Day Weather Forecast** - Visual weather icons with outdoor min/max temperatures
*   🌡️ **Home Climate Monitoring** - Optional HTU21D sensor for indoor temperature & humidity
*   🌐 **Web Configuration** - Easy setup through browser interface
*   💾 **Persistent Settings** - Configuration stored in EEPROM
*   🔄 **Auto-switching Display** - Cycles through time, weather, and home climate modes
*   📱 **Responsive Interface** - Works on phones, tablets, and computers
*   🏠 **Indoor + Outdoor** - Monitor both home climate and outdoor weather conditions
*   🎨 **Custom Weather Icons** - 16x16 pixel monochrome icons for all weather conditions

## 🎯 Display Modes

The clock automatically cycles through display modes:

### Standard Modes (Always Available)

1.  **Time Mode** (20 seconds) - Large time, date, and current temperature  
    ![Time Mode Img](/img/TIME.jpg)

*   Shows **indoor** temperature if HTU21D connected
*   Shows **outdoor** temperature from weather API if no sensor

1.  **Weather Mode** (10 seconds) - 3-day **outdoor** forecast with icons and temperatures

### Optional Home Climate Mode

1.  **Home Climate Mode** (10 seconds) - **Indoor** temperature, humidity, and comfort level
    *   Only appears when HTU21D sensor is connected
    *   Shows real-time home climate data
    *   Displays "Home Climate" title to distinguish from outdoor weather
    *   See [HTU21D Setup Guide](HTU21D_SETUP_GUIDE.md) for wiring and setup

**Key Benefit:** With HTU21D, you get **both indoor and outdoor** information - monitor your home comfort while staying informed about outside weather!

## 📋 Table of Contents

*   [Hardware Requirements](#-hardware-requirements)
*   [Software Requirements](#-software-requirements)
*   [Installation](#-installation)
*   [Wiring Diagram](#-wiring-diagram)
*   [First Time Setup](#-first-time-setup)
*   [Web Interface](#-web-interface)
*   [Configuration](#-configuration)
*   [API Setup](#-api-setup)
*   [Memory Optimization](#-memory-optimization)
*   [Troubleshooting](#-troubleshooting)
*   [License](#-license)

## 🔧 Hardware Requirements

### Required Components

*   **ESP8266 Board** (NodeMCU, Wemos D1 Mini, or similar)
*   **SSD1306 OLED Display** (128x64 pixels, I2C interface)
*   **Micro USB Cable** (for programming and power)

### Optional Components

*   **HTU21D Temperature & Humidity Sensor** (for indoor home climate monitoring)
    *   Measures indoor temperature and humidity
    *   Provides home comfort level indicator
    *   Connects to same I2C bus as OLED
    *   See [HTU21D Setup Guide](HTU21D_SETUP_GUIDE.md) for wiring and setup
    *   Gives you both indoor (home) and outdoor (weather API) data

### Wiring Connections

| OLED Display | ESP8266 Pin | Description |
| --- | --- | --- |
| VCC | 3.3V | Power |
| GND | GND | Ground |
| SDA | GPIO0 (D3) | I2C Data |
| SCL | GPIO2 (D4) | I2C Clock |

**Note:** HTU21D sensor uses the same I2C bus (see [HTU21D Setup Guide](HTU21D_SETUP_GUIDE.md))

## 💻 Software Requirements

### Arduino IDE

*   **Arduino IDE 1.8.x or 2.x** - [Download here](https://www.arduino.cc/en/software)

### Board Support

1.  Open Arduino IDE
2.  Go to **File > Preferences**
3.  Add this URL to "Additional Boards Manager URLs":
4.  Go to **Tools > Board > Boards Manager**
5.  Search for "ESP8266" by ESP8266 Community
6.  Click **Install**

### Required Libraries

Install the following libraries through **Sketch > Include Library > Manage Libraries**:

| Library Name | Version | Purpose |
| --- | --- | --- |
| WiFiManager | by tzapu | WiFi connection management |
| Adafruit GFX Library | Latest | Graphics library for OLED |
| Adafruit SSD1306 | Latest | OLED display driver |
| NTPClient | by Fabrice Weinberg | Time synchronization |
| Adafruit HTU21DF Library | Latest | HTU21D sensor (optional) |

**Note:** ESP8266WiFi and ESP8266WebServer are built-in with ESP8266 board package.

## 📥 Installation

### 1\. Download the Project

```
git clone https://github.com/yourusername/esp8266-weather-clock.git
cd esp8266-weather-clock
```

### 2\. Open in Arduino IDE

1.  Open `weather_clock.ino` in Arduino IDE
2.  Make sure all `.h` files are in the same directory

### Project Files

```
weather_clock/
├── weather_clock.ino       # Main sketch file
├── ConfigManager.h         # Web configuration interface
├── TimeClient.h            # NTP time synchronization
├── WeatherService.h        # Streaming weather API parser
├── WeatherIcons.h          # 16x16 weather icon bitmaps
├── HTU21DService.h         # Indoor climate sensor (optional)
├── README.md               # This file
├── HTU21D_SETUP_GUIDE.md   # Sensor wiring guide
├── DISPLAY_LAYOUT_GUIDE.md # Screen layout reference
└── WEATHER_ICONS_GUIDE.md  # Icon design reference
```

### 3\. Upload the Code

1.  Click **Verify** (checkmark) to compile
2.  Click **Upload** (arrow) to flash
3.  Wait for "Done uploading" message
4.  Open **Serial Monitor** (115200 baud) to see status

## 🔌 Wiring Diagram

### Basic Setup (OLED Only)

```
ESP8266          SSD1306 OLED
┌──────────┐    ┌──────────┐
│          │    │          │
│   3.3V   ├────┤   VCC    │
│    GND   ├────┤   GND    │
│GPIO0 (D3)├────┤   SDA    │  I2C Data
│GPIO2 (D4)├────┤   SCL    │  I2C Clock
│          │    │          │
└──────────┘    └──────────┘
```

### With HTU21D Sensor (Indoor Climate)

```
ESP8266          SSD1306 OLED     HTU21D Sensor
┌──────────┐    ┌──────────┐    ┌──────────┐
│   3.3V   ├────┤   VCC    ├────┤   VCC    │
│    GND   ├────┤   GND    ├────┤   GND    │
│GPIO0 (D3)├────┤   SDA    ├────┤   SDA    │  Shared I2C
│GPIO2 (D4)├────┤   SCL    ├────┤   SCL    │  Shared I2C
└──────────┘    └──────────┘    └──────────┘
```

**Important:** Both devices share the same I2C bus. No additional pins needed!

See [HTU21D Setup Guide](HTU21D_SETUP_GUIDE.md) for detailed wiring photos and troubleshooting.

## 🚀 First Time Setup

### Step 1: Initial WiFi Setup

On first boot, the device creates a WiFi access point:

**Connect to WiFi Network:**

*   SSID: `Smart_Clock_Setup`
*   Password: `12345678`

**Configure WiFi:**

*   Browser will auto-open configuration page
*   If not, navigate to `http://192.168.4.1`
*   Select your WiFi network and enter password
*   Click **Save**

**Device Restarts:**

*   Connects to your WiFi network
*   Gets IP address via DHCP

### Step 2: Access Web Interface

Find device IP address:

*   Check Serial Monitor (115200 baud)
*   Usually shown on OLED during startup

Open web browser and navigate to device IP:

You'll see the Weather Clock Dashboard

## 🌐 Web Interface

### Dashboard (`/`)

Main page showing:

*   WiFi connection status
*   Current IP address
*   Location setting
*   Links to configuration and status pages

### Configuration Page (`/config`)

Configure all settings:

**WiFi Settings:**

*   SSID and Password
*   Stored in EEPROM

**Weather API Settings:**

*   MeteoSource API Key
*   Place ID (location)

**Time Settings:**

*   UTC Offset (timezone)

### Status API (`/status`)

Returns JSON with current status:

```
{
  "wifi_ssid": "YourNetwork",
  "wifi_connected": true,
  "ip_address": "192.168.0.138",
  "rssi": -45,
  "weather_place_id": "postal-ua-79007",
  "utc_offset": 2,
  "free_heap": 24304
}
```

### Factory Reset (`/reset`)

Erases all settings and restarts device in AP mode.

## ⚙️ Configuration

### Default Settings

First boot creates these defaults (edit in `ConfigManager.h`):

```cpp
wifi_ssid: "YOUR_WIFI_SSID"
wifi_password: "YOUR_PASSWORD"
weather_api_key: "YOUR_METEOSOURCE_KEY"
weather_place_id: "postal-ua-79007"  // Lviv, Ukraine
utc_offset: 2  // EET (Eastern European Time)
```

### Changing Configuration

**Via Web Interface** (Recommended):

1.  Navigate to `http://DEVICE_IP/config`
2.  Update values
3.  Click **Save Configuration**
4.  Device restarts with new settings

## 🌤️ API Setup

### MeteoSource Weather API

**Official Website**: [https://www.meteosource.com](https://www.meteosource.com)

MeteoSource provides accurate weather forecasts with **full Ukraine support** including all Ukrainian cities and postal codes.

**Get Free API Key:**

*   Visit [meteosource.com](https://www.meteosource.com)
*   Click "Sign Up" or "Get API Key"
*   Register for free account
*   Verify your email
*   Get your API key from dashboard
*   **Free tier**: 400 requests/day (more than enough for hourly updates)

**Find Your Place ID:**

**Option A - Postal Code:**

**Option B - City Name:**

**Option C - Locations:**

**Enter in Web Interface:**

*   Go to `http://DEVICE_IP/config`
*   Paste API Key
*   Enter Place ID
*   Click Save

**Note**: MeteoSource has excellent coverage with accurate local forecasts!

### Weather Update Interval

Weather updates every **1 hour** by default. To change, edit in `weather_clock.ino`:

```cpp
const unsigned long weatherInterval = 1UL * 60UL * 60UL * 1000UL;  // 1 hour
```

## ⚡ Memory Optimization

This project uses **streaming JSON parsing** to minimize RAM usage on ESP8266's limited memory (80KB RAM).

### Key Optimizations

**No Full Payload Storage**

*   Traditional approach: Store entire ~3KB JSON response in RAM
*   Our approach: Parse on-the-fly with 100-byte buffer
*   **Savings: ~2KB RAM**

**Streaming Parser**

**Minimal Serial Output**

*   Only essential messages during operation
*   Reduced string literals in Flash memory

**Display Initialization Timing**

*   Display initialized early for status messages
*   OLED buffer (1KB) allocated at startup
*   Stable operation with 23KB+ free heap

### Memory Usage

```
Setup Phase:
├─ WiFi Connected:        ~27KB free
├─ Display Init:          ~26KB free (1KB for OLED buffer)
├─ Weather Fetch (SSL):   ~24KB free (uses ~2-3KB during HTTPS)
└─ Running:               ~23KB free stable

Weather Service RAM:
├─ DailyWeather[3]:       ~150 bytes
├─ Parser buffer:         ~100 bytes
└─ Temporary strings:     ~50 bytes
Total:                    ~300 bytes (vs 2KB traditional)
```

## 🔧 Troubleshooting

### Display Issues

**White screen on startup:**

*   Normal garbage data before initialization
*   Clears after "WiFi connected!" message
*   Display initializes early, may show random pixels briefly

**Nothing on display:**

*   Check wiring (SDA=GPIO0, SCL=GPIO2)
*   Verify 3.3V power (NOT 5V!)
*   Try different I2C address (0x3C or 0x3D)
*   Check Serial Monitor for "Display initialized" message

**Flickering display:**

*   Check power supply stability
*   Use shorter jumper wires
*   Add 100µF capacitor between VCC and GND

### WiFi Issues

**Can't connect to Smart\_Clock\_Setup:**

*   Wait 30 seconds after power-on
*   Check password: `12345678`
*   Try restarting device

**Can't connect to home WiFi:**

*   Check SSID and password spelling
*   Ensure 2.4GHz network (ESP8266 doesn't support 5GHz)
*   Check router allows new devices

**Random disconnects:**

*   Check WiFi signal strength (RSSI in status page)
*   Move device closer to router
*   Check for interference

### Weather API Issues

**"Weather update failed":**

*   Verify API key is correct
*   Check API quota (400/day free tier)
*   Verify Place ID format
*   Check internet connection

**Wrong location:**

*   Verify Place ID: `postal-{country}-{code}`
*   Use lowercase
*   No spaces or special characters

**Temperatures showing 0.0:**

*   API request failed (check Serial Monitor)
*   Invalid Place ID
*   API key quota exceeded
*   Look for "Weather: Connecting...FAIL" in Serial

### HTU21D Sensor Issues

**"HTU21D not found":**

*   Check wiring (uses same I2C as OLED)
*   Verify sensor address (usually 0x40)
*   Try `i2c_scanner` sketch to detect devices

**Incorrect readings:**

*   Wait 2 seconds between readings
*   Check for proper airflow around sensor
*   Verify 3.3V power supply

See [HTU21D Setup Guide](HTU21D_SETUP_GUIDE.md) for detailed troubleshooting.

### Memory Issues

**ESP crashes/restarts randomly:**

*   Memory exhaustion
*   Check free heap in Serial Monitor
*   Typical stable: 23KB+
*   Look for values below 20KB before crash

**"Out of Memory" errors:**

*   Don't add large libraries
*   Streaming parser already implemented
*   Monitor with `ESP.getFreeHeap()`

### Serial Monitor Tips

Open Serial Monitor (115200 baud) to see:

```
Display initialized
Start to connect
HTTP server started
IP Address: 192.168.0.138
Free heap: 26984
Weather: Connecting...OK
Weather: Parsing...OK
Free heap after weather: 24304
Using API temp: -0.50
HTU21D not found - continuing without local sensor...
System ready!
```

## 📚 Additional Documentation

*   [**HTU21D Setup Guide**](HTU21D_SETUP_GUIDE.md) - Indoor sensor wiring and troubleshooting
*   [**Display Layout Guide**](DISPLAY_LAYOUT_GUIDE.md) - Screen design and pixel coordinates
*   [**Weather Icons Guide**](WEATHER_ICONS_GUIDE.md) - Icon designs and MeteoSource mapping

## 🎨 Customization

### Display Timing

Edit in `weather_clock.ino`:

```cpp
const unsigned long modeInterval = 10000;  // 10 seconds per mode
long multiplyer = 2;  // TIME mode shows 2x longer (20 seconds)
```

### Temperature Units

Currently displays Celsius. To add Fahrenheit, modify in `showTime()` and `showWeather()`:

```cpp
String tempStr = String((currentTemp * 9/5) + 32, 1) + "F";
```

### Weather Icons

Custom 16x16 pixel icons in `WeatherIcons.h`. Edit bitmaps to customize appearance.

See [Weather Icons Guide](WEATHER_ICONS_GUIDE.md) for design reference.

### Temperature Decimal Places

Toggle decimal places in forecast, edit in `weather_clock.ino`:

```cpp
// Show decimals (e.g., -2.2°C)
String min0 = String(day0.tempMin, 1);

// Show whole numbers (e.g., -2°C)
String min0 = String((int)round(day0.tempMin));
```

## 🔐 Security Notes

*   **Default WiFi Password:** Change `12345678` in `wifiManager.autoConnect()`
*   **Web Interface:** No authentication - don't expose to internet
*   **API Keys:** Stored in EEPROM (plaintext) - device should be physically secure

## 🛠️ Development

### Adding New Features

1.  Keep RAM usage minimal
2.  Use streaming parsers for large data
3.  Test with `ESP.getFreeHeap()` monitoring
4.  Update documentation

### Testing

Monitor heap during development:

```cpp
Serial.print("Free heap: ");
Serial.println(ESP.getFreeHeap());
```

Minimum stable: 20KB+

### Debug Flag

Enable debug mode without display:

```cpp
#define DEBUG_NO_DISPLAY true  // Outputs to Serial only
```

## 🤝 Contributing

Contributions welcome! Please:

1.  Test thoroughly on ESP8266 hardware
2.  Monitor memory usage
3.  Update documentation
4.  Follow existing code style

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

*   **MeteoSource** - Weather API
*   **Adafruit** - OLED and sensor libraries
*   **ESP8266 Community** - Arduino core
*   **WiFiManager** by tzapu - WiFi configuration

---

**Made with ❤️ for ESP8266**

_Current Temperature & Forecast - Always Up to Date_ 🌤️

```cpp
// Read byte-by-byte, extract only needed fields
while (client.available()) {
  char c = client.read();
  buffer += c;
  if (buffer.endsWith("\"temperature_min\":")) {
    String num = readNumber();
    m_daily[dayIndex].tempMin = num.toFloat();
  }
}
```

```
Postal: postal-us-10001, postal-gb-sw1a, postal-de-10115
Cities: london, new-york, paris, berlin
```

```
Examples:
- lviv
- kyiv
- odesa
- kharkiv
- dnipro
```

```
Format: postal-ua-{code}
Examples:
- postal-ua-79007 (Lviv)
- postal-ua-01001 (Kyiv)
- postal-ua-65000 (Odesa)
- postal-ua-49000 (Dnipro)
- postal-ua-61000 (Kharkiv)
```

```
http://192.168.0.XXX
```

```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```